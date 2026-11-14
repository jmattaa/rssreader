#include "rss/ui.h"
#include <raygui.h>
#include <stdio.h>
#include "main.h"

static void open_url(const char *url);

void rchan_draw(rchan *c, int i, rchan **chdrw) {
    int starth = RBTN_UI_H * i;
    if (GuiButton((Rectangle){0, starth, win_w, RBTN_UI_H}, c->title)) {
        *chdrw = c;
    }
}

void rchan_drawitems(rchan *c) {
    for (size_t i = 0; i < c->nitems; i++) {
        int starth = RBTN_UI_H * i;
        if (GuiButton((Rectangle){0, starth, win_w, RBTN_UI_H},
                      c->items[i]->title)) {
            open_url(c->items[i]->link);
        }
    }
}

#include <stdlib.h>

static void open_url(const char *url) {
#ifdef _WIN32
    char cmd[2048];
    snprintf(cmd, sizeof(cmd), "start \"\" \"%s\"", url);
#elif __APPLE__
    char cmd[2048];
    snprintf(cmd, sizeof(cmd), "open \"%s\"", url);
#else
    char cmd[2048];
    snprintf(cmd, sizeof(cmd), "xdg-open \"%s\"", url);
#endif

    system(cmd);
}
