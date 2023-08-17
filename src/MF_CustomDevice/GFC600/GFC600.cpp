#include "GFC600.h"
#include <U8g2lib.h>
#include "allocateMem.h"
#include "commandmessenger.h"

// Lateral modes definitions:
const int ACTIVE_LATERAL_MODE_X = 8;  // X-coordinate of the active lateral mode label on the display.
const int ACTIVE_LATERAL_MODE_Y = 20; // Y-coordinate of the active lateral mode label on the display.

const int ARMED_LATERAL_MODE_X = 8;  // X-coordinate of the armed lateral mode label on the display.
const int ARMED_LATERAL_MODE_Y = 57; // Y-coordinate of the armed lateral mode label on the display.

const int SPACING_FROM_LINES = 4; // Spacing between different display elements.

const int DISPLAY_START_X = 0;   // X-coordinate of the display start.
const int DISPLAY_START_Y = 0;   // Y-coordinate of the display start.
const int DISPLAY_END_X   = 256; // X-coordinate of the display end.
const int DISPLAY_END_Y   = 64;  // Y-coordinate of the display end.

// Lateral modes definitions
bool rol_mode_selected          = false; // Flag indicating whether the roll mode is selected.
bool lvl_mode_selected          = false; // Flag indicating whether the level mode is selected.
bool nav_mode_selected          = false; // Flag indicating whether the NAV mode is selected.
bool hdg_mode_selected          = false; // Flag indicating whether the heading mode is selected.
bool nav_has_loc                = false; // Flag indicating whether the localizer mode is selected.
bool bc_mode_selected           = false; // Flag indicating whether the back-course mode is selected.
bool app_mode_selected          = false; // Flag indicating whether the approach mode is selected.
bool gps_source_selected        = false; // Flag indicating whether the GPS source is selected.
bool needle_not_fully_deflected = false; // Flag indicating whether the needle is not fully deflected.

// Vertical modes definitions:
const int ACTIVE_VERTICAL_MODE_X = 56; // X-coordinate of the active vertical mode label.
const int ACTIVE_VERTICAL_MODE_Y = 20; // Y-coordinate of the active vertical mode label.

const int ARMED_VERTICAL_MODE1_X = 56; // X-coordinate of the first armed vertical mode label.
const int ARMED_VERTICAL_MODE1_Y = 57; // Y-coordinate of the first armed vertical mode label.

// Coordinates for the vertical value label with 3 digits.
const int VERTICAL_VALUE_3_DIGITS_X = 130; // X-coordinate on the GUI.
const int VERTICAL_VALUE_3_DIGITS_Y = 20;  // Y-coordinate on the GUI.

// Coordinates for the vertical value label with 4 digits.
const int VERTICAL_VALUE_4_DIGITS_X = VERTICAL_VALUE_3_DIGITS_X - 4; // Shifted X-coordinate from the 3-digit label.
const int VERTICAL_VALUE_4_DIGITS_Y = VERTICAL_VALUE_3_DIGITS_Y;     // Same Y-coordinate as the 3-digit label.

// Coordinates for the vertical value label with 5 digits.
const int VERTICAL_VALUE_5_DIGITS_X = VERTICAL_VALUE_4_DIGITS_X - 4; // Shifted X-coordinate from the 4-digit label.
const int VERTICAL_VALUE_5_DIGITS_Y = VERTICAL_VALUE_4_DIGITS_Y;     // Same Y-coordinate as the 4-digit label.

const int VERTICAL_VALUE_X = 110;
const int VERTICAL_VALUE_Y = 20;

// Coordinates for the vertical value label with 3 digits.
const int VERTICAL_VALUE_2_DIGITS_X = VERTICAL_VALUE_3_DIGITS_X + 4; // X-coordinate on the GUI.
const int VERTICAL_VALUE_2_DIGITS_Y = 20;                            // Y-coordinate on the GUI.

const int VERTICAL_VALUE_UNITS_X = 142; // X-coordinate of the vertical value units label.
const int VERTICAL_VALUE_UNITS_Y = 35;  // Y-coordinate of the vertical value units label.

const int VERTICAL_ARROW_X = 118; // X-coordinate of the vertical speed arrow.
const int VERTICAL_ARROW_Y = 18;  // Y-coordinate of the vertical speed arrow.

const char *DOWN_ARROW = "↓"; // Symbol for a downward arrow.
const char *UP_ARROW   = "↑"; // Symbol for an upward arrow.
const int   ARROW_SIZE = 20;  // Size of the arrow symbol.

