#ifndef RSSREADER_RSS_RSSCHAN_H
#define RSSREADER_RSS_RSSCHAN_H

typedef struct {
    char *title;
    char *link;

    // items
    // nitems

    // optional stuff later
} rchan;

rchan *rchan_init(char *l, char *t);
rchan **rss_parsef(const char *fp, int *n);

void rchan_free(rchan *ch);
void rchanarr_free(rchan **ch, int n);

#endif
