#include "GFC600.h"
#include <U8g2lib.h>
#include "allocateMem.h"
#include "commandmessenger.h"

const int latModeX = 8;
const int latModeY = 17;

bool rol_mode_active = false;
bool lvl_mode_active = false;
bool vor_mode_active = false;
bool hdg_mode_active = false;
bool loc_mode_active = false;
bool bc_mode_active  = false;
bool app_mode_active = false;
bool gps_source      = false;

/*
struct Layout {
    Label Value;
    Label ValueLabel;
    Label Station;
    Label ModeComLabel;
    Label ModeNavLabel;
};
*/

Position NoOffset = {
    0,
    0};

Line LeftLine = {
    "Left",   // name
    {52, 11}, // StartPos
    {52, 57}  // EndPos
};

Line RightLine = {
    "Right",   // name
    {162, 11}, // StartPos
    {162, 57}  // EndPos
};

Layout DisplayLayout = {
    {u8g2_font_logisoso16_tr, 16, {latModeX, latModeY}},
    {u8g2_font_logisoso16_tr, 16, {56, 15}},
    {u8g2_font_profont10_mr, 10, {0, 32}},
    {u8g2_font_profont12_mr, 13, {18, 45}},
    {u8g2_font_profont12_mr, 12, {0, 54}},
    {u8g2_font_profont12_mr, 12, {0, 54}},
    {u8g2_font_profont12_mr, 12, {120, 18}},
};

Position OffsetActive = {
    0,
    0};

Position OffsetStandby = {
    140,
    0};

GFC600::GFC600(uint8_t clk, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset)
{
    _clk   = clk;
    _data  = data;
    _cs    = cs;
    _dc    = dc;
    _reset = reset;
}

void GFC600::attach()
{
    /* **********************************************************************************
        Next call the constructor of your custom device
        adapt it to the needs of your constructor
    ********************************************************************************** */
    if (!FitInMemory(sizeof(U8G2_SSD1322_NHD_256X64_F_4W_SW_SPI))) {
        // Error Message to Connector
        cmdMessenger.sendCmd(kStatus, F("Custom Device does not fit in Memory"));
        return;
    }

    //_oledDisplay = new (allocateMemory(sizeof(U8G2_SSD1322_NHD_256X64_F_4W_SW_SPI))) U8G2_SSD1322_NHD_256X64_F_4W_SW_SPI(U8G2_R0, _clk, _data, _cs, _dc);
    _oledDisplay = new (allocateMemory(sizeof(U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI))) U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI(U8G2_R2, 10, 9, 8);
    begin();
}

void GFC600::begin()
{
    _oledDisplay->begin();
    _oledDisplay->clearBuffer();
    //_update();
    drawLine(LeftLine);
    drawLine(RightLine);
    _oledDisplay->sendBuffer();
}

void GFC600::detach()
{
    if (!_initialised)
        return;
    _initialised = false;
}

void GFC600::_stop()
{
    _oledDisplay->clearBuffer();
    _oledDisplay->sendBuffer();
}

/**
 * @brief Renders the active lateral mode label based on the current mode states.
 * @param layout The layout to render the active lateral mode label on.
 */
void GFC600::renderActiveLateralMode(Layout layout)
{
    // Check and render each active lateral mode based on the boolean variables

    // ROL mode is active, display "ROL" label
    if (rol_mode_active) {
        _renderLabel("ROL", layout.ActiveLateralMode, NoOffset, 0);
    }

    // Both VOR navigation and GPS source are on. Display "GPS" label
    else if (vor_mode_active && gps_source) {
        _renderLabel("GPS", layout.ActiveLateralMode, NoOffset, 0);
    }

    // VOR navigation is on, but GPS source is off. Display "VOR" label
    else if (vor_mode_active && !gps_source) {
        _renderLabel("VOR", layout.ActiveLateralMode, NoOffset, 0);
    }

    // Display "LVL" label if LVL mode is active
    else if (lvl_mode_active) {
        _renderLabel("LVL", layout.ActiveLateralMode, NoOffset, 0);
    }

    // Heading mode is active, but navigation is off. Therefore, HDG mode is the active mode. Display "HDG" label
    else if (hdg_mode_active && !vor_mode_active) {
        _renderLabel("HDG", layout.ActiveLateralMode, NoOffset, 0);
    }

    // If none of the above conditions match, there is no active lateral mode. You can add a default label here if needed.

    // Add more conditions as per your specific requirements

    // If none of the above conditions match and no default label is set, no label will be rendered for the active lateral mode.
}

