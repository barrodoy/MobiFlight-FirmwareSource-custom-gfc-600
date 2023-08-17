#pragma once

#include "Arduino.h"
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define BLACK 0
#define WHITE 1

enum Messages {
    PFT = 0,
    ROL_MODE,
    LVL_MODE,
    NAV_MODE,
    HDG_MODE,
    HAS_LOC,
    BC_MODE,
    APP_MODE,
    GPS_SOURCE,
    NEEDLE,
    VS_MODE,
    VS_VALUE,
    IAS_MODE,
    IAS_VALUE,
    PIT_MODE,
    ALT_MODE,
    ALT_VALUE,
    ALTS_MODE,
    CURR_ALT,
    GS_ARM,
    GS_ACTIVE
};

enum LateralMode {
    NONE_LAT,
    ROL,
    HDG,
    LOC,
    VOR,
    GPS,
    LVL_LAT,
    BC,
    // Add more states as needed
};

enum VerticalMode {
    NONE_VERT,
    VS,
    ALT,
    ALTS,
    LVL_VERT,
    IAS,
    PIT,
    GS,
};

struct Position {
    int x;
    int y;
};

struct Label {
    const uint8_t *Font;
    const uint8_t  FontSize;
    const Position Pos;
};

struct Layout {
    Label ActiveLateralMode;  // Label for the active lateral mode display.
    Label ActiveVerticalMode; // Label for the active vertical mode display.
    Label Vertical2DigitsValue;
    Label Vertical3DigitsValue; // Label for the vertical value display with 3 digits.
    Label Vertical4DigitsValue; // Label for the vertical value display with 4 digits.
    Label Vertical5DigitsValue; // Label for the vertical value display with 4 digits.
    Label ValueUnits;           // Label for the units of the displayed value.
    Label ArmedLateralMode;     // Label for the armed lateral mode display.
    Label ArmedVerticalMode1;   // Label for the first armed vertical mode display.
    Label ArmedVerticalMode2;   // Label for the second armed vertical mode display.
    Label VsArrow;              // Label for the vertical speed arrow display.
};

struct Line {
    const char *name;
    Position    StartPos;
    Position    EndPos;
};

class GFC600
{
public:
    GFC600(uint8_t clk, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset);
    void begin();
    void attach();
    void detach();
    void set(uint8_t messageID, const char *setPoint);

private:
    // U8G2_SSD1322_NHD_256X64_F_4W_SW_SPI *_oledDisplay;
    U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI *_oledDisplay;
    bool                                 _initialised;
    uint8_t                              _clk, _data, _cs, _dc, _reset;
    bool                                 _hasChanged;
    char                                 activeFrequency[8]  = "123.456";
    char                                 standbyFrequency[8] = "123.456";

    void _update();
    void _stop();
    void _renderLabel(const char *text, Label label, Position offset, bool update = false);
    void drawLine(Line line);
    void renderLateralMode(Layout layout, LateralMode activeMode, LateralMode armedMode);
    void renderVerticalMode(Layout layout, VerticalMode activeMode, VerticalMode armedMode1, VerticalMode armedMode2);
    void renderSymbols(const char *arrow, Label label, Position offset, bool update);
    void handleVerticalSpeedMode(Layout layout);
    void handleAltMode(Layout layout);
    void handleIasMode(Layout layout);
    void clearArea(int start_x, int start_y, int end_x, int end_y);
    int  roundToClosestHundred(int value);
    void decideLateralModes();
    void decideVerticalModes();
    void flashAlts(Label label, Position offset, bool update);
};