const int MAX_2_DIGITS_VALUE = 99;
const int MAX_3_DIGITS_VALUE = 999;  // Maximum value displayed with 3 digits.
const int MAX_4_DIGITS_VALUE = 9999; // Maximum value displayed with 4 digits.

// Vertical Modes definitions
bool vs_mode_selected = false; // Flag indicating whether the vertical speed mode is selected.
int  vs_fpm_value     = 0;     // Value of the vertical speed in feet per minute.

bool ias_mode_selected = false; // Flag indicating whether the indicated airspeed mode is selected.
int  ias_kts_value     = 0;     // Value of the indicated airspeed in knots.

bool pit_mode_selected = false; // Flag indicating whether the pitch mode is selected.

bool alt_mode_selected = false; // Flag indicating whether the altitude mode is selected.
int  alt_ft_value      = 0;     // Value of the altitude in feet.
int  curr_alt          = 0;     // Current altitude value.

bool gs_mode_armed  = false;
bool gs_mode_active = false;

bool alts_mode_armed  = false; // Flag indicating whether the altitude mode is selected.
bool alts_mode_active = false;

// Common modes definitions:

const uint8_t *ACTIVE_MODE_FONT = u8g2_font_logisoso16_tr;  // Font used for the active mode labels.
const uint8_t *ARMED_MODE_FONT  = u8g2_font_profont12_mr;   // Font used for the armed mode labels.
const uint8_t *ARROW_FONT       = u8g2_font_9x15_m_symbols; // Font used for arrow symbols.

const char *CLEAR_STRING = "   "; // String representing empty spaces used for clearing the display.

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
    // ActiveLateralMode label settings: font ACTIVE_mode_FONT, font size 16, position (ACTIVE_LATERAL_mode_X, ACTIVE_LATERAL_mode_Y).
    {ACTIVE_MODE_FONT, 16, {ACTIVE_LATERAL_MODE_X, ACTIVE_LATERAL_MODE_Y}},
    // ActiveVerticalMode label settings: font ACTIVE_mode_FONT, font size 16, position (ACTIVE_VERTICAL_mode_X, ACTIVE_LATERAL_mode_Y).
    {ACTIVE_MODE_FONT, 16, {ACTIVE_VERTICAL_MODE_X, ACTIVE_VERTICAL_MODE_Y}},
    {ACTIVE_MODE_FONT, 12, {VERTICAL_VALUE_3_DIGITS_X - 5, VERTICAL_VALUE_3_DIGITS_Y}},
    // Vertical3DigitsValue label settings: font ARMED_mode_FONT, font size 12, position (VERTICAL_VALUE_3_DIGITS_X, VERTICAL_VALUE_3_DIGITS_Y).
    {ACTIVE_MODE_FONT, 12, {VERTICAL_VALUE_3_DIGITS_X, VERTICAL_VALUE_3_DIGITS_Y}},
    // Vertical4DigitsValue label settings: font ARMED_mode_FONT, font size 12, position (VERTICAL_VALUE_4_DIGITS_X, VERTICAL_VALUE_4_DIGITS_Y).
    {ACTIVE_MODE_FONT, 12, {VERTICAL_VALUE_4_DIGITS_X, VERTICAL_VALUE_4_DIGITS_Y}},
    // Vertical5DigitsValue label settings: font ARMED_mode_FONT, font size 12, position (VERTICAL_VALUE_4_DIGITS_X, VERTICAL_VALUE_4_DIGITS_Y).
    {ACTIVE_MODE_FONT, 16, {VERTICAL_VALUE_X, VERTICAL_VALUE_Y}},
    // ValueUnits label settings: font ARMED_mode_FONT, font size 12, position (VERTICAL_VALUE_UNITS_X, VERTICAL_VALUE_UNITS_Y).
    {ARMED_MODE_FONT, 12, {VERTICAL_VALUE_UNITS_X, VERTICAL_VALUE_UNITS_Y}},
    // ArmedLateralMode label settings: font ARMED_mode_FONT, font size 12, position (ARMED_LATERAL_mode_X, ARMED_LATERAL_mode_Y).
    {ARMED_MODE_FONT, 12, {ARMED_LATERAL_MODE_X, ARMED_LATERAL_MODE_Y}},
    // ArmedVerticalMode1 label settings: font ARMED_mode_FONT, font size 12, position (ARMED_VERTICAL_MODE1_X, ARMED_VERTICAL_MODE1_Y).
    {ARMED_MODE_FONT, 12, {ARMED_VERTICAL_MODE1_X, ARMED_VERTICAL_MODE1_Y}},
    // ArmedVerticalMode2 label settings: font ARMED_mode_FONT, font size 12, position (120, 18).
    {ARMED_MODE_FONT, 12, {146, 57}},
    // VsArrow label settings: font ARROW_FONT, font size 12, position (VERTICAL_ARROW_X, VERTICAL_ARROW_Y).
    {ARROW_FONT, ARROW_SIZE, {VERTICAL_ARROW_X, VERTICAL_ARROW_Y}}};