void GFC600::set(uint8_t messageID, const char *data)
{
    /* **********************************************************************************
        Each messageID has it's own value
        check for the messageID and define what to do:
    ********************************************************************************** */
    // do something according your messageID
    switch (messageID) {
    case 0:
        _stop();
        break;

        // Check if ROL is active
    case ROL_MODE:
        rol_mode_active = atoi(data);
        cmdMessenger.sendCmd(kDebug, "ROL value: ");
        cmdMessenger.sendCmd(kDebug, rol_mode_active);
        break;

        // Check if LVL is active
    case LVL_MODE:
        // Implement the logic for setting Active LVL Mode
        lvl_mode_active = atoi(data);
        cmdMessenger.sendCmd(kDebug, "LVL value: ");
        cmdMessenger.sendCmd(kDebug, lvl_mode_active);
        break;

        // Check if HDG is active
    case HDG_MODE:
        // Implement the logic for setting Active HDG Mode
        hdg_mode_active = atoi(data);

        cmdMessenger.sendCmd(kDebug, "HDG value: ");
        cmdMessenger.sendCmd(kDebug, hdg_mode_active);
        break;

        // Check if VOR is active
    case VOR_MODE:
        // Implement the logic for setting Active VOR Mode
        vor_mode_active = atoi(data);
        cmdMessenger.sendCmd(kDebug, "VOR value: ");
        cmdMessenger.sendCmd(kDebug, vor_mode_active);
        break;

        // Check if LOC is active
    case LOC_MODE:
        // Implement the logic for setting Active LOC Mode
        loc_mode_active = atoi(data);
        cmdMessenger.sendCmd(kDebug, "LOC value: ");
        cmdMessenger.sendCmd(kDebug, loc_mode_active);
        break;

        // Check if BC is active
    case BC_MODE:
        // Implement the logic for setting Active BC Mode
        bc_mode_active = atoi(data);
        cmdMessenger.sendCmd(kDebug, "BC value: ");
        cmdMessenger.sendCmd(kDebug, bc_mode_active);
        break;

        // Check if APP is active
    case APP_MODE:
        // Implement the logic for setting Active VAPP Mode
        app_mode_active = atoi(data);
        cmdMessenger.sendCmd(kDebug, "APP value: ");
        cmdMessenger.sendCmd(kDebug, app_mode_active);
        break;

        // Check if GPS Source is active
    case GPS_SOURCE:
        // Implement the logic for setting Active VAPP Mode
        gps_source = atoi(data);
        cmdMessenger.sendCmd(kDebug, "GPS value: ");
        cmdMessenger.sendCmd(kDebug, gps_source);
        break;

    default:
        break;
    }

    renderActiveLateralMode(DisplayLayout);
    _oledDisplay->sendBuffer();
}

void GFC600::drawLine(Line line)
{
    _oledDisplay->drawLine(line.StartPos.x, line.StartPos.y, line.EndPos.x, line.EndPos.y);
}

void GFC600::_renderLabel(const char *text, Label label, Position offset, bool update)
{
    _oledDisplay->setFont(label.Font);
    u8g2_int_t w = _oledDisplay->getStrWidth(text);
    u8g2_int_t h = label.FontSize;
    _oledDisplay->setDrawColor(0);
    _oledDisplay->drawBox(offset.x + label.Pos.x, offset.y + label.Pos.y - h, w, h);
    _oledDisplay->setDrawColor(1);
    _oledDisplay->setFontMode(0);
    _oledDisplay->setCursor(offset.x + label.Pos.x, offset.y + label.Pos.y);

    _oledDisplay->print(text);
    if (update) _oledDisplay->sendBuffer();
}
