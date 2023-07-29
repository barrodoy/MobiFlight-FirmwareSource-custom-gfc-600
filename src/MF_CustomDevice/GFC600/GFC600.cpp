#include "GFC600.h"
#include <U8g2lib.h>
#include "allocateMem.h"
#include "commandmessenger.h"

// Lateral modes definitions:
const int ACTIVE_LATERAL_MODE_X = 8;     // X-coordinate of the active lateral mode label on the display.
const int ACTIVE_LATERAL_MODE_Y = 17;    // Y-coordinate of the active lateral mode label on the display.

const int ARMED_LATERAL_MODE_X = 8;      // X-coordinate of the armed lateral mode label on the display.
const int ARMED_LATERAL_MODE_Y = 57;     // Y-coordinate of the armed lateral mode label on the display.

bool rol_mode_selected          = false; // Flag indicating whether the roll mode is selected.
bool lvl_mode_selected          = false; // Flag indicating whether the level mode is selected.
bool vor_mode_selected          = false; // Flag indicating whether the VOR mode is selected.
bool hdg_mode_selected          = false; // Flag indicating whether the heading mode is selected.
bool loc_mode_selected          = false; // Flag indicating whether the localizer mode is selected.
bool bc_mode_selected           = false; // Flag indicating whether the back-course mode is selected.
bool app_mode_selected          = false; // Flag indicating whether the approach mode is selected.
bool gps_source_selected        = false; // Flag indicating whether the GPS source is selected.
bool needle_not_fully_deflected = false; // Flag indicating whether the needle is not fully deflected.

// Vertical modes definitions:
const int ACTIVE_VERTICAL_MODE_X = 56;     // X-coordinate of the active vertical mode label on the display.
const int ACTIVE_VERTICAL_MODE_Y = 15;     // Y-coordinate of the active vertical mode label on the display.

const int ARMED_VERTICAL_MODE1_X = 56;     // X-coordinate of the first armed vertical mode label on the display.
const int ARMED_VERTICAL_MODE1_Y = 57;     // Y-coordinate of the first armed vertical mode label on the display.

const int VERTICAL_VALUE_3_DIGITS_X = 130; // X-coordinate of the vertical value label with 3 digits on the display.
const int VERTICAL_VALUE_3_DIGITS_Y = 20;  // Y-coordinate of the vertical value label with 3 digits on the display.

const int VERTICAL_VALUE_4_DIGITS_X = 126; // X-coordinate of the vertical value label with 4 digits on the display.
const int VERTICAL_VALUE_4_DIGITS_Y = 20;  // Y-coordinate of the vertical value label with 4 digits on the display.

const int VERTICAL_VALUE_UNITS_X = 142;    // X-coordinate of the vertical value units label on the display.
const int VERTICAL_VALUE_UNITS_Y = 35;     // Y-coordinate of the vertical value units label on the display.

const int VERTICAL_ARROW_X = 118;          // X-coordinate of the vertical speed arrow on the display.
const int VERTICAL_ARROW_Y = 18;           // Y-coordinate of the vertical speed arrow on the display.

const char *DOWN_ARROW = "↓";              // Symbol representing a downward arrow.
const char *UP_ARROW   = "↑";              // Symbol representing an upward arrow.
const int   ARROW_SIZE = 20;

const int MAX_3_DIGITS_VALUE = 999;  // Maximum value that can be displayed with 3 digits.
const int MAX_4_DIGITS_VALUE = 9999; // Maximum value that can be displayed with 4 digits.

bool vs_mode_selected = false;       // Flag indicating whether the vertical speed mode is selected.
int  vs_fpm_value     = 0;           // Value of the vertical speed in feet per minute.

bool ias_mode_selected = false;
int  ias_kts_value     = 0;

bool pit_mode_selected = false;

bool alt_mode_selected = false;
int  alt_ft_value      = 0;

// Common modes definitions:

