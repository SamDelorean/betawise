#include "os3k.h"

/*
 * BatteryProbe
 *
 * Baseline probe for OS3K A-line syscall A138 / ShowBatteryPercentage().
 *
 * This first version deliberately exercises only the form already observed
 * in existing code:
 *
 *     ShowBatteryPercentage(0);
 *
 * Trigger: Ctrl+Cmd+B
 *
 * No attempt is made yet to vary the argument. Timed values are reserved for
 * a later probe so the baseline remains directly comparable with official use.
 */

APPLET_HEADER_BEGIN
    APPLET_ID(0xB138)
    APPLET_NAME("BatteryProbe")
    APPLET_INFO("A138 battery status baseline probe")
    APPLET_VERSION(BETAWISE_VERSION_MAJOR, BETAWISE_VERSION_MINOR, BETAWISE_VERSION_REVISION)
    APPLET_LANGUAGE_EN_US
APPLET_HEADER_END

GLOBAL_DATA_BEGIN
GLOBAL_DATA_END

static void ShowHome(void)
{
    ClearScreen();
    SetCursorMode(CURSOR_MODE_HIDE);
    PutStringCentered(1, "BatteryProbe - A138");
    PutStringCentered(2, "Press Ctrl+Cmd+B");
    PutStringCentered(3, "Calls A138 with 0");
}

void ProcessMessage(Message_e message, uint32_t param, uint32_t* status)
{
    KeyMod_e key;

    *status = 0;

    switch(message) {

        case MSG_INIT:
            break;

        case MSG_SETFOCUS:
            ShowHome();
            break;

        case MSG_KEY:
            key = (KeyMod_e)(param & ~KEY_MOD_CAPS_LOCK);

            if(key == (KEY_MOD_CTRL | KEY_MOD_CMD | KEY_B)) {
                ShowBatteryPercentage(0);
            }
            break;

        default:
            break;
    }
}
