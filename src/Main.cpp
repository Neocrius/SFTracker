#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#undef RAYGUI_IMPLEMENTATION

int main()
{
    int screenWidth = 1280;
    int screenHeight = 800;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "SFTracker");
    SetTargetFPS(60);

    GuiLoadStyleDefault();

    while (!WindowShouldClose())
    {
        screenWidth     = GetScreenWidth();
        screenHeight    = GetScreenHeight();

        BeginDrawing();
            ClearBackground(GetColor( GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            GuiGroupBox((Rectangle){8, 8, float(screenWidth - 16), 256}, "SONG");
        EndDrawing();
    }

    CloseWindow();

    return 0;
}