Position OffsetActive = {
    0,
    0};

Position OffsetArmed = {
    5,
    5};

Position Offset2Digits = {
    30,
    0};

Position Offset3Digits = {
    20,
    0};

Position Offset4Digits = {
    10,
    0};

Position Offset5Digits = {
    0,
    0};

Position Offset3DigitsArrow{
    0, -5};

Position Offset4DigitsArrow{
    -8, 0};

Position OffsetZero{
    40, 0};

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

/*
 * @brief Decides the active and armed lateral modes based on the current mode selections.
 *        Then, renders the lateral mode labels using the determined modes.
 */
void GFC600::decideLateralModes()
{
    LateralMode activeMode = NONE_LAT;
    LateralMode armedMode  = NONE_LAT;

    if (rol_mode_selected) {
        activeMode = ROL;
        armedMode  = NONE_LAT;
    }

    // If heading mode selected
    else if (hdg_mode_selected) {

        // if no nav mode selected, Heading is active and none is armed
        if (!nav_mode_selected) {
            activeMode = HDG;
            armedMode  = NONE_LAT;
        }

        // If nav mode selected
        else if (nav_mode_selected) {

            // If GPS nav source is selected, Heading is Active, GPS is armed
            if (gps_source_selected) {
                activeMode = HDG;
                armedMode  = GPS;
            }

            // If GPS nav source is not selected
            else if (!gps_source_selected) {

                // If the current NAVAID has Localizer, Heading is active, LOC is armed
                if (nav_has_loc) {
                    activeMode = HDG;
                    armedMode  = LOC;
                }

                // If the current NAVAID does not have Localizer, Heading is active, VOR is armed
                else if (!nav_has_loc) {
                    activeMode = HDG;
                    armedMode  = VOR;
                }
            }
        }

    }

    else if (nav_mode_selected) {

        if (gps_source_selected) {
            activeMode = GPS;
            armedMode  = NONE_LAT;
        }

        else if (!gps_source_selected && !nav_has_loc) {
            activeMode = VOR;
            armedMode  = NONE_LAT;
        }

        else if (!gps_source_selected && nav_has_loc) {
            activeMode = LOC;
            armedMode  = NONE_LAT;
        }
    }

    else if (app_mode_selected) {

        if (nav_has_loc) {
            activeMode = LOC;
            armedMode  = NONE_LAT;
        }
    }

    else if (bc_mode_selected) {
        activeMode = BC;
        armedMode  = NONE_LAT;
    }

    else if (lvl_mode_selected) {
        activeMode = LVL_LAT;
        armedMode  = NONE_LAT;
    }

    renderLateralMode(DisplayLayout, activeMode, armedMode);
}

/**
 * @brief Renders the lateral mode labels based on the provided active and armed modes.
 * @param layout The layout to render the labels on.
 * @param activeMode The active lateral mode to render labels for.
 * @param armedMode The armed lateral mode to render labels for.
 */
