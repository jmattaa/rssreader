#include "rss/ui.h"
#include <raygui.h>
#include <stdio.h>
#include "main.h"

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
            printf("%s\n", c->items[i]->link);
        }
    }
}
