#include "Module.h"

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#undef RAYGUI_IMPLEMENTATION

#include <algorithm>
#include <string>

#define TOOLBAR_HEIGHT      32
#define TOOL_BUTTON_S   32
#define TOP_PANEL_Y         (8 + TOOLBAR_HEIGHT + 8)
#define TOP_PANEL_H         128
#define TOP_PANEL_W         800

#define PATTERN_ORDER_W     (TRACK_CHECK_W * 16 + 16)
#define PATTERN_ORDER_H     72

#define TRACK_MASK_H        (16 + 2 * 8)
#define TRACK_CHECK_W       32

/*
*   TODO: check CPU usage on Raspberry Pi to see whether it would make sense to switch to a custom frame loop
*   This requires recompiling Raylib with SUPPORT_CUSTOM_FRAME_CONTROL
*   Taken from Raylib comments:
*
*   NOTE: WARNING: This is an example for advanced users willing to have full control over
*   the frame processes. By default, EndDrawing() calls the following processes:
*       1. Draw remaining batch data: rlDrawRenderBatchActive()
*       2. SwapScreenBuffer()
*       3. Frame time control: WaitTime()
*       4. PollInputEvents()
*
*   To avoid steps 2, 3 and 4, flag SUPPORT_CUSTOM_FRAME_CONTROL can be enabled in
*   config.h (it requires recompiling raylib). This way those steps are up to the user.
*
*   Note that enabling this flag invalidates some functions:
*       - GetFrameTime()
*       - SetTargetFPS()
*       - GetFPS()
*/

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

void DrawInstrumentSettings(Rectangle bounds)
{
    //draw SoundFont list

#define INSTRUMENT_PROPS_W  ((TOP_PANEL_W - 24) / 2)
#define SOUNDFONT_LIST_W    (TOP_PANEL_W - INSTRUMENT_PROPS_W - 24)
#define SOUNDFONT_BUTTON_S  24

    GuiGroupBox((Rectangle){bounds.x, bounds.y, SOUNDFONT_LIST_W, TOP_PANEL_H - 16}, "SoundFonts");

    if (GuiButton((Rectangle){ bounds.x + 8, bounds.y + 8, SOUNDFONT_BUTTON_S, SOUNDFONT_BUTTON_S }, GuiIconText(ICON_FILE_OPEN, "")))
    {
        //Open & add a new SoundFont
    }

    if (GuiButton((Rectangle){ bounds.x + 8, bounds.y + 8 + SOUNDFONT_BUTTON_S + 4, SOUNDFONT_BUTTON_S, SOUNDFONT_BUTTON_S }, GuiIconText(ICON_CROSS, "")))
    {
        //Delete current SoundFont
    }

    GuiSetStyle(LISTVIEW, LIST_ITEMS_HEIGHT, 16);
    const char* soundFonts[] = { "~/SoundFonts/ASoundFont.sf2", "~/SoundFonts/AnotherSoundFont.sf2" };
    int sfIndex = 0;
    int active = 0;
    int focus = 0;
    GuiListViewEx((Rectangle){ bounds.x + 8 + SOUNDFONT_BUTTON_S + 4, bounds.y + 8, SOUNDFONT_LIST_W - SOUNDFONT_BUTTON_S - 8 - 8 - 4, TOP_PANEL_H - 16 - 16},
        soundFonts, 2, &sfIndex, &active, &focus);

    //draw instrument properties

    bounds.x += SOUNDFONT_LIST_W + 8;
    GuiGroupBox((Rectangle){bounds.x, bounds.y, INSTRUMENT_PROPS_W, TOP_PANEL_H - 16}, "Instrument Properties");

    bounds.y += 16;

#define BANK_COMBO_W    64
#define SF_COMBO_X      64
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
    GuiLabel((Rectangle){ bounds.x + 8, bounds.y, SF_COMBO_X - 4, 24 }, "SoundFont");
    int sf = 0;
    GuiComboBox((Rectangle){ bounds.x + 8 + SF_COMBO_X, bounds.y, INSTRUMENT_PROPS_W - 16 - SF_COMBO_X, 24 }, "None", &sf);

    bounds.y += 24 + 8;

    GuiLabel((Rectangle){ bounds.x + 8, bounds.y, SF_COMBO_X - 4, 24 }, "Bank");
    int bank = 0;
    GuiComboBox((Rectangle){ bounds.x + 8 + SF_COMBO_X, bounds.y, BANK_COMBO_W, 24 }, "---", &bank);

#define PATCH_COMBO_X   (8 + SF_COMBO_X + BANK_COMBO_W + 8 + 40)
    GuiLabel((Rectangle){ bounds.x + 8 + SF_COMBO_X + BANK_COMBO_W + 8, bounds.y, 36, 24 }, "Patch");
    int patch = 0;
    GuiComboBox((Rectangle){ bounds.x + PATCH_COMBO_X, bounds.y, INSTRUMENT_PROPS_W - PATCH_COMBO_X - 8, 24 }, "---", &patch);

    bounds.y += 24 + 8;
 
    GuiLabel((Rectangle){ bounds.x + 8, bounds.y, SF_COMBO_X - 4, 24 }, "Velocity");
    int velocity = 100;
    GuiSpinner((Rectangle){ bounds.x + 8 + SF_COMBO_X, bounds.y, 80, 24 }, "", &velocity, 1, 200, false);

    GuiLabel((Rectangle){ bounds.x + 8 + SF_COMBO_X + 80, bounds.y, 72 - 4, 24 }, "Transpose");
    int transpose = 0;
    GuiSpinner((Rectangle){ bounds.x + 8 + SF_COMBO_X + 80 + 72, bounds.y, 80, 24 }, "", &transpose, -48, 48, false);

    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
}

