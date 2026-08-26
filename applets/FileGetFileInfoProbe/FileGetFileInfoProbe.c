#include "os3k.h"

/*
 * FileGetFileInfo / A1C4 regression probe.
 *
 * Uses the reserved clipboard token because it resolves independently of the
 * current SmartApplet folder.  The probe is read-only: it does not modify the
 * clipboard or any file contents.
 */

#define FILE_TOKEN_CLIPBOARD 0x00CB

APPLET_HEADER_BEGIN
    APPLET_ID(0xA1E1)
    APPLET_NAME("FileGetFileInfo Probe")
    APPLET_INFO("Betawise A1C4/FileGetFileInfo ABI regression")
    APPLET_VERSION(BETAWISE_VERSION_MAJOR, BETAWISE_VERSION_MINOR, BETAWISE_VERSION_REVISION)
    APPLET_LANGUAGE_EN_US
APPLET_HEADER_END

typedef struct _ProbeResults_t {
    uint8_t* returnedStorage;
    uint8_t* mirroredStorage;
    uint8_t* unbindReturn;
    uint32_t currentSize;
    uint32_t maxSize;
    uint32_t cursor;
    uint16_t checks;
    uint16_t failures;
} ProbeResults_t;

static ProbeResults_t g_results;
static char g_line[48];

static void check(bool condition) {
    g_results.checks++;
    if (!condition) {
        g_results.failures++;
    }
}

static void run_probe(void) {
    uint8_t* storage = (uint8_t*)0xDEADBEEFUL;
    uint32_t size = 0xDEADBEEFUL;
    uint32_t maxSize = 0xDEADBEEFUL;
    uint32_t cursor = 0xDEADBEEFUL;
    uint8_t* base;
    uint8_t* unbindBase;

    memset(&g_results, 0, sizeof(g_results));

    base = FileGetFileInfo(
        FILE_TOKEN_CLIPBOARD,
        &storage,
        &size,
        &maxSize,
        &cursor
    );

    g_results.returnedStorage = base;
    g_results.mirroredStorage = storage;
    g_results.currentSize = size;
    g_results.maxSize = maxSize;
    g_results.cursor = cursor;

    /* Direct A1C4 contract. */
    check(base != NULL);
    check(storage == base);
    check(size != 0xDEADBEEFUL);
    check(maxSize != 0xDEADBEEFUL);
    check(cursor != 0xDEADBEEFUL);

    /* Normal descriptor invariants; useful as emulator regressions. */
    check(size <= maxSize);
    check(cursor <= maxSize);

    /*
     * A1C4 stores mirror addresses persistently in descriptor+0x24..+0x30.
     * System 3 itself clears temporary bindings by calling A1C4 again with
     * NULL pointers.  Do the same before these stack locals leave scope.
     */
    unbindBase = FileGetFileInfo(
        FILE_TOKEN_CLIPBOARD,
        NULL,
        NULL,
        NULL,
        NULL
    );
    g_results.unbindReturn = unbindBase;

    check(unbindBase == base);

    /*
     * Emulator trace assertion after the second call:
     * descriptor+0x24 == descriptor+0x28 == descriptor+0x2C ==
     * descriptor+0x30 == 0.
     */
}

static void draw_results(void) {
    ClearScreen();
    SetCursorMode(CURSOR_MODE_HIDE);

    SetCursor(1, 1, CURSOR_MODE_HIDE);
    sprintf(g_line, "FGFI %u/%u fail", g_results.failures, g_results.checks);
    PutStringRaw(g_line);

    SetCursor(2, 1, CURSOR_MODE_HIDE);
    sprintf(g_line, "ptr %08lx/%08lx",
            (unsigned long)g_results.returnedStorage,
            (unsigned long)g_results.mirroredStorage);
    PutStringRaw(g_line);

    SetCursor(3, 1, CURSOR_MODE_HIDE);
    sprintf(g_line, "sz %lu max %lu",
            (unsigned long)g_results.currentSize,
            (unsigned long)g_results.maxSize);
    PutStringRaw(g_line);

    SetCursor(4, 1, CURSOR_MODE_HIDE);
    sprintf(g_line, "cur %lu ub %08lx",
            (unsigned long)g_results.cursor,
            (unsigned long)g_results.unbindReturn);
    PutStringRaw(g_line);
}

static void draw_idle_screen(void) {
    ClearScreen();
    SetCursorMode(CURSOR_MODE_HIDE);
    PutStringCentered(1, "FileGetFileInfo probe");
    PutStringCentered(2, "Cmd+I: run tests");
    PutStringCentered(4, "Read-only / unbinds");
}

void ProcessMessage(Message_e message, uint32_t param, uint32_t* status) {
    *status = 0;

    switch (message) {
        case MSG_SETFOCUS:
            draw_idle_screen();
            break;

        case MSG_KEY:
            if (((param & 0xFF) == KEY_I) && ((param & KEY_MOD_CMD) != 0)) {
                run_probe();
                draw_results();
            }
            break;

        default:
            break;
    }
}