const uint8_t *ACTIVE_MODE_FONT = u8g2_font_logisoso16_tr;  // Font used for the active mode labels.
const uint8_t *ARMED_MODE_FONT  = u8g2_font_profont12_mr;   // Font used for the armed mode labels.
const uint8_t *ARROW_FONT       = u8g2_font_9x15_m_symbols; // Font used for arrow symbols.

const char *CLEAR_STRING = "      ";                        // String representing empty spaces used for clearing the display.

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

// Define an instance of the Layout struct named DisplayLayout and initialize its members.
Layout DisplayLayout = {
    // ActiveLateralMode label settings: font ACTIVE_MODE_FONT, font size 16, position (ACTIVE_LATERAL_MODE_X, ACTIVE_LATERAL_MODE_Y).
    {ACTIVE_MODE_FONT, 16, {ACTIVE_LATERAL_MODE_X, ACTIVE_LATERAL_MODE_Y}},
    // ActiveVerticalMode label settings: font ACTIVE_MODE_FONT, font size 16, position (ACTIVE_VERTICAL_MODE_X, ACTIVE_LATERAL_MODE_Y).
    {ACTIVE_MODE_FONT, 16, {ACTIVE_VERTICAL_MODE_X, ACTIVE_LATERAL_MODE_Y}},
    {ACTIVE_MODE_FONT, 12, {VERTICAL_VALUE_3_DIGITS_X - 5, VERTICAL_VALUE_3_DIGITS_Y}},
    // Vertical3DigitsValue label settings: font ARMED_MODE_FONT, font size 12, position (VERTICAL_VALUE_3_DIGITS_X, VERTICAL_VALUE_3_DIGITS_Y).
    {ACTIVE_MODE_FONT, 12, {VERTICAL_VALUE_3_DIGITS_X, VERTICAL_VALUE_3_DIGITS_Y}},
    // Vertical4DigitsValue label settings: font ARMED_MODE_FONT, font size 12, position (VERTICAL_VALUE_4_DIGITS_X, VERTICAL_VALUE_4_DIGITS_Y).
    {ACTIVE_MODE_FONT, 12, {VERTICAL_VALUE_4_DIGITS_X, VERTICAL_VALUE_4_DIGITS_Y}},
    // ValueUnits label settings: font ARMED_MODE_FONT, font size 12, position (VERTICAL_VALUE_UNITS_X, VERTICAL_VALUE_UNITS_Y).
    {ARMED_MODE_FONT, 12, {VERTICAL_VALUE_UNITS_X, VERTICAL_VALUE_UNITS_Y}},
    // ArmedLateralMode label settings: font ARMED_MODE_FONT, font size 12, position (ARMED_LATERAL_MODE_X, ARMED_LATERAL_MODE_Y).
    {ARMED_MODE_FONT, 12, {ARMED_LATERAL_MODE_X, ARMED_LATERAL_MODE_Y}},
    // ArmedVerticalMode1 label settings: font ARMED_MODE_FONT, font size 12, position (ARMED_VERTICAL_MODE1_X, ARMED_VERTICAL_MODE1_Y).
    {ARMED_MODE_FONT, 12, {ARMED_VERTICAL_MODE1_X, ARMED_VERTICAL_MODE1_Y}},
    // ArmedVerticalMode2 label settings: font ARMED_MODE_FONT, font size 12, position (120, 18).
    {ARMED_MODE_FONT, 12, {120, 18}},
    // VsArrow label settings: font ARROW_FONT, font size 12, position (VERTICAL_ARROW_X, VERTICAL_ARROW_Y).
    {ARROW_FONT, ARROW_SIZE, {VERTICAL_ARROW_X, VERTICAL_ARROW_Y}}};

Position OffsetActive = {
    0,
    0};

Position OffsetArmed = {
    5,
    5};

Position Offset4Digits = {
    -6,
    0};

Position Offset3DigitsArrow{
    0, -5};

Position Offset4DigitsArrow{
    -8, 0};

