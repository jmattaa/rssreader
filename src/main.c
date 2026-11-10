#include "main.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "rsschan.h"

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

int win_w = 800, win_h = 600;

int main(void) {
    InitWindow(win_w, win_h, "rssreader");
    SetTargetFPS(60);

    rchan *ch = malloc(sizeof(rchan));
    if (ch == NULL)
        pfail(1, "couldn't malloc for ch");
    ch->title = "Helloo";
    ch->link = "test link";

    while (!WindowShouldClose()) {
        if (IsWindowResized())
            win_w = GetScreenWidth(), win_h = GetScreenHeight();

        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        rchan_draw(ch, 0);

        EndDrawing();
    }

    free(ch);

    return 0;
}
