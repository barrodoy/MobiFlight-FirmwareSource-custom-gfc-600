#pragma once

#include "Arduino.h"
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define ROL_MODE   1
#define LVL_MODE   2
#define VOR_MODE   3
#define HDG_MODE   4
#define LOC_MODE   5
#define BC_MODE    6
#define APP_MODE   7
#define GPS_SOURCE 8
#define NEEDLE     9
#define VS_MODE    10
#define VS_VALUE 11

struct Position {
    uint8_t x;
    uint8_t y;
};

struct Label {
    const uint8_t *Font;
    const uint8_t  FontSize;
    const Position Pos;
};

struct Layout {
    Label ActiveLateralMode;
    Label ActiveVerticalMode;
    Label VerticalModeValue;
    Label ValueUnits;
    Label ArmedLateralMode;
    Label ArmedVerticalMode1;
    Label ArmedVerticalMode2;
    Label VsArrow;
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
    void renderLateralMode(Layout layout);
    void renderVerticalMode(Layout layout);
};