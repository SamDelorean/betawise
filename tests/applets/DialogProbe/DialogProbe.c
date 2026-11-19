#include "os3k.h"

/*
 * DialogProbe - baseline test
 *
 * Purpose:
 *   Characterize the NORMAL use of OS3K A-line syscall A0F4 /
 *   DialogAddItem() before changing any uncertain parameter.
 *
 * Workflow deliberately follows the pattern already used by DebugTool:
 *
 *   DialogInit()
 *   DialogAddExitKey()
 *   DialogAddItem() x N
 *   DialogSetChoice()
 *   DialogDraw()
 *   DialogRun()
 *   DialogGetChoice()
 *   DialogGetChoiceId()
 *   DialogGetItemId()
 *
 * Open the test menu with Cmd+H.
 */

APPLET_HEADER_BEGIN
    APPLET_ID(0xD1A6)
    APPLET_NAME("DialogProbe")
    APPLET_INFO("DialogAddItem A0F4 baseline probe")
    APPLET_VERSION(BETAWISE_VERSION_MAJOR, BETAWISE_VERSION_MINOR, BETAWISE_VERSION_REVISION)
    APPLET_LANGUAGE_EN_US
APPLET_HEADER_END

GLOBAL_DATA_BEGIN
GLOBAL_DATA_END

static void PutLine(uint8_t row, const char* text)
{
    SetCursor(row, 1, CURSOR_MODE_HIDE);
    PutStringRaw(text);
}

static void ShowHome(void)
{
    ClearScreen();
    SetCursorMode(CURSOR_MODE_HIDE);

    PutLine(1, "DialogAddItem A0F4 probe");
    PutLine(2, "Press Cmd+H to open menu");
    PutLine(3, "Test: index + id + hotkey");
    PutLine(4, "ESC exits menu");
}

/*
 * Baseline experiment.
 *
 * All three entries use conservative/default-looking values:
 *
 *   marker    = ' '
 *   file_size = -1
 *
 * Only text, id and shortcut differ between items.
 *
 * We intentionally pass the COMPLETE visible strings ("ALPHA", etc.).
 * We do not yet assume that shortcut_key is automatically rendered as
 * part of the label. What appears on screen is itself experimental data.
 */
static void RunBaselineDialog(void)
{
    int add1;
    int add2;
    int add3;

    KeyMod_e run_key;
    char choice;
    int choice_id;
    int item_id = -1;

    char buf[40];

    ClearScreen();

    /* Same conservative 4-row geometry already used in DebugTool. */
    DialogInit(0, 1, 4, 40);

    DialogAddExitKey(KEY_ENTER);
    DialogAddExitKey(KEY_ESC);

    add1 = DialogAddItem("ALPHA", 5, ' ', 100, KEY_A, (size_t)-1);
    add2 = DialogAddItem("BETA",  4, ' ', 200, KEY_B, (size_t)-1);
    add3 = DialogAddItem("GAMMA", 5, ' ', 300, KEY_G, (size_t)-1);

    DialogSetChoice(1);
    DialogDraw();

    run_key = DialogRun() & ~KEY_MOD_CAPS_LOCK;

    choice = DialogGetChoice();
    choice_id = DialogGetChoiceId();

    if(choice >= 1 && choice <= 3) {
        item_id = DialogGetItemId((uint8_t)choice);
    }

    ClearScreen();
    SetCursorMode(CURSOR_MODE_HIDE);

    sprintf(buf, "add: %d %d %d", add1, add2, add3);
    PutLine(1, buf);

    sprintf(buf, "run=%04X choice=%d",
            (unsigned short)run_key,
            (int)choice);
    PutLine(2, buf);

    sprintf(buf, "choiceId=%d itemId=%d",
            choice_id,
            item_id);
    PutLine(3, buf);

    PutLine(4, "Any key -> home");
    WaitForKey();

    ShowHome();
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

            switch(key) {

                case KEY_MOD_CMD | KEY_H:
                    RunBaselineDialog();
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
}
