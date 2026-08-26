#include "os3k.h"

/*
 * FileSetFolder ABI regression probe.
 *
 * Discovery is finished: this applet is an executable specification for the
 * contract recovered from System 3 firmware.  Invalid-index cases are intended
 * for emulator-first execution before use on physical hardware.
 */

#define APPLET_ID_ALPHAWORD   0xA000
#define APPLET_ID_ALPHAQUIZ   0xA001
#define APPLET_ID_CALCULATOR  0xA002

#define FILESETFOLDER_RAW_NO_FILE_NAMESPACE  (-7)
#define FILESETFOLDER_RAW_INVALID_APPLET      (-64)
#define FILESETFOLDER_FLAG_0X40               0x00000040UL

APPLET_HEADER_BEGIN
    APPLET_ID(0xA1DF)
    APPLET_NAME("FileSetFolder Probe")
    APPLET_INFO("Betawise FileSetFolder ABI regression")
    APPLET_VERSION(BETAWISE_VERSION_MAJOR, BETAWISE_VERSION_MINOR, BETAWISE_VERSION_REVISION)
    APPLET_LANGUAGE_EN_US
APPLET_HEADER_END

typedef struct _ProbeResults_t {
    uint8_t alphaWordIndex;
    uint8_t alphaQuizIndex;
    uint8_t calculatorIndex;
    uint8_t missingMask;

    int32_t alphaWordResult;
    int32_t alphaWordRestoreResult;
    uint32_t alphaWordFlags;

    int32_t alphaQuizResult;
    int32_t alphaQuizRestoreResult;
    uint32_t alphaQuizFlags;

    int32_t noFilesResult;
    int32_t noFilesRestoreResult;
    uint32_t noFilesFlags;

    int32_t invalidResult;
    int32_t invalidRestoreResult;
    uint32_t invalidFlags;

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

static void reset_results(void) {
    memset(&g_results, 0, sizeof(g_results));
    g_results.alphaWordResult = -999;
    g_results.alphaWordRestoreResult = -999;
    g_results.alphaQuizResult = -999;
    g_results.alphaQuizRestoreResult = -999;
    g_results.noFilesResult = -999;
    g_results.noFilesRestoreResult = -999;
    g_results.invalidResult = -999;
    g_results.invalidRestoreResult = -999;
}

static void run_alpha_word_case(void) {
    int32_t previous;

    g_results.alphaWordFlags = 0xDEADBEEFUL;
    previous = FileSetFolder(g_results.alphaWordIndex, &g_results.alphaWordFlags);
    g_results.alphaWordResult = previous;

    check(previous >= 0);
    check((g_results.alphaWordFlags & FILESETFOLDER_FLAG_0X40) != 0);

    if (previous >= 0) {
        g_results.alphaWordRestoreResult = FileSetFolder((uint32_t)previous, NULL);
        check(g_results.alphaWordRestoreResult == (int32_t)g_results.alphaWordIndex);
    }
}

static void run_alpha_quiz_case(void) {
    int32_t previous;

    g_results.alphaQuizFlags = 0xDEADBEEFUL;
    previous = FileSetFolder(g_results.alphaQuizIndex, &g_results.alphaQuizFlags);
    g_results.alphaQuizResult = previous;

    check(previous >= 0);
    check((g_results.alphaQuizFlags & FILESETFOLDER_FLAG_0X40) == 0);

    if (previous >= 0) {
        g_results.alphaQuizRestoreResult = FileSetFolder((uint32_t)previous, NULL);
        check(g_results.alphaQuizRestoreResult == (int32_t)g_results.alphaQuizIndex);
    }
}

static void run_no_file_namespace_case(void) {
    int32_t originalFolder;

    /*
     * Use AlphaQuiz as a known-valid anchor, then attempt Calculator.  Stock
     * Calculator has fileCount=0 and no header flag 0x40, so A1C0 takes the
     * confirmed -7 validation path.  Restoring the anchor also checks that the
     * failed call did not change current_folder.
     */
    originalFolder = FileSetFolder(g_results.alphaQuizIndex, NULL);
    check(originalFolder >= 0);
    if (originalFolder < 0) {
        return;
    }

    g_results.noFilesFlags = 0xDEADBEEFUL;
    g_results.noFilesResult = FileSetFolder(g_results.calculatorIndex, &g_results.noFilesFlags);

    check(g_results.noFilesResult == FILESETFOLDER_RAW_NO_FILE_NAMESPACE);
    check(g_results.noFilesFlags == 0);

    g_results.noFilesRestoreResult = FileSetFolder((uint32_t)originalFolder, NULL);
    check(g_results.noFilesRestoreResult == (int32_t)g_results.alphaQuizIndex);
}

static void run_invalid_index_case(void) {
    int32_t originalFolder;

    /* applet_index 32 is the first value outside the firmware's 0..31 range. */
    originalFolder = FileSetFolder(g_results.alphaQuizIndex, NULL);
    check(originalFolder >= 0);
    if (originalFolder < 0) {
        return;
    }

    g_results.invalidFlags = 0xDEADBEEFUL;
    g_results.invalidResult = FileSetFolder(32, &g_results.invalidFlags);

    check(g_results.invalidResult == FILESETFOLDER_RAW_INVALID_APPLET);
    check(g_results.invalidFlags == 0);

    g_results.invalidRestoreResult = FileSetFolder((uint32_t)originalFolder, NULL);
    check(g_results.invalidRestoreResult == (int32_t)g_results.alphaQuizIndex);
}

static void run_probe(void) {
    reset_results();

    g_results.alphaWordIndex = AppletFindById(APPLET_ID_ALPHAWORD);
    g_results.alphaQuizIndex = AppletFindById(APPLET_ID_ALPHAQUIZ);
    g_results.calculatorIndex = AppletFindById(APPLET_ID_CALCULATOR);

    if (g_results.alphaWordIndex == 0) {
        g_results.missingMask |= 1;
    }
    if (g_results.alphaQuizIndex == 0) {
        g_results.missingMask |= 2;
    }
    if (g_results.calculatorIndex == 0) {
        g_results.missingMask |= 4;
    }

    if (g_results.missingMask == 0) {
        run_alpha_word_case();
        run_alpha_quiz_case();
        run_no_file_namespace_case();
        run_invalid_index_case();
    }
}

static void draw_results(void) {
    ClearScreen();
    SetCursorMode(CURSOR_MODE_HIDE);

    if (g_results.missingMask != 0) {
        PutStringCentered(1, "FileSetFolder probe");
        SetCursor(2, 1, CURSOR_MODE_HIDE);
        sprintf(g_line, "Missing stock app:%02x", g_results.missingMask);
        PutStringRaw(g_line);
        SetCursor(3, 1, CURSOR_MODE_HIDE);
        PutStringRaw("Need AW/AQ/Calculator");
        return;
    }

    SetCursor(1, 1, CURSOR_MODE_HIDE);
    sprintf(g_line, "FSF %u/%u fail", g_results.failures, g_results.checks);
    PutStringRaw(g_line);

    SetCursor(2, 1, CURSOR_MODE_HIDE);
    sprintf(g_line, "AW %ld f=%02lx", (long)g_results.alphaWordResult,
            (unsigned long)(g_results.alphaWordFlags & 0xFF));
    PutStringRaw(g_line);

    SetCursor(3, 1, CURSOR_MODE_HIDE);
    sprintf(g_line, "CAL %ld f=%08lx", (long)g_results.noFilesResult,
            (unsigned long)g_results.noFilesFlags);
    PutStringRaw(g_line);

    SetCursor(4, 1, CURSOR_MODE_HIDE);
    sprintf(g_line, "BAD %ld f=%08lx", (long)g_results.invalidResult,
            (unsigned long)g_results.invalidFlags);
    PutStringRaw(g_line);
}

static void draw_idle_screen(void) {
    ClearScreen();
    SetCursorMode(CURSOR_MODE_HIDE);
    PutStringCentered(1, "FileSetFolder probe");
    PutStringCentered(2, "Cmd+F: run tests");
    PutStringCentered(4, "Emulator first");
}

void ProcessMessage(Message_e message, uint32_t param, uint32_t* status) {
    *status = 0;

    switch (message) {
        case MSG_SETFOCUS:
            draw_idle_screen();
            break;

        case MSG_KEY:
            if (((param & 0xFF) == KEY_F) && ((param & KEY_MOD_CMD) != 0)) {
                run_probe();
                draw_results();
            }
            break;

        default:
            break;
    }
}
