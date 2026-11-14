#ifndef RSSREADER_RSS_RSSCHAN_H
#define RSSREADER_RSS_RSSCHAN_H

#include <curl/curl.h>

typedef struct {
    char *title;
    char *link;
} ritem;

typedef struct {
    char *title;
    char *link;

    ritem **items;
    size_t nitems;

    // optional stuff later
} rchan;

rchan **rss_parsef(CURL *curl, const char *fp, int *n);

void ritem_free(ritem *i);
void rchan_free(rchan *ch);
void rchanarr_free(rchan **ch, int n);

#endif
