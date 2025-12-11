#ifndef RSSREADER_RSS_H
#define RSSREADER_RSS_H

#include <stddef.h>

typedef struct
{
    char *title;
    char *link;

    // add optional shi later on
} ritem;

typedef struct
{
    char *title;
    char *link;

    ritem **items;
    size_t nitems;

    // optional shi later on
} rchannel;

// parse a rss channel
rchannel *rss_parse(const char *xml, size_t len);

void rss_free(rchannel *channel);

void dbg_print_rchannel(rchannel *channel);

#endif
