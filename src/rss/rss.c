#include "rss/rss.h"
#include <curl/curl.h>
#include <raygui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "main.h"
#include "net.h"

#define CHK_ASSIGN(tagname, taglen, field)                                     \
    if (end - p > taglen && memcmp(p, tagname, taglen) == 0) {                 \
        p = skip_tag(p, end);                                                  \
        char *start = p;                                                       \
        while (p < end && *p != '<') /* closing tag */                         \
            p++;                                                               \
        size_t len = p - start;                                                \
        field = malloc(len + 1);                                               \
        if (!field) {                                                          \
            fprintf(stderr, "unable to allocate memory for %s\n", #field);     \
            return NULL;                                                       \
        }                                                                      \
        memcpy(field, start, len);                                             \
        field[len] = '\0';                                                     \
    }

static net_mem *rchan_recv(CURL *curl, const char *uri);
static rchan *rchan_parse(net_mem *m);
static rchan *rchan_parsecontent(char *p, char *end);
static inline char *skip_tag(char *p, char *end);

rchan **rss_parsef(CURL *curl, const char *fp, int *n) {
    rchan **chs = NULL;

    char *rsslinks = io_readf(fp);
    char *l = strtok(rsslinks, "\n");

    *n = 0;
    while (l != NULL) {
        chs = realloc(chs, ((*n) + 1) * sizeof(*chs));
        if (!chs)
            pfail(NULL, "couldn't allocate memory while parsing \"%s\"\n", fp);

        net_mem *m = rchan_recv(curl, l);
        chs[*n] = rchan_parse(m);
        net_mem_free(m);

        (*n)++;
        l = strtok(NULL, "\n");
    }

    free(rsslinks);

    return chs;
}

void rchan_free(rchan *ch) {
    if (ch->link)
        free(ch->link);
    if (ch->title)
        free(ch->title);
    free(ch);
}

void rchanarr_free(rchan **chs, int n) {
    for (int i = 0; i < n; i++)
        if (chs[i])
            rchan_free(chs[i]);

    free(chs);
}

static net_mem *rchan_recv(CURL *curl, const char *uri) {
    net_mem *m = net_mem_alloc();
    CURLcode res = net_recv(curl, uri, m);
    if (res != CURLE_OK)
        pfail(NULL, "net_recv failed: %s\n", curl_easy_strerror(res));
    return m;
}

static rchan *rchan_parse(net_mem *m) {
    char *p = m->ptr;
    char *end = m->ptr + m->len;

    while (p < end) {
        if (*p == '<') {
            p++; // skip '<'

            if ((end - p >= 7) && memcmp(p, "channel", 7) == 0)
                return rchan_parsecontent(p = skip_tag(p, end), end);

            p = skip_tag(p, end);
        } else
            p++;
    }

    return NULL;
}

static rchan *rchan_parsecontent(char *p, char *end) {
    rchan *ch = calloc(1, sizeof(rchan));
    if (!ch)
        pfail(NULL,
              "unable to allocate memory for ch parsing channel content\n");

    while (p < end) {
        if (*p != '<') {
            p++;
            continue;
        } // skip text
        p++; // skip '<'

        CHK_ASSIGN("title", 5, ch->title);
        CHK_ASSIGN("link", 4, ch->link);

        if ((end - p >= 4) && memcmp(p, "item", 4) == 0) {
            // item parsing

            // skipping item tag for now
            if (end - p >= 4 && memcmp(p, "item", 4) == 0) {
                p = skip_tag(p, end);
                while (p < end && memcmp(p, "</item>", 7) != 0)
                    p++;
                p += 7;
                continue;
            }
            continue;
        }

        // skip any unknown tag
        p = skip_tag(p, end);
    }

    return ch;
}

static inline char *skip_tag(char *p, char *end) {
    while (p < end && *p != '>')
        p++;
    if (p < end)
        p++; // skip '>'
    return p;
}
