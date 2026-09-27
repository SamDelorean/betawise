; DebugTool-AS2000 fixed v0 workspace ABI.
; Increment 2 authority: ABI_STATE.md.
; All offsets are relative to externally bound DBG_WS_BASE.
;
; No absolute RAM address belongs in this file.

        .equ DBG_WS_SIZE,          30

        .equ DBG_WS_MEM_BANK,      0
        .equ DBG_WS_MEM_ADDR,      1
        .equ DBG_WS_MEM_CURSOR,    3
        .equ DBG_WS_UI_MODE,       4
        .equ DBG_WS_INPUT_DIGITS,  5
        .equ DBG_WS_INPUT_BANK,    6
        .equ DBG_WS_INPUT_VALUE,   7
        .equ DBG_WS_CALL_CTL,      9
        .equ DBG_WS_CALL_TARGET,  10
        .equ DBG_WS_CALL_IN_A,    12
        .equ DBG_WS_CALL_IN_B,    13
        .equ DBG_WS_CALL_IN_X,    14
        .equ DBG_WS_CALL_IN_Y,    16
        .equ DBG_WS_CALL_OUT_A,   18
        .equ DBG_WS_CALL_OUT_B,   19
        .equ DBG_WS_CALL_OUT_X,   20
        .equ DBG_WS_CALL_OUT_Y,   22
        .equ DBG_WS_CALL_OUT_CCR, 24
        .equ DBG_WS_ENV_SAVED_MAP,25
        .equ DBG_WS_TMP0,         26
        .equ DBG_WS_TMP1,         27
        .equ DBG_WS_TMP_WORD,     28

        .equ DBG_MODE_MEM,         0
        .equ DBG_MODE_EDIT,        1
        .equ DBG_MODE_GOTO,        2
        .equ DBG_MODE_CALL,        3
        .equ DBG_MODE_INFO,        4

        .equ DBG_INPUT_NO_BANK,  0xff

        .equ DBG_CALL_FIELD_TARGET,0
        .equ DBG_CALL_FIELD_A,     1
        .equ DBG_CALL_FIELD_B,     2
        .equ DBG_CALL_FIELD_X,     3
        .equ DBG_CALL_FIELD_Y,     4
        .equ DBG_CALL_RESULT_VALID,0x80
