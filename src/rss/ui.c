#include "rss/ui.h"
#include <raygui.h>
#include <stdio.h>
#include "main.h"

void rchan_draw(rchan *c, int i) {
    int starth = RCHAN_UI_H * i;
    if (GuiButton((Rectangle){0, starth, win_w, RCHAN_UI_H}, c->title)) {
        printf("%s\n", c->link);
    }
}
