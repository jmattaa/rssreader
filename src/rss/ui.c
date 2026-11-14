#include "rss/ui.h"
#include <raygui.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

static Vector2 scroll_chs = {0, 0};
static Vector2 scroll_items = {0, 0};
static void begin_scroll(Rectangle bounds, Rectangle content, Vector2 *scroll);

static void open_url(const char *url);

void rchan_drawall(rchan **chs, int nlinks, rchan **chdrw) {
    Rectangle bounds = {0, 0, win_w, win_h};
    Rectangle content = {0, 0, RBTN_UI_W, RBTN_UI_H * nlinks};

    begin_scroll(bounds, content, &scroll_chs);

    for (int i = 0; i < nlinks; i++) {
        int starth = RBTN_UI_H * i + scroll_chs.y;

        if (GuiButton((Rectangle){0, starth, RBTN_UI_W, RBTN_UI_H},
                      chs[i]->title)) {
            *chdrw = chs[i];
        }
    }

    EndScissorMode();
}

void rchan_drawitems(rchan *c) {
    Rectangle bounds = {0, 0, win_w, win_h};
    Rectangle content = {0, 0, RBTN_UI_W, RBTN_UI_H * c->nitems};

    begin_scroll(bounds, content, &scroll_items);

    for (size_t i = 0; i < c->nitems; i++) {
        int starth = RBTN_UI_H * i + scroll_items.y;

        if (GuiButton((Rectangle){0, starth, RBTN_UI_W, RBTN_UI_H},
                      c->items[i]->title)) {
            open_url(c->items[i]->link);
        }
    }

    EndScissorMode();
}

static void begin_scroll(Rectangle bounds, Rectangle content, Vector2 *scroll) {
    GuiScrollPanel(bounds, NULL, content, scroll, NULL);
    BeginScissorMode(bounds.x, bounds.y, bounds.width, bounds.height);
}

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