void GFC600::renderLateralMode(Layout layout, LateralMode activeMode, LateralMode armedMode)
{

    clearArea(DISPLAY_START_X, DISPLAY_START_Y, LeftLine.StartPos.x - 2, DISPLAY_END_Y);
    // Render the active labels based on the FSM Modes
    switch (activeMode) {
    case ROL:
        _renderLabel("ROL", layout.ActiveLateralMode, NoOffset, 0);
        break;
    case HDG:
        _renderLabel("HDG", layout.ActiveLateralMode, NoOffset, 0);
        break;
    case LOC:
        _renderLabel("LOC", layout.ActiveLateralMode, NoOffset, 0);
        break;
    case VOR:
        _renderLabel("VOR", layout.ActiveLateralMode, NoOffset, 0);
        break;
    case GPS:
        _renderLabel("GPS", layout.ActiveLateralMode, NoOffset, 0);
        break;
    case LVL_LAT:
        _renderLabel("LVL", layout.ActiveLateralMode, NoOffset, 0);
        break;
    case BC:
        _renderLabel("BC", layout.ActiveLateralMode, NoOffset, 0);
        break;
    // ... Add more cases for other modes ...

    // Handle the default case if needed
    default:
        // Render a default label or leave it empty
        _renderLabel(CLEAR_STRING, layout.ActiveLateralMode, NoOffset, 0);
        break;
    }

    // Render the armed labels based on the armedMode
    switch (armedMode) {
    case VOR:
        _renderLabel("VOR", layout.ArmedLateralMode, NoOffset, 0);
        break;
    case GPS:
        _renderLabel("GPS", layout.ArmedLateralMode, NoOffset, 0);
        break;
    case LOC:
        _renderLabel("LOC", layout.ArmedLateralMode, NoOffset, 0);
        break;
        // ... Add more cases for other armed modes ...

    default:
        // Render a default label or leave it empty
        _renderLabel(CLEAR_STRING, layout.ArmedLateralMode, NoOffset, 0);
        break;
    }
}

/**
 * @brief Renders the lateral mode label based on the current mode Modes.
 * @param layout The layout to render the active lateral mode label on.
 */
/*
void GFC600::renderLateralMode(Layout layout)
{
    // Check and render each active lateral mode based on the boolean variables

    // ROL mode is active (when all other later modes are off), display "ROL" label
    if (rol_mode_selected) {
        _renderLabel("ROL", layout.ActiveLateralMode, NoOffset, 0);
        _renderLabel(CLEAR_STRING, layout.ArmedLateralMode, NoOffset, 0);
    }

    // Heading mode is active, but navigation is off. Therefore, HDG mode is the active mode. Display "HDG" label
    else if (hdg_mode_selected && !nav_mode_selected) {
        _renderLabel("HDG", layout.ActiveLateralMode, NoOffset, 0);
        _renderLabel(CLEAR_STRING, layout.ArmedLateralMode, NoOffset, 0);
    }

    // Heading is active, nav is armed and gps source is off
    else if (hdg_mode_selected && nav_mode_selected && !gps_source_selected) {
        _renderLabel("HDG", layout.ActiveLateralMode, NoOffset, 0);
        _renderLabel("VOR", layout.ArmedLateralMode, NoOffset, 0);
    }

    // Heading is active, nav is armed and gps source is on
    else if (hdg_mode_selected && nav_mode_selected && gps_source_selected) {
        _renderLabel("HDG", layout.ActiveLateralMode, NoOffset, 0);
        _renderLabel("GPS", layout.ArmedLateralMode, NoOffset, 0);
    }

    // Only heading mode is selected, no armed mode
    else if (hdg_mode_selected) {
        _renderLabel("HDG", layout.ActiveLateralMode, NoOffset, 0);
    }

    // Both VOR navigation and GPS source are on. Display "GPS" label
    else if (nav_mode_selected && gps_source_selected) {
        _renderLabel("GPS", layout.ActiveLateralMode, NoOffset, 0);
        _renderLabel(CLEAR_STRING, layout.ArmedLateralMode, NoOffset, 0);
    }

    // VOR navigation is on, but GPS source is off. Display "VOR" label
    else if (nav_mode_selected && !gps_source_selected && !nav_has_loc) {
        _renderLabel("VOR", layout.ActiveLateralMode, NoOffset, 0);
        _renderLabel(CLEAR_STRING, layout.ArmedLateralMode, NoOffset, 0);
    }

    // Nav is ON, no GPS Source and LOC is available
    else if (nav_mode_selected && !gps_source_selected && nav_has_loc) {
        _renderLabel("LOC", layout.ActiveLateralMode, NoOffset, 0);
        _renderLabel(CLEAR_STRING, layout.ArmedLateralMode, NoOffset, 0);
    }

    // Display "LVL" label if LVL mode is active
    else if (lvl_mode_selected) {
        _renderLabel("LVL", layout.ActiveLateralMode, NoOffset, 0);
        _renderLabel(CLEAR_STRING, layout.ArmedLateralMode, NoOffset, 0);
    }

    if (app_mode_selected && (hdg_mode_selected || rol_mode_selected)) {
        _renderLabel("LOC", layout.ArmedLateralMode, NoOffset, 0);
    }

    // If none of the above conditions match, there is no active lateral mode. You can add a default label here if needed.

    // Add more conditions as per your specific requirements

    // If none of the above conditions match and no default label is set, no label will be rendered for the active lateral mode.
}
*/

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
        _renderLabel("0", layout.Vertical5DigitsValue, OffsetZero, false);
    }

    // Check if the vertical speed is positive.
    else if (vs_fpm_value > 0) {
        char vs_fpm_string[10];
        sprintf(vs_fpm_string, "%d", vs_fpm_value);

        // If the vertical speed is within the range of a 3-digit value, display it with an up arrow symbol.
        if (vs_fpm_value <= MAX_3_DIGITS_VALUE) {
            _oledDisplay->setDrawColor(BLACK);
            _oledDisplay->drawBox(VERTICAL_VALUE_3_DIGITS_X - 24, VERTICAL_VALUE_3_DIGITS_Y - 15, 18, 20);
            renderSymbols(UP_ARROW, layout.VsArrow, NoOffset, false);
            _renderLabel(vs_fpm_string, layout.Vertical5DigitsValue, Offset3Digits, false);
        }

        // If the vertical speed is larger than a 3-digit value, display it with a 4-digit layout.
        else {
            renderSymbols(UP_ARROW, layout.VsArrow, Offset4DigitsArrow, false);
            _renderLabel(vs_fpm_string, layout.Vertical5DigitsValue, Offset4Digits, false);
        }
    }

    // Vertical speed is negative.
    else {
        char vs_fpm_string[10];
        sprintf(vs_fpm_string, "%d", vs_fpm_value * (-1));

        // If the vertical speed is within the range of a 3-digit value, display it with a down arrow symbol.
        if (vs_fpm_value >= (-1) * MAX_3_DIGITS_VALUE) {
            _oledDisplay->setDrawColor(BLACK);
            _oledDisplay->drawBox(VERTICAL_VALUE_3_DIGITS_X - 24, VERTICAL_VALUE_3_DIGITS_Y - 15, 18, 20);
            renderSymbols(DOWN_ARROW, layout.VsArrow, NoOffset, false);
            _renderLabel(vs_fpm_string, layout.Vertical5DigitsValue, Offset3Digits, false);
        }

        // If the vertical speed is larger than a 3-digit value, display it with a 4-digit layout.
        else {
            renderSymbols(DOWN_ARROW, layout.VsArrow, Offset4DigitsArrow, false);
            _renderLabel(vs_fpm_string, layout.Vertical5DigitsValue, Offset4Digits, false);
        }
    }
}

