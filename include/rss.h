#ifndef RSSREADER_RSSCHAN_H
#define RSSREADER_RSSCHAN_H

#define RCHAN_UI_H 50

typedef struct {
    char *title;
    char *link;

    // items
    // nitems

    // optional stuff later
} rchan;

void rchan_draw(rchan *c, int i);

rchan *rchan_init(char *l, char *t);
rchan **rss_parsef(const char *fp, int *n);
void rchan_free(rchan *ch);
void rchanarr_free(rchan **ch, int n);

#endif
