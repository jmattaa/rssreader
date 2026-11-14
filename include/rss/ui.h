#ifndef RSSREADER_RSS_UI_H
#define RSSREADER_RSS_UI_H

#include "rss/rss.h"

#define RBTN_UI_H 50
#define RBTN_UI_W win_w - 14

void rchan_drawall(rchan **chs, int nlinks, rchan **chdrw);
void rchan_drawitems(rchan *c);

#endif