/**
 * Handles the IAS (Indicated Airspeed) mode by rendering labels and values based on the provided layout.
 *
 * @param layout The layout configuration for displaying labels.
 */
void GFC600::handleIasMode(Layout layout)
{
    // Render labels for active and armed vertical modes, and units (e.g., "IAS", "ALTS", "KTS").
    _renderLabel("IAS", layout.ActiveVerticalMode, NoOffset, false);
    _renderLabel("ALTS", layout.ArmedVerticalMode1, NoOffset, false);
    _renderLabel("KTS", layout.ValueUnits, NoOffset, false);

    // Check if the indicated airspeed (IAS) is zero.
    if (ias_kts_value == 0) {
        // Display "0" without an arrow symbol.
        _renderLabel("0", layout.Vertical5DigitsValue, OffsetZero, false);
    }

    // Check if the indicated airspeed (IAS) is positive.
    else if (ias_kts_value > 0) {
        char ias_kts_string[10];
        sprintf(ias_kts_string, "%d", ias_kts_value);

        // If IAS is within 2 digits, display it with an up arrow symbol.
        if (ias_kts_value <= MAX_2_DIGITS_VALUE) {
            _renderLabel(ias_kts_string, layout.Vertical5DigitsValue, Offset2Digits, false);
        }

        // If IAS is larger than 2 digits, display it using 3 digits layout.
        else {
            _renderLabel(ias_kts_string, layout.Vertical5DigitsValue, Offset3Digits, false);
        }
    }
}

/**
 * Handles the ALT (Altitude) mode by rendering labels and values based on the provided layout.
 *
 * @param layout The layout configuration for displaying labels.
 */
