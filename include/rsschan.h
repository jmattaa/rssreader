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

#endif
