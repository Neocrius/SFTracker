#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#undef RAYGUI_IMPLEMENTATION

#include <algorithm>
#include <string>

#define TOOLBAR_HEIGHT      32
#define TOOL_BUTTON_WIDTH   32
#define TOP_PANEL_Y         (8 + TOOLBAR_HEIGHT + 8)
#define TOP_PANEL_H         256
#define TOP_PANEL_W         800


#define PATTERN_ORDER_W     (TRACK_CHECK_W * 16 + 16)
#define PATTERN_ORDER_H     72

#define TRACK_MASK_H        (16 + 2 * 8)
#define TRACK_CHECK_W       32

void DrawSongPanel(Rectangle bounds)
{
    //TODO: scrollbar when bounds.width < limit?

    //draw pattern order
    GuiGroupBox((Rectangle){bounds.x, bounds.y, PATTERN_ORDER_W, PATTERN_ORDER_H}, "Pattern Order");

    //TODO: calculate scrollbar slider width based on the number of patterns in the order
    //GuiSetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE, (int)(((bounds.width - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - verticalScrollBarWidth)/(int)content.width)*((int)bounds.width - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - verticalScrollBarWidth)));
    /*order = */GuiScrollBar((Rectangle){bounds.x + 4, bounds.y + PATTERN_ORDER_H - 16 - 4, PATTERN_ORDER_W - 8, 16}, 0/*order*/, 0, 0 /*max*/);


    //draw track mask for current order
    const float trackMaskY = bounds.y + PATTERN_ORDER_H + 8;

    GuiGroupBox((Rectangle){bounds.x, trackMaskY, PATTERN_ORDER_W, TRACK_MASK_H}, "Track Mask");

    for (int track = 0; track < 16; ++track)
    {
        std::string trackS = std::to_string(track + 1);

        bool checked = true;
        GuiCheckBox((Rectangle){bounds.x + 8 + track * TRACK_CHECK_W, trackMaskY + 8, 16, 16}, trackS.c_str(), &checked);
    }

    int bpm = 96;
    GuiSpinner((Rectangle){bounds.x + PATTERN_ORDER_W + 32, bounds.y + 8, 80, 24}, "BPM ", &bpm, 1, 255, false);

    int lpb = 4;
    GuiSpinner((Rectangle){bounds.x + PATTERN_ORDER_W + 184, bounds.y + 8, 72, 24}, "Lines/Beat ", &lpb, 1, 16, false);

    int tpl = 6;
    GuiSpinner((Rectangle){bounds.x + PATTERN_ORDER_W + 184, bounds.y + 40, 72, 24}, "Ticks/Line ", &tpl, 1, 16, false);

    int octave = 4;
    GuiSpinner((Rectangle){bounds.x + PATTERN_ORDER_W + 184, bounds.y + 80, 72, 24}, "Kbd Octave ", &octave, 0, 7, false);

    int velocity = 100;
    GuiSpinner((Rectangle){bounds.x + PATTERN_ORDER_W + 32, bounds.y + 80, 80, 24}, "Vel. ", &velocity, 1, 127, false);
}

void DrawInstrumentList(Rectangle bounds)
{
    GuiGroupBox(bounds, "Instruments");
}