void GFC600::handleAltMode(Layout layout)
{
    // Render labels for the active vertical mode and units (e.g., "ALT", "FT").
    _renderLabel("ALT", layout.ActiveVerticalMode, NoOffset, false);
    _renderLabel("FT", layout.ValueUnits, NoOffset, false);

    // Check if the current altitude is zero.
    if (curr_alt == 0) {
        // Display "0" without an arrow symbol.
        _oledDisplay->setDrawColor(BLACK);
        _oledDisplay->drawBox(VERTICAL_VALUE_3_DIGITS_X - 16, VERTICAL_VALUE_3_DIGITS_Y - 20, 36, 20);
        _renderLabel("0", layout.Vertical5DigitsValue, OffsetZero, false);
    }

    // Check if the current altitude is positive.
    else if (curr_alt > 0) {
        char curr_alt_string[10];
        sprintf(curr_alt_string, "%d", curr_alt); // Rounding to closest hundred.

        // Display altitude value and handle different digit ranges.
        if (curr_alt <= MAX_3_DIGITS_VALUE) {
            _oledDisplay->setDrawColor(BLACK);
            _oledDisplay->drawBox(VERTICAL_VALUE_3_DIGITS_X - 24, VERTICAL_VALUE_3_DIGITS_Y - 15, 18, 20);
            _renderLabel(curr_alt_string, layout.Vertical5DigitsValue, Offset3Digits, false);
        } else if (curr_alt <= MAX_4_DIGITS_VALUE) {
            _renderLabel(curr_alt_string, layout.Vertical5DigitsValue, Offset4Digits, false);
        } else if (curr_alt > MAX_4_DIGITS_VALUE) {
            _renderLabel(curr_alt_string, layout.Vertical5DigitsValue, Offset5Digits, false);
        }
    }
}

void GFC600::clearArea(int start_x, int start_y, int end_x, int end_y)
{
    _oledDisplay->setDrawColor(BLACK);
    _oledDisplay->drawBox(start_x, start_y, end_x, end_y);
}

/*
 * @brief Decides the active and armed vertical modes based on the current mode selections.
 *        Then, renders the vertical mode labels using the determined modes.
 */
void GFC600::decideVerticalModes()
{
    VerticalMode activeMode = NONE_VERT;
    VerticalMode armedMode1 = NONE_VERT;
    VerticalMode armedMode2 = NONE_VERT;

    if (lvl_mode_selected) {
        activeMode = LVL_VERT;
    }

    else if (vs_mode_selected) {

        if (gs_mode_armed) {
            activeMode = VS;
            armedMode1 = ALTS;
            armedMode2 = GS;
        }

        else {
            activeMode = VS;
            armedMode1 = ALTS;
            armedMode2 = NONE_VERT;
        }

    }

    else if (ias_mode_selected) {

        if (gs_mode_armed) {
            activeMode = IAS;
            armedMode1 = ALTS;
            armedMode2 = GS;
        }

        else {
            activeMode = IAS;
            armedMode1 = ALTS;
            armedMode2 = NONE_VERT;
        }

    }

    else if (pit_mode_selected) {

        if (gs_mode_armed) {
            activeMode = PIT;
            armedMode1 = ALTS;
            armedMode2 = GS;
        }

        else {
            activeMode = PIT;
            armedMode1 = ALTS;
            armedMode2 = NONE_VERT;
        }

    }

    else if (alt_mode_selected && !alts_mode_active) {

        if (gs_mode_armed) {
            activeMode = ALT;
            armedMode1 = NONE_VERT;
            armedMode2 = GS;
        } else {
            activeMode = ALT;
            armedMode1 = NONE_VERT;
            armedMode2 = NONE_VERT;
        }
    }

    else if (gs_mode_active) {
        if (app_mode_selected) {
            activeMode = GS;
            armedMode1 = NONE_VERT;
            armedMode2 = NONE_VERT;
        }
    }

    /*else if (alts_mode_armed && abs(alt_ft_value - curr_alt >= 400)) {
        activeMode = ALTS;
        armedMode1 = ALT;
        armedMode2 = NONE_VERT;
    }*/

    renderVerticalMode(DisplayLayout, activeMode, armedMode1, armedMode2);
}

/**
 * @brief Renders the labels for the active and armed vertical modes on the display layout.
 * @param layout The layout to render the labels on.
 * @param activeMode The currently active vertical mode.
 * @param armedMode1 The first armed vertical mode.
 * @param armedMode2 The second armed vertical mode.
 */