Position OffsetZero{
    20, 0};

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
 * @brief Renders the lateral mode label based on the current mode states.
 * @param layout The layout to render the active lateral mode label on.
 */
void GFC600::renderLateralMode(Layout layout)
{
    // Check and render each active lateral mode based on the boolean variables

    // ROL mode is active (when all other later modes are off), display "ROL" label
    if (rol_mode_selected) {
        _renderLabel("ROL", layout.ActiveLateralMode, NoOffset, 0);
        _renderLabel(CLEAR_STRING, layout.ArmedLateralMode, NoOffset, 0);
    }

    // Heading mode is active, but navigation is off. Therefore, HDG mode is the active mode. Display "HDG" label
    else if (hdg_mode_selected && !vor_mode_selected) {
        _renderLabel("HDG", layout.ActiveLateralMode, NoOffset, 0);
        _renderLabel(CLEAR_STRING, layout.ArmedLateralMode, NoOffset, 0);
    }

    else if (hdg_mode_selected && vor_mode_selected && !needle_not_fully_deflected) {
        _renderLabel("HDG", layout.ActiveLateralMode, NoOffset, 0);
        _renderLabel("VOR", layout.ArmedLateralMode, NoOffset, 0);
    }

    // Both VOR navigation and GPS source are on. Display "GPS" label
    else if (vor_mode_selected && gps_source_selected) {
        _renderLabel("GPS", layout.ActiveLateralMode, NoOffset, 0);
        _renderLabel(CLEAR_STRING, layout.ArmedLateralMode, NoOffset, 0);
    }

    // VOR navigation is on, but GPS source is off. Display "VOR" label
    else if (vor_mode_selected && !gps_source_selected) {
        _renderLabel("VOR", layout.ActiveLateralMode, NoOffset, 0);
        _renderLabel(CLEAR_STRING, layout.ArmedLateralMode, NoOffset, 0);
    }

    // Display "LVL" label if LVL mode is active
    else if (lvl_mode_selected) {
        _renderLabel("LVL", layout.ActiveLateralMode, NoOffset, 0);
        _renderLabel(CLEAR_STRING, layout.ArmedLateralMode, NoOffset, 0);
    }

    // If none of the above conditions match, there is no active lateral mode. You can add a default label here if needed.

    // Add more conditions as per your specific requirements

    // If none of the above conditions match and no default label is set, no label will be rendered for the active lateral mode.
}

/**
 * Handle the display of the Vertical Speed mode on the OLED display.
 * This function updates the OLED display to show the current vertical speed information.
 *
 * @param layout The struct containing the layout configuration for the vertical speed mode.
 *              The struct should contain labels and positions to render the information.
 */