int main()
{
    int screenWidth = 1280;
    int screenHeight = 720;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "SFTracker");
    SetWindowMinSize(1024, 480);
    SetTargetFPS(60);

    GuiLoadStyleDefault();

    while (!WindowShouldClose())
    {
        screenWidth     = GetScreenWidth();
        screenHeight    = GetScreenHeight();

        int instListW   = screenWidth - TOP_PANEL_W - 24;

        BeginDrawing();
            ClearBackground(GetColor( GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
            
            //File Operations

            if (GuiButton((Rectangle){ 8, 8, TOOL_BUTTON_WIDTH, TOOLBAR_HEIGHT },
                GuiIconText(ICON_FILE_NEW, "")))
            {
                //New
            }
            if (GuiButton((Rectangle){ 8 + TOOL_BUTTON_WIDTH + 8, 8, TOOL_BUTTON_WIDTH, TOOLBAR_HEIGHT },
                GuiIconText(ICON_FILE_OPEN, "")))
            {
                //Open
            }
            if (GuiButton((Rectangle){ 8 + 2 * (TOOL_BUTTON_WIDTH + 8), 8, TOOL_BUTTON_WIDTH, TOOLBAR_HEIGHT },
                GuiIconText(ICON_FILE_SAVE_CLASSIC, "")))
            {
                //Save (as)
            }
            if (GuiButton((Rectangle){ 8 + 3 * (TOOL_BUTTON_WIDTH + 8), 8, TOOL_BUTTON_WIDTH, TOOLBAR_HEIGHT },
                GuiIconText(ICON_FILE_EXPORT, "")))
            {
                //Export MIDI
            }
            if (GuiButton((Rectangle){ 8 + 4 * (TOOL_BUTTON_WIDTH + 8), 8, TOOL_BUTTON_WIDTH, TOOLBAR_HEIGHT },
                GuiIconText(ICON_WAVE, "")))
            {
                //Render Audio
            }

            //Transport
#define TRANSPORT_X (TOP_PANEL_W / 2 + 8 - (TOOL_BUTTON_WIDTH * 5 + 8 * 4) / 2)

            if (GuiButton((Rectangle){ TRANSPORT_X, 8, TOOL_BUTTON_WIDTH, TOOLBAR_HEIGHT },
                GuiIconText(ICON_PLAYER_PLAY, "")))
            {
                //Play
            }
            if (GuiButton((Rectangle){ float(TRANSPORT_X + TOOL_BUTTON_WIDTH + 8), 8, TOOL_BUTTON_WIDTH, TOOLBAR_HEIGHT },
                GuiIconText(ICON_REPEAT, "")))
            {
                //Loop Pattern Toggle
            }
            if (GuiButton((Rectangle){ float(TRANSPORT_X + 2 * (TOOL_BUTTON_WIDTH + 8)), 8, TOOL_BUTTON_WIDTH, TOOLBAR_HEIGHT },
                GuiIconText(ICON_PLAYER_STOP, "")))
            {
                //Stop
            }
            if (GuiButton((Rectangle){ float(TRANSPORT_X + 3 * (TOOL_BUTTON_WIDTH + 8)), 8, TOOL_BUTTON_WIDTH, TOOLBAR_HEIGHT },
                GuiIconText(ICON_PLAYER_RECORD, "")))
            {
                //Record/Edit Toggle
            }
            if (GuiButton((Rectangle){ float(TRANSPORT_X + 4 * (TOOL_BUTTON_WIDTH + 8)), 8, TOOL_BUTTON_WIDTH, TOOLBAR_HEIGHT },
                GuiIconText(ICON_MAGNET, "")))
            {
                //Follow Playback Toggle
            }

#define TABS_WIDTH  (TOOL_BUTTON_WIDTH * 5 + 8 * 6)
            int tabsLeft = 8 + TOP_PANEL_W - TABS_WIDTH;

            GuiDrawRectangle((Rectangle){float(tabsLeft), 8, TABS_WIDTH, TOOLBAR_HEIGHT + 8}, RAYGUI_PANEL_BORDER_WIDTH,
                GetColor(GuiGetStyle(DEFAULT, (int)LINE_COLOR)),
                GetColor(GuiGetStyle(DEFAULT, (int)BACKGROUND_COLOR)));                
            GuiDrawRectangle((Rectangle){8, TOP_PANEL_Y, float(TOP_PANEL_W), TOP_PANEL_H}, RAYGUI_PANEL_BORDER_WIDTH,
                GetColor(GuiGetStyle(DEFAULT, (int)LINE_COLOR)),
                GetColor(GuiGetStyle(DEFAULT, (int)BACKGROUND_COLOR)));                
            GuiDrawRectangle((Rectangle){float(tabsLeft) + RAYGUI_PANEL_BORDER_WIDTH,
                                         8 + RAYGUI_PANEL_BORDER_WIDTH,
                                         TABS_WIDTH - 2 * RAYGUI_PANEL_BORDER_WIDTH,
                                         TOOLBAR_HEIGHT + 8 + RAYGUI_PANEL_BORDER_WIDTH}, 0,
                GetColor(GuiGetStyle(DEFAULT, (int)LINE_COLOR)),
                GetColor(GuiGetStyle(DEFAULT, (int)BACKGROUND_COLOR)));                

            GuiSetState(STATE_PRESSED);
            if (GuiButton((Rectangle){ float(tabsLeft + 8), 16, TOOL_BUTTON_WIDTH, TOOLBAR_HEIGHT },
                GuiIconText(ICON_FILETYPE_AUDIO, "")))
            {
                //Switch to Song Properties
            }
            GuiSetState(STATE_NORMAL);
            if (GuiButton((Rectangle){ float(tabsLeft + 8 + TOOL_BUTTON_WIDTH + 8), 16, TOOL_BUTTON_WIDTH, TOOLBAR_HEIGHT },
                GuiIconText(ICON_FILETYPE_AUDIO, "")))
            {
                //Switch to SoundFont and Instrument Properties
            }
            if (GuiButton((Rectangle){ float(tabsLeft + 8 + 2 * (TOOL_BUTTON_WIDTH + 8)), 16, TOOL_BUTTON_WIDTH, TOOLBAR_HEIGHT },
                GuiIconText(ICON_TOOLS, "")))
            {
                //Switch to Tools
            }
            if (GuiButton((Rectangle){ float(tabsLeft + 8 + 3 * (TOOL_BUTTON_WIDTH + 8)), 16, TOOL_BUTTON_WIDTH, TOOLBAR_HEIGHT },
                GuiIconText(ICON_GEAR_BIG, "")))
            {
                //Switch to Settings
            }
            if (GuiButton((Rectangle){ float(tabsLeft + 8 + 4 * (TOOL_BUTTON_WIDTH + 8)), 16, TOOL_BUTTON_WIDTH, TOOLBAR_HEIGHT },
                GuiIconText(ICON_FX, "")))
            {
                //Switch to FX Help
            }

            DrawSongPanel((Rectangle){16, TOP_PANEL_Y + 8, TOP_PANEL_W - 16, TOP_PANEL_H - 16});

            DrawInstrumentList((Rectangle){TOP_PANEL_W + 16, 8, float(instListW), TOP_PANEL_H + TOP_PANEL_Y - 8});

        EndDrawing();
    }

    CloseWindow();

    return 0;
}