void GFC600::renderVerticalMode(Layout layout, VerticalMode activeMode, VerticalMode armedMode1, VerticalMode armedMode2)
{
    // Clear the area where vertical mode labels will be displayed
    clearArea(LeftLine.StartPos.x + SPACING_FROM_LINES - 2, DISPLAY_START_Y, RightLine.StartPos.x - LeftLine.StartPos.x - SPACING_FROM_LINES - 1, DISPLAY_END_Y);

    // Render the labels for the active vertical mode
    switch (activeMode) {
    case VS:
        handleVerticalSpeedMode(layout);
        break;

    case LVL_VERT:
        _renderLabel("LVL", layout.ActiveVerticalMode, NoOffset, false);
        break;

    case IAS:
        handleIasMode(layout);
        break;

    case PIT:
        _renderLabel("PIT", layout.ActiveVerticalMode, NoOffset, false);
        break;

    case ALT:
        handleAltMode(layout);
        break;

    case GS:
        _renderLabel("GS", layout.ActiveVerticalMode, NoOffset, false);
        break;

    case ALTS:
        flashAlts(layout.ActiveVerticalMode, NoOffset, true);
        break;

    default:
        // Clear the area if no valid active mode
        clearArea(LeftLine.StartPos.x + SPACING_FROM_LINES - 2, DISPLAY_START_Y, RightLine.StartPos.x - LeftLine.StartPos.x - SPACING_FROM_LINES - 1, DISPLAY_END_Y);
        break;
    }

    // Render the labels for the first armed vertical mode
    switch (armedMode1) {
    case ALTS:
        _renderLabel("ALTS", layout.ArmedVerticalMode1, NoOffset, false);
        break;

    case ALT:
        _renderLabel("ALT", layout.ArmedVerticalMode1, NoOffset, false);
        break;
    }

    // Render the labels for the second armed vertical mode
    switch (armedMode2) {
    case GS:
        _renderLabel("GS", layout.ArmedVerticalMode2, NoOffset, false);
        break;

    case NONE_VERT:
        // Clear the second armed mode label if no valid armed mode
        _renderLabel(CLEAR_STRING, layout.ArmedVerticalMode2, NoOffset, false);
        break;
    }
}

/*void GFC600::renderVerticalModess(Layout layout)
{

    if (lvl_mode_selected) {
        clearArea(LeftLine.StartPos.x + SPACING_FROM_LINES - 2, DISPLAY_START_Y, RightLine.StartPos.x - LeftLine.StartPos.x - SPACING_FROM_LINES - 1, DISPLAY_END_Y);
        _renderLabel("LVL", layout.ActiveVerticalMode, NoOffset, false);
    }

    else if (vs_mode_selected) {
        clearArea(LeftLine.StartPos.x + SPACING_FROM_LINES - 2, DISPLAY_START_Y, RightLine.StartPos.x - LeftLine.StartPos.x - SPACING_FROM_LINES - 1, DISPLAY_END_Y);
        handleVerticalSpeedMode(layout);
    }

    else if (alt_mode_selected) {
        clearArea(LeftLine.StartPos.x + SPACING_FROM_LINES - 2, DISPLAY_START_Y, RightLine.StartPos.x - LeftLine.StartPos.x - SPACING_FROM_LINES - 1, DISPLAY_END_Y);
        handleAltMode(layout);
    }

    else if (ias_mode_selected) {
        clearArea(LeftLine.StartPos.x + SPACING_FROM_LINES - 2, DISPLAY_START_Y, RightLine.StartPos.x - LeftLine.StartPos.x - SPACING_FROM_LINES - 1, DISPLAY_END_Y);
        handleIasMode(layout);
    }

    else if (pit_mode_selected) {
        clearArea(LeftLine.StartPos.x + SPACING_FROM_LINES - 2, DISPLAY_START_Y, RightLine.StartPos.x - LeftLine.StartPos.x - SPACING_FROM_LINES - 1, DISPLAY_END_Y);
        _renderLabel("PIT", layout.ActiveVerticalMode, NoOffset, false);
        _renderLabel("ALTS", layout.ArmedVerticalMode1, NoOffset, false);
    }

    // Armed vertical mode2
    if (app_mode_selected && gs_mode_armed) {
        _renderLabel("GS", layout.ArmedVerticalMode2, NoOffset, false);
    }
}*/

