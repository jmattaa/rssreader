#ifndef RSSREADER_RSS_H
#define RSSREADER_RSS_H

#include "curl/curl.h"
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

typedef struct
{
    rchannel **channels;
    size_t nchannels;
} rchannelset;

// parse a rss channel
rchannel *rss_recvandparse(CURL *curl, const char *uri);
void rss_free(rchannel *channel);

rchannelset *rss_recvlinkf(CURL *curl, const char *fp);
void rss_free_set(rchannelset *set);

void dbg_print_rchannel(rchannel *channel);

#endif