void DrawInstrumentList(Rectangle bounds)
{
    GuiGroupBox(bounds, "Instruments");
}

void DrawTabbedArea(Rectangle bounds, int tabButtons, const int* tabIcons, int* currentTab)
{
    const float tabsWidth   = tabButtons * (TOOL_BUTTON_S + 8) + 8;
    const float tabsLeft    = bounds.x + bounds.width - tabsWidth;

    GuiDrawRectangle((Rectangle){tabsLeft, bounds.y, tabsWidth, TOOLBAR_HEIGHT + 8}, RAYGUI_PANEL_BORDER_WIDTH,
        GetColor(GuiGetStyle(DEFAULT, (int)LINE_COLOR)),
        GetColor(GuiGetStyle(DEFAULT, (int)BACKGROUND_COLOR)));                
    GuiDrawRectangle((Rectangle){bounds.x, bounds.y + TOOL_BUTTON_S + 8, bounds.width, bounds.height - TOOL_BUTTON_S - 8}, RAYGUI_PANEL_BORDER_WIDTH,
        GetColor(GuiGetStyle(DEFAULT, (int)LINE_COLOR)),
        GetColor(GuiGetStyle(DEFAULT, (int)BACKGROUND_COLOR)));                
    GuiDrawRectangle((Rectangle){float(tabsLeft) + RAYGUI_PANEL_BORDER_WIDTH,
                                 bounds.y + RAYGUI_PANEL_BORDER_WIDTH,
                                 tabsWidth - 2 * RAYGUI_PANEL_BORDER_WIDTH,
                                 TOOL_BUTTON_S + 8 + RAYGUI_PANEL_BORDER_WIDTH}, 0,
        GetColor(GuiGetStyle(DEFAULT, (int)LINE_COLOR)),
        GetColor(GuiGetStyle(DEFAULT, (int)BACKGROUND_COLOR)));

    for (int tab = 0; tab < tabButtons; ++tab)
    {
        if (tab == *currentTab) GuiSetState(STATE_PRESSED);
        if (GuiButton((Rectangle){ tabsLeft + 8 + tab * (TOOL_BUTTON_S + 8), bounds.y + 8, TOOL_BUTTON_S, TOOLBAR_HEIGHT },
            GuiIconText(tabIcons[tab], "")))
        {
            *currentTab = tab;
        }
        GuiSetState(STATE_NORMAL);
    }
}

#define WIDE_CHAR       12
#define NARROW_CHAR     8
#define COLUMN_SPACING  1
#define LINE_HEIGHT     16