void GFC600::set(uint8_t messageID, const char *data)
{
    /* **********************************************************************************
        Each messageID has it's own value
        check for the messageID and define what to do:
    ********************************************************************************** */
    // do something according your messageID
    switch (messageID) {
    case PFT:
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
    case NAV_MODE:

        nav_mode_selected = atoi(data);
        cmdMessenger.sendCmd(kDebug, "VOR value: ");
        cmdMessenger.sendCmd(kDebug, nav_mode_selected);
        break;

        // Check if LOC is active
    case HAS_LOC:

        nav_has_loc = atoi(data);
        cmdMessenger.sendCmd(kDebug, "LOC value: ");
        cmdMessenger.sendCmd(kDebug, nav_has_loc);
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

    case ALT_MODE:
        // Implement the logic for setting Active VAPP Mode
        alt_mode_selected = atoi(data);
        cmdMessenger.sendCmd(kDebug, "Alt Mode value: ");
        cmdMessenger.sendCmd(kDebug, alt_mode_selected);
        break;

    case ALT_VALUE:
        // Implement the logic for setting Active VAPP Mode
        alt_ft_value = atoi(data);
        cmdMessenger.sendCmd(kDebug, "Alt ft value: ");
        cmdMessenger.sendCmd(kDebug, alt_ft_value);
        break;

    case ALTS_MODE:
        // Implement the logic for setting Active VAPP Mode
        alts_mode_armed = atoi(data);
        cmdMessenger.sendCmd(kDebug, "ALTS Mode value: ");
        cmdMessenger.sendCmd(kDebug, alts_mode_armed);
        break;

    case CURR_ALT:
        // Implement the logic for setting Active VAPP Mode
        curr_alt = atoi(data);
        cmdMessenger.sendCmd(kDebug, "Current Altitude value: ");
        cmdMessenger.sendCmd(kDebug, curr_alt);
        break;

    case GS_ARM:
        // Implement the logic for setting Active VAPP Mode
        gs_mode_armed = atoi(data);
        cmdMessenger.sendCmd(kDebug, "GS Armed value: ");
        cmdMessenger.sendCmd(kDebug, gs_mode_armed);
        break;

    case GS_ACTIVE:
        // Implement the logic for setting Active VAPP Mode
        gs_mode_active = atoi(data);
        cmdMessenger.sendCmd(kDebug, "GS Active value: ");
        cmdMessenger.sendCmd(kDebug, gs_mode_active);
        break;

    default:
        break;
    }

    decideLateralModes();
    decideVerticalModes();
    _oledDisplay->sendBuffer();
}

// Draw a line on the display.
void GFC600::drawLine(Line line)
{
    _oledDisplay->drawLine(line.StartPos.x, line.StartPos.y, line.EndPos.x, line.EndPos.y);
}

// Render a labeled text on the display.
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

// Render a symbol (UTF-8 arrow) with a labeled text on the display.
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

// Round a given value to the closest hundred.
int GFC600::roundToClosestHundred(int value)
{
    int remainder = value % 100;
    if (remainder < 50) {
        return (value / 100) * 100;
    } else {
        return ((value / 100) + 1) * 100;
    }
}

void GFC600::flashAlts(Label label, Position offset, bool update)
{
    const int flashCount          = 10;  // 10 flashes in total (5 seconds with 0.5-second flashes)
    const int flashDurationMillis = 500; // 0.5 seconds

    unsigned long previousMillis = millis(); // Initialize previousMillis

    bool drawState = false;

    _oledDisplay->setFont(label.Font);
    u8g2_int_t w = _oledDisplay->getStrWidth("ALTS");
    u8g2_int_t h = label.FontSize;

    for (int i = 0; i < flashCount; ++i) {
        unsigned long currentMillis = millis();

        if ((currentMillis - previousMillis) >= flashDurationMillis) {
            previousMillis = currentMillis;

            _oledDisplay->setFont(label.Font);
            _oledDisplay->setCursor(offset.x + label.Pos.x, offset.y + label.Pos.y);
            _oledDisplay->setFontMode(0);

            if (drawState) {
                _oledDisplay->setDrawColor(BLACK); // Black box
                _oledDisplay->drawBox(offset.x + label.Pos.x, offset.y + label.Pos.y - h, w, h);
                _oledDisplay->setDrawColor(WHITE); // White text
                _oledDisplay->print("ALTS");
            } else {
                _oledDisplay->setDrawColor(WHITE); // White box
                _oledDisplay->drawBox(offset.x + label.Pos.x, offset.y + label.Pos.y - h, w, h);
                _oledDisplay->setDrawColor(BLACK); // Black text
                _oledDisplay->print("ALTS");
            }

            if (update) _oledDisplay->sendBuffer();

            drawState = !drawState;
        }

        if (update) _oledDisplay->sendBuffer(); // Update display at each loop iteration if requested
    }
}
