#include <raylib.h>
#include <stdio.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

int main(void) {
    InitWindow(800, 600, "rssreader");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        EndDrawing();
    }

    return 0;
}