void DrawPattern(Rectangle bounds)
{
    int noteColumns[16], fxColumns[16];

    int totalWidth = NARROW_CHAR * 3 + COLUMN_SPACING * 2 + RAYGUI_PANEL_BORDER_WIDTH;

    sft::Module::P module = sft::Module::current();
    
    for (int track = 0; track < 16; ++track)
    {
        noteColumns[track]  = module->noteColumns(track);
        fxColumns[track]    = module->fxColumns(track);

        totalWidth += (WIDE_CHAR * 3 + NARROW_CHAR * 2 + COLUMN_SPACING * 2) * noteColumns[track] + COLUMN_SPACING;
        
        if (fxColumns[track])
        {
            totalWidth += (NARROW_CHAR * 4 + COLUMN_SPACING) * fxColumns[track];
        }
    }

    int order       = module->editingOrder();
    int line        = module->editingLine();
    int track       = module->editingTrack();
    int column      = module->editingColumn();
    int trackMask   = module->trackMask(order);
    
    sft::Pattern::P pattern = module->orderPattern(order);

    Rectangle content = (Rectangle){0, 0, float(totalWidth), bounds.height + pattern->length() * 8}, view;
    static Vector2 scroll;

    GuiScrollPanel(bounds, nullptr, content, &scroll, &view);

    if (-scroll.y / 8 != line)
    {
        line = std::min(pattern->length() - 1, int(-scroll.y / 8));
        module->setEditingLine(line, true);
    }

    //scissor
    int visibleHeight = int(bounds.height);

    int originY = int(bounds.y + bounds.height / 2) - line * LINE_HEIGHT + RAYGUI_PANEL_BORDER_WIDTH;

    for (int drawLine = 0; drawLine < pattern->length(); ++drawLine)
    {
        if (originY < bounds.y + bounds.height && originY + 16 > bounds.y)
        {
            int originX = int(bounds.x + scroll.x) + RAYGUI_PANEL_BORDER_WIDTH + COLUMN_SPACING;

            //line #
            for (int c = 0; c < 3; ++c)
            {
                GuiDrawRectangle(
                    (Rectangle){float(originX), float(originY), NARROW_CHAR - 1, LINE_HEIGHT - 1},
                    0,  //border width
                    GetColor(GuiGetStyle(DEFAULT, (int)LINE_COLOR)),
                    GetColor(GuiGetStyle(DEFAULT, (int)LINE_COLOR)));

                originX += NARROW_CHAR;
            }

            originX += COLUMN_SPACING;

            for (int track = 0; track < 16; ++track)
            {
                //notes

                for (int nc = 0; nc < noteColumns[track]; ++nc)
                {
                    //note

                    for (int c = 0; c < 3; ++c)
                    {
                        GuiDrawRectangle(
                            (Rectangle){float(originX), float(originY), WIDE_CHAR - 1, LINE_HEIGHT - 1},
                            0,  //border width
                            GetColor(GuiGetStyle(DEFAULT, (int)LINE_COLOR)),
                            GetColor(GuiGetStyle(DEFAULT, (int)LINE_COLOR)));

                        originX += WIDE_CHAR;
                    }

                    //velocity

                    for (int c = 0; c < 2; ++c)
                    {
                        GuiDrawRectangle(
                            (Rectangle){float(originX), float(originY), NARROW_CHAR - 1, LINE_HEIGHT - 1},
                            0,  //border width
                            GetColor(GuiGetStyle(DEFAULT, (int)LINE_COLOR)),
                            GetColor(GuiGetStyle(DEFAULT, (int)LINE_COLOR)));

                        originX += NARROW_CHAR;
                    }

                    originX += COLUMN_SPACING;

                    //instrument

                    for (int c = 0; c < 2; ++c)
                    {
                        GuiDrawRectangle(
                            (Rectangle){float(originX), float(originY), NARROW_CHAR - 1, LINE_HEIGHT - 1},
                            0,  //border width
                            GetColor(GuiGetStyle(DEFAULT, (int)LINE_COLOR)),
                            GetColor(GuiGetStyle(DEFAULT, (int)LINE_COLOR)));

                        originX += NARROW_CHAR;
                    }

                    originX += COLUMN_SPACING;
                }

                //fx
            }
        }
        
        originY += LINE_HEIGHT;
    }
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

    int currentTab = 0;
    int patternTab = 0;

    while (!WindowShouldClose())
    {
        screenWidth     = GetScreenWidth();
        screenHeight    = GetScreenHeight();

        int instListW   = screenWidth - TOP_PANEL_W - 24;

        BeginDrawing();
            ClearBackground(GetColor( GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            //Pattern Editor

            Rectangle editorBounds = { 8, 8 + TOP_PANEL_Y + TOP_PANEL_H, float(screenWidth - 16), float(screenHeight - TOP_PANEL_Y - TOP_PANEL_H - 16) };

            const int patternIcons[] = {ICON_FILETYPE_AUDIO, ICON_TOOLS};
            DrawTabbedArea(editorBounds, 2, patternIcons, &patternTab);

            //Pattern Editor Controls

            int currentPattern = 1;
            GuiSpinner((Rectangle){8 + 48, editorBounds.y + 8, 80, 24}, "Pattern ", &currentPattern, 1, 256, false);

            int patternLength = 64;
            GuiSpinner((Rectangle){8 + 48 + 80 + 48, editorBounds.y + 8, 80, 24}, "Length ", &patternLength, 1, 256, false);

            int editingStep = 1;
            GuiSpinner((Rectangle){8 + 48 + 80 + 48 + 80 + 40, editorBounds.y + 8, 80, 24}, "Step ", &editingStep, 1, 256, false);

            DrawPattern((Rectangle){16, editorBounds.y + 48, editorBounds.width - 16, editorBounds.height - 56});

            //Top Panel

            const int topIcons[] = {ICON_FILETYPE_AUDIO, ICON_FILETYPE_AUDIO, ICON_TOOLS, ICON_FX, ICON_GEAR_BIG, ICON_HELP_BOX};
            DrawTabbedArea((Rectangle){ 8, 8, TOP_PANEL_W, TOP_PANEL_H + TOP_PANEL_Y - 8}, 6, topIcons, &currentTab);

            Rectangle topPanelBounds = {16, TOP_PANEL_Y + 8, TOP_PANEL_W - 16, TOP_PANEL_H - 16};

            switch (currentTab)
            {
            case 0:
                DrawSongPanel(topPanelBounds);
                break;
            case 1:
                DrawInstrumentSettings(topPanelBounds);
                break;
            default:
                break;
            }

            DrawInstrumentList((Rectangle){TOP_PANEL_W + 16, 8, float(instListW), TOP_PANEL_H + TOP_PANEL_Y - 8});

            //Top Buttons
            
            //File Operations

            if (GuiButton((Rectangle){ 8, 8, TOOL_BUTTON_S, TOOLBAR_HEIGHT },
                GuiIconText(ICON_FILE_NEW, "")))
            {
                //New
            }
            if (GuiButton((Rectangle){ 8 + TOOL_BUTTON_S + 8, 8, TOOL_BUTTON_S, TOOLBAR_HEIGHT },
                GuiIconText(ICON_FILE_OPEN, "")))
            {
                //Open
            }
            if (GuiButton((Rectangle){ 8 + 2 * (TOOL_BUTTON_S + 8), 8, TOOL_BUTTON_S, TOOLBAR_HEIGHT },
                GuiIconText(ICON_FILE_SAVE_CLASSIC, "")))
            {
                //Save (as)
            }
            if (GuiButton((Rectangle){ 8 + 3 * (TOOL_BUTTON_S + 8), 8, TOOL_BUTTON_S, TOOLBAR_HEIGHT },
                GuiIconText(ICON_FILE_EXPORT, "")))
            {
                //Export MIDI
            }
            if (GuiButton((Rectangle){ 8 + 4 * (TOOL_BUTTON_S + 8), 8, TOOL_BUTTON_S, TOOLBAR_HEIGHT },
                GuiIconText(ICON_WAVE, "")))
            {
                //Render Audio
            }

            //Transport
#define TRANSPORT_X (TOP_PANEL_W / 2 + 8 - (TOOL_BUTTON_S * 6 + 8 * (5 + 2)) / 2)

            if (GuiButton((Rectangle){ TRANSPORT_X, 8, TOOL_BUTTON_S, TOOLBAR_HEIGHT },
                GuiIconText(ICON_PLAYER_PLAY, "")))
            {
                //Play
            }
            if (GuiButton((Rectangle){ float(TRANSPORT_X + TOOL_BUTTON_S + 8), 8, TOOL_BUTTON_S, TOOLBAR_HEIGHT },
                GuiIconText(ICON_REPEAT, "")))
            {
                //Loop Pattern Toggle
            }
            if (GuiButton((Rectangle){ float(TRANSPORT_X + 2 * (TOOL_BUTTON_S + 8)), 8, TOOL_BUTTON_S, TOOLBAR_HEIGHT },
                GuiIconText(ICON_PLAYER_STOP, "")))
            {
                //Stop
            }
            if (GuiButton((Rectangle){ float(TRANSPORT_X + 3 * (TOOL_BUTTON_S + 8)), 8, TOOL_BUTTON_S, TOOLBAR_HEIGHT },
                GuiIconText(ICON_PLAYER_RECORD, "")))
            {
                //Record/Edit Toggle
            }
            if (GuiButton((Rectangle){ float(TRANSPORT_X + 4 * (TOOL_BUTTON_S + 8)), 8, TOOL_BUTTON_S, TOOLBAR_HEIGHT },
                GuiIconText(ICON_MAGNET, "")))
            {
                //Follow Playback Toggle
            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}