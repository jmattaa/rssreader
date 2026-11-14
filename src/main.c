#include "main.h"
#include <stdio.h>
#include <raylib.h>
#include "rss/rss.h"
#include "rss/ui.h"

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

int win_w = 800, win_h = 600;

int main(void) {
    CURL *curl = curl_easy_init();
    if (!curl)
        pfail(1, "couldn't init curl\n");

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(win_w, win_h, "rssreader");
    SetTargetFPS(60);

    int nlinks;
    rchan **chs = rss_parsef(curl, "rsslinks", &nlinks);
    if (!chs)
        pfail(1, "couldn't parse rsslinks file");

    while (!WindowShouldClose()) {
        if (IsWindowResized())
            win_w = GetScreenWidth(), win_h = GetScreenHeight();

        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        for (int i = 0; i < nlinks; i++)
            rchan_draw(chs[i], i);

        EndDrawing();
    }

    rchanarr_free(chs, nlinks);
    curl_easy_cleanup(curl);

    return 0;
}
