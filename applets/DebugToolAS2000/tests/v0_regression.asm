; DebugTool-AS2000 v0 deterministic consolidation regression.
; Test-only harness. Not linked into the product image.

        .include "debugtool_state.inc"

        .extern DBG_WS_BASE
        .extern DBG_BIND_DYNFS_ACTIVE_CTX
        .extern DBG_BIND_STOCK_FSLOT
        .extern DBG_BIND_STOCK_VIEW
        .extern DBG_BIND_STOCK_CURSOR
        .extern DBG_BIND_STOCK_END

        .extern DBG_MEM_READ_X
        .extern DBG_MEM_REDRAW
        .extern DBG_GOTO_RESET
        .extern DBG_GOTO_ACCEPT_A
        .extern DBG_EDIT_RESET
        .extern DBG_EDIT_ACCEPT_A
        .extern DBG_CALL_EXECUTE
        .extern DBG_INFO_REDRAW

        .extern DBG_BIND_LCD_ROW3
        .extern DBG_BIND_LCD_PUTS

        .text
        .globl DBG_V0_REGRESSION_ENTRY

DBG_TEST_BANK0:
        LDAA    0x0000
        ANDA    #0x8f
        ORAA    #0x40
        STAA    0x0000
        RTS

DBG_TEST_BANK1:
        LDAA    0x0000
        ANDA    #0x8f
        ORAA    #0x50
        STAA    0x0000
        RTS

DBG_TEST_READ_B1_123B:
        JSR     DBG_TEST_BANK1
        LDAA    0x123b
        TAB
        JSR     DBG_TEST_BANK0
        TBA
        RTS

; Synthetic CALL target verifies all four input registers.
DBG_TEST_CALL_TARGET:
        CMPA    #0x12
        BNE     DBG_TEST_CALL_BAD
        CMPB    #0x34
        BNE     DBG_TEST_CALL_BAD
        CPX     #0x5678
        BNE     DBG_TEST_CALL_BAD
        CPY     #0x9abc
        BNE     DBG_TEST_CALL_BAD
        LDAA    #0xa1
        LDAB    #0xb2
        LDX     #0xc3d4
        LDY     #0xe5f6
        CLC
        CLV
        RTS
DBG_TEST_CALL_BAD:
        LDAA    #0xee
        LDAB    #0xee
        RTS

