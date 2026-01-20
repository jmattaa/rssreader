#include "rss.h"
#include "io.h"
#include "net.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static rchannel *parse_channel(char *p, char *end);
static ritem *parse_item(char *p, char *end);

static inline char *skip_tag(char *p, char *end)
{
    while (p < end && *p != '>')
        p++;
    if (p < end)
        p++; // skip '>'
    return p;
}

rchannel *rss_recvandparse(CURL *curl, const char *uri)
{
    net_mem *mem = net_mem_alloc();
    CURLcode res = net_recv(curl, uri, mem);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "net_recv failed: %s\n", curl_easy_strerror(res));
        return NULL;
    }

    char *p = mem->ptr;
    char *end = mem->ptr + mem->len;

    while (p < end)
    {
        if (*p == '<')
        {
            p++; // skip '<'

            if ((end - p >= 7) && memcmp(p, "channel", 7) == 0)
                return parse_channel(p = skip_tag(p, end), end);

            p = skip_tag(p, end);
        }
        else
            p++;
    }

    return NULL;
}

rchannelset *rss_recvlinkf(CURL *curl, const char *fp)
{
    rchannelset *chs = malloc(sizeof(rchannelset));
    if (!chs)
        return NULL;

    char *buf = io_read(fp);
    if (!buf)
    {
        free(chs);
        return NULL;
    }

    chs->channels = malloc(sizeof(rchannel *));
    if (!chs->channels)
    {
        free(buf);
        free(chs);
        printf("unable to allocate memory for channel\n");
        return NULL;
    }

    char *p = buf;
    size_t i = 0;

    while (*p)
    {
        char *nextl = strchr(p, '\n');
        if (nextl)
            *nextl = '\0';

        printf("%s\n", p);

        chs->channels = realloc(chs->channels, sizeof(rchannel *) * (i + 1));
        if (!chs->channels)
        {
            free(chs->channels);
            free(buf);
            free(chs);
            printf("unable to allocate memory for channel\n");
            return NULL;
        }

        chs->channels[i++] = rss_recvandparse(curl, p);

        if (!nextl)
            break;

        p = nextl + 1;
    }

    chs->nchannels = i;
    free(buf);
    return chs;
}

void rss_free_set(rchannelset *set)
{
    for (size_t i = 0; i < set->nchannels; i++)
        if (set->channels[i])
            rss_free(set->channels[i]);

    free(set->channels);
    free(set);
}

#define CHK_ASSIGN(tagname, taglen, field)                                     \
    if (end - p > taglen && memcmp(p, tagname, taglen) == 0)                   \
    {                                                                          \
        p = skip_tag(p, end);                                                  \
        char *start = p;                                                       \
        while (p < end && *p != '<') /* closing tag */                         \
            p++;                                                               \
        size_t len = p - start;                                                \
        field = malloc(len + 1);                                               \
        if (!field)                                                            \
        {                                                                      \
            fprintf(stderr, "unable to allocate memory for %s\n", #field);     \
            return NULL;                                                       \
        }                                                                      \
        memcpy(field, start, len);                                             \
        field[len] = '\0';                                                     \
    }

static rchannel *parse_channel(char *p, char *end)
{
    rchannel *channel = calloc(1, sizeof(rchannel));
    if (!channel)
    {
        fprintf(stderr, "unable to allocate memory for channel\n");
        return NULL;
    }

    while (p < end)
    {
        if (*p != '<')
        {
            p++;
            continue;
        } // skip text
        p++; // skip '<'

        CHK_ASSIGN("title", 5, channel->title);
        CHK_ASSIGN("link", 4, channel->link);

        if ((end - p >= 4) && memcmp(p, "item", 4) == 0)
        {
            p = skip_tag(p, end);
            ritem *item = parse_item(p, end);
            if (item)
            {
                channel->items = realloc(
                    channel->items, sizeof(ritem *) * (channel->nitems + 1));
                channel->items[channel->nitems++] = item;
            }
            p = skip_tag(p, end);
            continue;
        }

        // skip any unknown tag
        p = skip_tag(p, end);
    }

    return channel;
}

static ritem *parse_item(char *p, char *end)
{
    ritem *item = calloc(1, sizeof(ritem));
    if (!item)
    {
        fprintf(stderr, "unable to allocate memory for item\n");
        return NULL;
    }

    while (p < end)
    {
        if (*p != '<')
        {
            p++;
            continue;
        }
        p++; // skip '<'
        if ((end - p >= 6) && memcmp(p, "/item", 5) == 0)
        {
            // reached closing </item> tag
            p = skip_tag(p, end);
            break;
        }

        CHK_ASSIGN("title", 5, item->title);
        CHK_ASSIGN("link", 4, item->link);

        // skip any unknown tag
        p = skip_tag(p, end);
    }

    return item;
}

#undef CHK_ASSIGN

static inline void ritem_free(ritem *item)
{
    if (!item)
        return;
    free(item->title);
    free(item->link);
    free(item);
}
void rss_free(rchannel *channel)
{
    if (!channel)
        return;
    free(channel->title);
    free(channel->link);
    for (size_t i = 0; i < channel->nitems; i++)
        ritem_free(channel->items[i]);
    free(channel->items);
    free(channel);
}

static inline void dbg_print_item(ritem *item)
{
    if (!item)
        return;
    printf("title: %s\n", item->title);
    printf("link: %s\n", item->link);
}

void dbg_print_rchannel(rchannel *channel)
{
    if (!channel)
        return;
    printf("title: %s\n", channel->title);
    printf("link: %s\n", channel->link);
    for (size_t i = 0; i < channel->nitems; i++)
        dbg_print_item(channel->items[i]);
}
