#include "os3k.h"
#include "file_dynamic.h"

/*
 * FileIOProbe — dynamic regression for the System 3 binary File API.
 *
 * Executes the dynamic cases specified (not yet executed) in:
 *   docs/fileopen-a1c8-source-first-closure.md        A1C8 FileOpen
 *   docs/fileclose-a1cc-source-first-closure.md       A1CC FileClose
 *   docs/filewritebuffer-a198-source-first-closure.md A198 FileWriteBuffer
 *   docs/filereadbuffer-a19c-source-first-closure.md  A19C FileReadBuffer
 *   docs/sys-a190-file-position-closure.md            A190
 *   docs/sys-a194-file-position-query-closure.md      A194
 * using A1E8 to create, and A1EC to remove, a private test file.
 *
 * Runs automatically on MSG_SETFOCUS and leaves no file behind when A1EC
 * succeeds. Screen line 1 is "FILEIO <failures>/<checks> fail"; line 2 lists
 * the first failing check numbers. Check numbers are stable: see README.md.
 */

/* Not yet declared in os3k.h; resolved through os3k/syscall.c indices 100/101.
 * Proposed name (isotherm/betawise os3k_file.h): FileSeek.
 * origin 0 = start, 1 = current, 2 = end; result clamped to [0, size]. */
int32_t SYS_A190(uint16_t file_id, int32_t offset, uint8_t origin);
/* Proposed name (isotherm/betawise os3k_file.h): FileTell. */
int32_t SYS_A194(uint16_t file_id);

#define FILE_ID_ACTIVE   0x0000
#define FILE_ID_INVALID  0x7F7F   /* not expected to resolve on fresh NVRAM */
#define MAX_FAILS_SHOWN  8

APPLET_HEADER_BEGIN
    APPLET_ID(0xA19C)
    APPLET_NAME("FileIOProbe")
    APPLET_INFO("A190-A19C/A1C8/A1CC dynamic regression")
    APPLET_VERSION(BETAWISE_VERSION_MAJOR, BETAWISE_VERSION_MINOR, BETAWISE_VERSION_REVISION)
    APPLET_LANGUAGE_EN_US
APPLET_HEADER_END

static uint16_t g_checks;
static uint16_t g_failures;
static uint8_t  g_failed_ids[MAX_FAILS_SHOWN];
static int32_t  g_token;
static char     g_line[48];

static void check(bool condition) {
    g_checks++;
    if (!condition) {
        if (g_failures < MAX_FAILS_SHOWN) {
            g_failed_ids[g_failures] = (uint8_t)g_checks;
        }
        g_failures++;
    }
}

static bool same(const uint8_t* a, const char* b, uint32_t n) {
    return memcmp(a, b, n) == 0;
}