DBG_V0_REGRESSION_ENTRY:
        ; ---------------------------------------------------------------
        ; Seed physical bank 1 at 1234: "V0REGRES".
        ; ---------------------------------------------------------------
        JSR     DBG_TEST_BANK1
        LDX     #0x1234
        LDAA    #'V'
        STAA    0,X
        LDAA    #'0'
        STAA    1,X
        LDAA    #'R'
        STAA    2,X
        LDAA    #'E'
        STAA    3,X
        LDAA    #'G'
        STAA    4,X
        LDAA    #'R'
        STAA    5,X
        LDAA    #'E'
        STAA    6,X
        LDAA    #'S'
        STAA    7,X
        JSR     DBG_TEST_BANK0

        ; ---------------------------------------------------------------
        ; MEM: physical banked read + full redraw must survive.
        ; ---------------------------------------------------------------
        LDAA    #1
        STAA    DBG_WS_BASE+DBG_WS_MEM_BANK
        LDD     #0x1234
        STD     DBG_WS_BASE+DBG_WS_MEM_ADDR
        CLRA
        STAA    DBG_WS_BASE+DBG_WS_MEM_CURSOR
        LDX     #0x1234
        JSR     DBG_MEM_READ_X
        CMPA    #'V'
        BNE     DBG_V0_FAIL_MEM
        JSR     DBG_MEM_REDRAW

        ; ---------------------------------------------------------------
        ; GOTO: parse 1:1234 and commit.
        ; ---------------------------------------------------------------
        JSR     DBG_GOTO_RESET
        LDAA    #'1'
        JSR     DBG_GOTO_ACCEPT_A
        LDAA    #':'
        JSR     DBG_GOTO_ACCEPT_A
        LDAA    #'1'
        JSR     DBG_GOTO_ACCEPT_A
        LDAA    #'2'
        JSR     DBG_GOTO_ACCEPT_A
        LDAA    #'3'
        JSR     DBG_GOTO_ACCEPT_A
        LDAA    #'4'
        JSR     DBG_GOTO_ACCEPT_A
        LDAA    #0x0d
        JSR     DBG_GOTO_ACCEPT_A
        TSTB
        BEQ     DBG_V0_FAIL_GOTO
        LDAA    DBG_WS_BASE+DBG_WS_MEM_BANK
        CMPA    #1
        BNE     DBG_V0_FAIL_GOTO
        LDD     DBG_WS_BASE+DBG_WS_MEM_ADDR
        CPD     #0x1234
        BNE     DBG_V0_FAIL_GOTO

        ; ---------------------------------------------------------------
        ; EDIT: cursor +7, first nibble must not write, second commits '!'.
        ; ---------------------------------------------------------------
        LDAA    #7
        STAA    DBG_WS_BASE+DBG_WS_MEM_CURSOR
        JSR     DBG_EDIT_RESET
        LDAA    #'2'
        JSR     DBG_EDIT_ACCEPT_A
        TSTB
        BNE     DBG_V0_FAIL_EDIT
        JSR     DBG_TEST_READ_B1_123B
        CMPA    #'S'
        BNE     DBG_V0_FAIL_EDIT
        LDAA    #'1'
        JSR     DBG_EDIT_ACCEPT_A
        TSTB
        BEQ     DBG_V0_FAIL_EDIT
        JSR     DBG_TEST_READ_B1_123B
        CMPA    #'!'
        BNE     DBG_V0_FAIL_EDIT

        ; ---------------------------------------------------------------
        ; CALL: native A/B/X/Y delivery + return capture.
        ; ---------------------------------------------------------------
        LDX     #DBG_TEST_CALL_TARGET
        STX     DBG_WS_BASE+DBG_WS_CALL_TARGET
        LDAA    #0x12
        STAA    DBG_WS_BASE+DBG_WS_CALL_IN_A
        LDAA    #0x34
        STAA    DBG_WS_BASE+DBG_WS_CALL_IN_B
        LDD     #0x5678
        STD     DBG_WS_BASE+DBG_WS_CALL_IN_X
        LDD     #0x9abc
        STD     DBG_WS_BASE+DBG_WS_CALL_IN_Y
        CLRA
        STAA    DBG_WS_BASE+DBG_WS_CALL_CTL
        JSR     DBG_CALL_EXECUTE
        LDAA    DBG_WS_BASE+DBG_WS_CALL_OUT_A
        CMPA    #0xa1
        BNE     DBG_V0_FAIL_CALL
        LDAA    DBG_WS_BASE+DBG_WS_CALL_OUT_B
        CMPA    #0xb2
        BNE     DBG_V0_FAIL_CALL
        LDD     DBG_WS_BASE+DBG_WS_CALL_OUT_X
        CPD     #0xc3d4
        BNE     DBG_V0_FAIL_CALL
        LDD     DBG_WS_BASE+DBG_WS_CALL_OUT_Y
        CPD     #0xe5f6
        BNE     DBG_V0_FAIL_CALL
        LDAA    DBG_WS_BASE+DBG_WS_CALL_CTL
        BITA    #DBG_CALL_RESULT_VALID
        BEQ     DBG_V0_FAIL_CALL

        ; ---------------------------------------------------------------
        ; INFO: seed known live state and render final diagnostic screen.
        ; ---------------------------------------------------------------
        LDAA    #0x03
        STAA    DBG_BIND_STOCK_FSLOT
        LDD     #0x3004
        STD     DBG_BIND_STOCK_VIEW
        LDD     #0x3012
        STD     DBG_BIND_STOCK_CURSOR
        LDD     #0x3080
        STD     DBG_BIND_STOCK_END
        LDD     #0x4100
        STD     DBG_BIND_DYNFS_ACTIVE_CTX+0
        CLRA
        CLRB
        STD     DBG_BIND_DYNFS_ACTIVE_CTX+2
        LDAA    #0x07
        STAA    DBG_BIND_DYNFS_ACTIVE_CTX+4
        LDD     #0x3000
        STD     DBG_BIND_DYNFS_ACTIVE_CTX+5
        LDAA    #1
        STAA    DBG_WS_BASE+DBG_WS_MEM_BANK

        JSR     DBG_INFO_REDRAW
        JSR     DBG_BIND_LCD_ROW3
        LDX     #DBG_V0_PASS_MSG
        JSR     DBG_BIND_LCD_PUTS
DBG_V0_HOLD:
        SEI
        BRA     DBG_V0_HOLD

DBG_V0_FAIL_MEM:
        LDX     #DBG_V0_FAIL_MEM_MSG
        BRA     DBG_V0_FAIL
DBG_V0_FAIL_GOTO:
        LDX     #DBG_V0_FAIL_GOTO_MSG
        BRA     DBG_V0_FAIL
DBG_V0_FAIL_EDIT:
        LDX     #DBG_V0_FAIL_EDIT_MSG
        BRA     DBG_V0_FAIL
DBG_V0_FAIL_CALL:
        LDX     #DBG_V0_FAIL_CALL_MSG
DBG_V0_FAIL:
        JSR     DBG_BIND_LCD_ROW3
        JSR     DBG_BIND_LCD_PUTS
        BRA     DBG_V0_HOLD

DBG_V0_PASS_MSG:
        .asciz  "V0 PASS"
DBG_V0_FAIL_MEM_MSG:
        .asciz  "FAIL MEM"
DBG_V0_FAIL_GOTO_MSG:
        .asciz  "FAIL GOTO"
DBG_V0_FAIL_EDIT_MSG:
        .asciz  "FAIL EDIT"
DBG_V0_FAIL_CALL_MSG:
        .asciz  "FAIL CALL"