void GFC600::handleVerticalSpeedMode(Layout layout)
{
    // Render labels for the active and armed vertical modes, and units (e.g., "VS", "ALTS", "FPM").
    _renderLabel("VS", layout.ActiveVerticalMode, NoOffset, false);
    _renderLabel("ALTS", layout.ArmedVerticalMode1, NoOffset, false);
    _renderLabel("FPM", layout.ValueUnits, NoOffset, false);

    // Check if the vertical speed is zero.
    if (vs_fpm_value == 0) {
        // If the vertical speed is zero, display "0" without an arrow symbol.
        _oledDisplay->setDrawColor(BLACK);
        _oledDisplay->drawBox(VERTICAL_VALUE_3_DIGITS_X - 16, VERTICAL_VALUE_3_DIGITS_Y - 20, 36, 20);
        _renderLabel("0", layout.Vertical3DigitsValue, OffsetZero, false);
    }

    // Check if the vertical speed is positive.
    else if (vs_fpm_value > 0) {
        char vs_fpm_string[10];
        sprintf(vs_fpm_string, "%d", vs_fpm_value);

        // If the vertical speed is within the range of a 3-digit value, display it with an up arrow symbol.
        if (vs_fpm_value <= MAX_3_DIGITS_VALUE) {
            _oledDisplay->setDrawColor(BLACK);
            _oledDisplay->drawBox(VERTICAL_VALUE_3_DIGITS_X - 18, VERTICAL_VALUE_3_DIGITS_Y - 15, 12, 20);
            renderSymbols(UP_ARROW, layout.VsArrow, NoOffset, false);
            _renderLabel(vs_fpm_string, layout.Vertical3DigitsValue, NoOffset, false);
        }

        // If the vertical speed is larger than a 3-digit value, display it with a 4-digit layout.
        else {
            renderSymbols(UP_ARROW, layout.VsArrow, Offset4DigitsArrow, false);
            _renderLabel(vs_fpm_string, layout.Vertical4DigitsValue, Offset4Digits, false);
        }
    }

    // Vertical speed is negative.
    else {
        char vs_fpm_string[10];
        sprintf(vs_fpm_string, "%d", vs_fpm_value * (-1));

        // If the vertical speed is within the range of a 3-digit value, display it with a down arrow symbol.
        if (vs_fpm_value >= (-1) * MAX_3_DIGITS_VALUE) {
            _oledDisplay->setDrawColor(BLACK);
            _oledDisplay->drawBox(VERTICAL_VALUE_3_DIGITS_X - 18, VERTICAL_VALUE_3_DIGITS_Y - 15, 12, 20);
            renderSymbols(DOWN_ARROW, layout.VsArrow, NoOffset, false);
            _renderLabel(vs_fpm_string, layout.Vertical3DigitsValue, NoOffset, false);
        }

        // If the vertical speed is larger than a 3-digit value, display it with a 4-digit layout.
        else {
            renderSymbols(DOWN_ARROW, layout.VsArrow, Offset4DigitsArrow, false);
            _renderLabel(vs_fpm_string, layout.Vertical4DigitsValue, Offset4Digits, false);
        }
    }
}
void GFC600::renderVerticalMode(Layout layout)
{
    if (vs_mode_selected) {
        handleVerticalSpeedMode(layout);
    }

    else if (ias_mode_selected) {
        _renderLabel("IAS", layout.ActiveVerticalMode, NoOffset, false);
        _renderLabel("ALTS", layout.ArmedVerticalMode1, NoOffset, false);
        _renderLabel("KTS", layout.ValueUnits, NoOffset, false);

        char ias_kts_string[10];
        sprintf(ias_kts_string, "%d", vs_fpm_value);

        if (ias_kts_value <= 99) {
            _oledDisplay->setDrawColor(BLACK);
            _oledDisplay->drawBox(VERTICAL_VALUE_3_DIGITS_X - 12, VERTICAL_VALUE_3_DIGITS_Y - 15, 12, 20);
            _renderLabel(ias_kts_string, layout.Vertical2DigitsValue, NoOffset, false);
        }

        else {
            _renderLabel(ias_kts_string, layout.Vertical2DigitsValue, NoOffset, false);
        }
    }

    else if (pit_mode_selected) {
        _renderLabel("PIT", layout.ActiveVerticalMode, NoOffset, false);
        _renderLabel("ALTS", layout.ArmedVerticalMode1, NoOffset, false);
    }
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
        rol_mode_selected = atoi(data);
        cmdMessenger.sendCmd(kDebug, "ROL value: ");
        cmdMessenger.sendCmd(kDebug, rol_mode_selected);
        break;

        // Check if LVL is active
    case LVL_MODE:

        lvl_mode_selected = atoi(data);
        cmdMessenger.sendCmd(kDebug, "LVL value: ");
        cmdMessenger.sendCmd(kDebug, lvl_mode_selected);
        break;

        // Check if HDG is active
    case HDG_MODE:

        hdg_mode_selected = atoi(data);

        cmdMessenger.sendCmd(kDebug, "HDG value: ");
        cmdMessenger.sendCmd(kDebug, hdg_mode_selected);
        break;

        // Check if VOR is active
    case VOR_MODE:

        vor_mode_selected = atoi(data);
        cmdMessenger.sendCmd(kDebug, "VOR value: ");
        cmdMessenger.sendCmd(kDebug, vor_mode_selected);
        break;

        // Check if LOC is active
    case LOC_MODE:

        loc_mode_selected = atoi(data);
        cmdMessenger.sendCmd(kDebug, "LOC value: ");
        cmdMessenger.sendCmd(kDebug, loc_mode_selected);
        break;

        // Check if BC is active
    case BC_MODE:

        bc_mode_selected = atoi(data);
        cmdMessenger.sendCmd(kDebug, "BC value: ");
        cmdMessenger.sendCmd(kDebug, bc_mode_selected);
        break;

        // Check if APP is active
    case APP_MODE:

        app_mode_selected = atoi(data);
        cmdMessenger.sendCmd(kDebug, "APP value: ");
        cmdMessenger.sendCmd(kDebug, app_mode_selected);
        break;

        // Check if GPS Source is active
    case GPS_SOURCE:

        gps_source_selected = atoi(data);
        cmdMessenger.sendCmd(kDebug, "GPS value: ");
        cmdMessenger.sendCmd(kDebug, gps_source_selected);
        break;

        // Check if needle is not fully deflected
    case NEEDLE:

        needle_not_fully_deflected = atoi(data);
        cmdMessenger.sendCmd(kDebug, "Needle value: ");
        cmdMessenger.sendCmd(kDebug, needle_not_fully_deflected);
        break;

        // Check if VS mode is selected
    case VS_MODE:

        vs_mode_selected = atoi(data);
        cmdMessenger.sendCmd(kDebug, "VS Mode value: ");
        cmdMessenger.sendCmd(kDebug, vs_mode_selected);
        break;

    case VS_VALUE:
        // Implement the logic for setting Active VAPP Mode
        vs_fpm_value = atoi(data);
        cmdMessenger.sendCmd(kDebug, "VS FPM value: ");
        cmdMessenger.sendCmd(kDebug, vs_fpm_value);
        break;

    case IAS_MODE:
        // Implement the logic for setting Active VAPP Mode
        ias_mode_selected = atoi(data);
        cmdMessenger.sendCmd(kDebug, "IAS mode value: ");
        cmdMessenger.sendCmd(kDebug, ias_mode_selected);
        break;

    case IAS_VALUE:
        // Implement the logic for setting Active VAPP Mode
        ias_kts_value = atoi(data);
        cmdMessenger.sendCmd(kDebug, "IAS KIAS value: ");
        cmdMessenger.sendCmd(kDebug, ias_kts_value);
        break;

    case PIT_MODE:
        // Implement the logic for setting Active VAPP Mode
        pit_mode_selected = atoi(data);
        cmdMessenger.sendCmd(kDebug, "Pit Mode value: ");
        cmdMessenger.sendCmd(kDebug, pit_mode_selected);
        break;

    default:
        break;
    }

    renderLateralMode(DisplayLayout);
    renderVerticalMode(DisplayLayout);
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

void GFC600::renderSymbols(const char *arrow, Label label, Position offset, bool update)
{
    _oledDisplay->setFont(label.Font);
    u8g2_int_t w = _oledDisplay->getStrWidth(arrow);
    u8g2_int_t h = label.FontSize;
    _oledDisplay->setDrawColor(0);
    _oledDisplay->drawBox(offset.x + label.Pos.x, offset.y + label.Pos.y - h, w, h);
    _oledDisplay->setDrawColor(1);
    _oledDisplay->setFontMode(0);
    _oledDisplay->setCursor(offset.x + label.Pos.x, offset.y + label.Pos.y);

    _oledDisplay->drawUTF8(offset.x + label.Pos.x, offset.y + label.Pos.y, arrow);
    if (update) _oledDisplay->sendBuffer();
}