static void run_probe(void) {
    uint8_t buf[16];
    uint16_t tok;

    g_checks = 0;
    g_failures = 0;

    /* --- setup: create + open ------------------------------------------ */
    g_token = SYS_A1E8(0, "FIOPROBE", 0, 0x200);
    check(g_token > 0);                                          /*  1 */
    if (g_token <= 0) {
        return;
    }
    tok = (uint16_t)g_token;
    check(FileOpen(tok, 1) == tok);                              /*  2 */
    check(SYS_A194(tok) == 0);                                   /*  3 */

    /* --- A198 mode 1 append -------------------------------------------- */
    check(FileWriteBuffer(tok, (const uint8_t*)"ABCDE", 5, 1) == 5); /* 4 */
    check(SYS_A194(tok) == 0);                                   /*  5 append keeps cursor */
    check(SYS_A190(tok, 0, 2) == 5);                             /*  6 size 5 */

    /* --- A19C reads ----------------------------------------------------- */
    check(SYS_A190(tok, 0, 0) == 0);                             /*  7 */
    memset(buf, 0, sizeof(buf));
    check(FileReadBuffer(tok, buf, 3) == 3 && same(buf, "ABC", 3)); /* 8 */
    check(SYS_A194(tok) == 3);                                   /*  9 cursor advanced */
    memset(buf, 0, sizeof(buf));
    check(FileReadBuffer(tok, buf, 10) == 2 && same(buf, "DE", 2)); /* 10 EOF clamp */
    check(SYS_A194(tok) == 5);                                   /* 11 */
    check(FileReadBuffer(tok, buf, 4) == 0);                     /* 12 at EOF */
    check(FileReadBuffer(tok, NULL, 1) == -6);                   /* 13 null buffer */
    check(SYS_A194(tok) == 5);                                   /* 14 unchanged */

    /* --- A198 mode 2 insert -------------------------------------------- */
    check(SYS_A190(tok, 1, 0) == 1);                             /* 15 */
    check(FileWriteBuffer(tok, (const uint8_t*)"xy", 2, 2) == 2); /* 16 */
    check(SYS_A194(tok) == 3);                                   /* 17 cursor += 2 */
    check(SYS_A190(tok, 0, 2) == 7);                             /* 18 "AxyBCDE" */

    /* --- A198 mode 3 overwrite, extending EOF -------------------------- */
    check(SYS_A190(tok, 5, 0) == 5);                             /* 19 */
    check(FileWriteBuffer(tok, (const uint8_t*)"QRS", 3, 3) == 3); /* 20 */
    check(SYS_A194(tok) == 8);                                   /* 21 */
    check(SYS_A190(tok, 0, 2) == 8);                             /* 22 "AxyBCQRS" */
    check(SYS_A190(tok, 0, 0) == 0);                             /* 23 */
    memset(buf, 0, sizeof(buf));
    check(FileReadBuffer(tok, buf, 16) == 8 && same(buf, "AxyBCQRS", 8)); /* 24 */

    /* --- A198 invalid mode / zero count -------------------------------- */
    check(FileWriteBuffer(tok, (const uint8_t*)"Z", 1, 4) == -5); /* 25 */
    check(FileWriteBuffer(tok, (const uint8_t*)"Z", 0, 1) == 0);  /* 26 */
    check(SYS_A190(tok, 0, 2) == 8);                             /* 27 size unchanged */

    /* --- A190 origins and clamps --------------------------------------- */
    check(SYS_A190(tok, 2, 0) == 2);                             /* 28 */
    check(SYS_A190(tok, 0, 3) == -2);                            /* 29 invalid origin */
    check(SYS_A194(tok) == 2);                                   /* 30 cursor untouched */
    check(SYS_A190(tok, -100, 1) == 0);                          /* 31 clamp low */
    check(SYS_A190(tok, 100, 0) == 8);                           /* 32 clamp EOF */
    check(SYS_A190(tok, -3, 2) == 5);                            /* 33 end-relative */
    check(SYS_A190(tok, 2, 1) == 7);                             /* 34 current-relative */

    /* --- A194 on token 0 (active descriptor) --------------------------- */
    check(SYS_A194(FILE_ID_ACTIVE) == 7);                        /* 35 */

    /* --- A1C8 reset_position / A1CC ------------------------------------ */
    check(SYS_A190(tok, 4, 0) == 4);                             /* 36 */
    FileClose();
    check(FileOpen(tok, 0) == tok);                              /* 37 */
    check(SYS_A194(tok) == 4);                                   /* 38 0 keeps cursor */
    check(FileOpen(tok, 1) == tok);                              /* 39 */
    check(SYS_A194(tok) == 0);                                   /* 40 1 rewinds */

    /* --- A1C8 failure preserves active context ------------------------- */
    SYS_A190(tok, 6, 0);
    check(FileOpen(FILE_ID_INVALID, 1) == 0);                    /* 41 */
    check(SYS_A194(FILE_ID_ACTIVE) == 6);                        /* 42 */

    /* --- unresolved token never returns a byte count ------------------- */
    check(FileReadBuffer(FILE_ID_INVALID, buf, 1) <= 0);         /* 43 */

    /* --- teardown: close + remove -------------------------------------- */
    FileClose();
    check(SYS_A194(tok) == 6);                                   /* 44 explicit token after close */
    check(SYS_A1EC(tok) == 0);                                   /* 45 */
}

static void draw_results(void) {
    uint16_t i;
    uint16_t shown;
    char* p;

    ClearScreen();
    SetCursorMode(CURSOR_MODE_HIDE);

    SetCursor(1, 1, CURSOR_MODE_HIDE);
    sprintf(g_line, "FILEIO %u/%u fail", g_failures, g_checks);
    PutStringRaw(g_line);

    SetCursor(2, 1, CURSOR_MODE_HIDE);
    p = g_line;
    p += sprintf(p, "ids");
    shown = g_failures < MAX_FAILS_SHOWN ? g_failures : MAX_FAILS_SHOWN;
    for (i = 0; i < shown; i++) {
        p += sprintf(p, " %u", g_failed_ids[i]);
    }
    PutStringRaw(g_line);

    SetCursor(3, 1, CURSOR_MODE_HIDE);
    sprintf(g_line, "tok %08lx", (unsigned long)g_token);
    PutStringRaw(g_line);
}

void ProcessMessage(Message_e message, uint32_t param, uint32_t* status) {
    (void)param;
    *status = 0;

    switch (message) {
        case MSG_SETFOCUS:
            run_probe();
            draw_results();
            break;

        default:
            break;
    }
}
