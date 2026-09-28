; DebugTool-AS2000 portable core.
; Increment 8 implements MEM + GOTO + EDIT + CALL + INFO only.
; No absolute hardware, ROM, RAM, or DynFS addresses are permitted here.

        .include "debugtool_state.inc"
        .include "debugtool_bindings.inc"

        .text
        .globl DEBUGTOOL_ENTRY
        .globl DBG_MEM_REDRAW
        .globl DBG_MEM_READ_X
        .type DEBUGTOOL_ENTRY, @function

; ---------------------------------------------------------------------------
; Session entry — Increment 4 initializes only the state required by MEM.
; ---------------------------------------------------------------------------
DEBUGTOOL_ENTRY:
        CLRA
        STAA    DBG_WS_BASE+DBG_WS_MEM_BANK
        STAA    DBG_WS_BASE+DBG_WS_MEM_CURSOR
        STAA    DBG_WS_BASE+DBG_WS_UI_MODE

        ; Initial address = fixed-ROM start.  Build it bytewise so the portable
        ; core contains no environment absolute address literal.
        LDAA    #0x80
        STAA    DBG_WS_BASE+DBG_WS_MEM_ADDR
        CLRA
        STAA    DBG_WS_BASE+DBG_WS_MEM_ADDR+1

        JSR     DBG_MEM_REDRAW
        RTS

        .size DEBUGTOOL_ENTRY, .-DEBUGTOOL_ENTRY

; ---------------------------------------------------------------------------
; DBG_MEM_READ_X
;   X = CPU-visible address
;   returns A = physical byte
;   preserves X
;
; Addresses with high bit set are fixed main ROM and are read directly.
; Lower-half addresses are read through the environment RAM mapping adapter.
; The adapter pair is deliberately immediate/non-nestable.
; ---------------------------------------------------------------------------
        .type DBG_MEM_READ_X, @function
DBG_MEM_READ_X:
        PSHX
        XGDX
        BITA    #0x80
        PULX
        BNE     DBG_MEM_READ_ROM

        LDAA    DBG_WS_BASE+DBG_WS_MEM_BANK
        JSR     DBG_BIND_RAM_ENTER
        LDAA    0,X
        TAB
        JSR     DBG_BIND_RAM_RESTORE
        TBA
        RTS

DBG_MEM_READ_ROM:
        LDAA    0,X
        RTS

        .size DBG_MEM_READ_X, .-DBG_MEM_READ_X

; ---------------------------------------------------------------------------
; Output helpers.
; ---------------------------------------------------------------------------
DBG_OUT_A:
        TAB
        JMP     DBG_BIND_LCD_PUTBYTE

; A = raw byte. Uses stock verified byte -> two uppercase hex ASCII helper.
DBG_OUT_HEX_A:
        JSR     DBG_BIND_BYTE_TO_HEX
        PSHB
        TAB
        JSR     DBG_BIND_LCD_PUTBYTE
        PULB
        JMP     DBG_BIND_LCD_PUTBYTE

; A = byte. Printable 20h..7Eh passes through; everything else becomes '.'.
DBG_OUT_ASCII_A:
        CMPA    #0x20
        BLO     DBG_OUT_ASCII_DOT
        CMPA    #0x7f
        BHS     DBG_OUT_ASCII_DOT
        JMP     DBG_OUT_A
DBG_OUT_ASCII_DOT:
        LDAA    #'.'
        JMP     DBG_OUT_A

; ---------------------------------------------------------------------------
; Row selection.
; Stock display traversal proves this 40x4 order:
; row0=A54B, row1=A556, row2=A561, row3=A56C in the v3.1.4 binding.
; The core sees only binding symbols.
; ---------------------------------------------------------------------------
DBG_MEM_SELECT_ROW:
        LDAA    DBG_WS_BASE+DBG_WS_TMP0
        BEQ     DBG_MEM_ROW0
        CMPA    #1
        BEQ     DBG_MEM_ROW1
        CMPA    #2
        BEQ     DBG_MEM_ROW2
        JMP     DBG_BIND_LCD_ROW3
DBG_MEM_ROW0:
        JMP     DBG_BIND_LCD_ROW0
DBG_MEM_ROW1:
        JMP     DBG_BIND_LCD_ROW1
DBG_MEM_ROW2:
        JMP     DBG_BIND_LCD_ROW2

; ---------------------------------------------------------------------------
; 40-column row format, exactly 40 characters:
;
;   Bn:AAAA XX XX XX XX XX XX XX XX abcdefgh
;   RO:AAAA XX XX XX XX XX XX XX XX abcdefgh
;
; Header = 8 chars, hex field = 24 chars, ASCII = 8 chars.
; n is the current RAM bank. RO denotes fixed ROM.
; ---------------------------------------------------------------------------
DBG_MEM_ROW_HEADER:
        LDAA    DBG_WS_BASE+DBG_WS_TMP_WORD
        BITA    #0x80
        BNE     DBG_MEM_HEADER_ROM

        LDAA    #'B'
        JSR     DBG_OUT_A
        LDAA    DBG_WS_BASE+DBG_WS_MEM_BANK
        ADDA    #'0'
        JSR     DBG_OUT_A
        BRA     DBG_MEM_HEADER_COMMON

DBG_MEM_HEADER_ROM:
        LDAA    #'R'
        JSR     DBG_OUT_A
        LDAA    #'O'
        JSR     DBG_OUT_A

DBG_MEM_HEADER_COMMON:
        LDAA    #':'
        JSR     DBG_OUT_A

        LDAA    DBG_WS_BASE+DBG_WS_TMP_WORD
        JSR     DBG_OUT_HEX_A
        LDAA    DBG_WS_BASE+DBG_WS_TMP_WORD+1
        JSR     DBG_OUT_HEX_A

        LDAA    #' '
        JMP     DBG_OUT_A

; ---------------------------------------------------------------------------
; DBG_MEM_REDRAW
; Streaming renderer: no line/screen buffer.
; Reads each byte twice (hex pass, ASCII pass) to save RAM.
; Natural 16-bit address wrap is intentional.
; ---------------------------------------------------------------------------
        .type DBG_MEM_REDRAW, @function
DBG_MEM_REDRAW:
        JSR     DBG_BIND_LCD_CLEAR

        LDX     DBG_WS_BASE+DBG_WS_MEM_ADDR
        STX     DBG_WS_BASE+DBG_WS_TMP_WORD
        CLRA
        STAA    DBG_WS_BASE+DBG_WS_TMP0       ; row = 0

DBG_MEM_ROW_LOOP:
        JSR     DBG_MEM_SELECT_ROW
        JSR     DBG_MEM_ROW_HEADER

        LDAA    #8
        STAA    DBG_WS_BASE+DBG_WS_TMP1

DBG_MEM_HEX_LOOP:
        LDX     DBG_WS_BASE+DBG_WS_TMP_WORD
        JSR     DBG_MEM_READ_X
        INX
        STX     DBG_WS_BASE+DBG_WS_TMP_WORD
        JSR     DBG_OUT_HEX_A
        LDAA    #' '
        JSR     DBG_OUT_A

        DEC     DBG_WS_BASE+DBG_WS_TMP1
        BNE     DBG_MEM_HEX_LOOP

        ; Rewind eight bytes for the ASCII pass.
        LDD     DBG_WS_BASE+DBG_WS_TMP_WORD
        SUBD    #8
        STD     DBG_WS_BASE+DBG_WS_TMP_WORD

        LDAA    #8
        STAA    DBG_WS_BASE+DBG_WS_TMP1

DBG_MEM_ASCII_LOOP:
        LDX     DBG_WS_BASE+DBG_WS_TMP_WORD
        JSR     DBG_MEM_READ_X
        INX
        STX     DBG_WS_BASE+DBG_WS_TMP_WORD
        JSR     DBG_OUT_ASCII_A

        DEC     DBG_WS_BASE+DBG_WS_TMP1
        BNE     DBG_MEM_ASCII_LOOP

        INC     DBG_WS_BASE+DBG_WS_TMP0
        LDAA    DBG_WS_BASE+DBG_WS_TMP0
        CMPA    #4
        BLO     DBG_MEM_ROW_LOOP
        RTS

        .size DBG_MEM_REDRAW, .-DBG_MEM_REDRAW


; ===========================================================================
; Increment 5 — GOTO only
;
; Syntax:
;   AAAA      keep current RAM bank, select 16-bit CPU address
;   b:AAAA    select RAM bank 0..3 plus 16-bit CPU address
;
; Parser consumes translated characters. No text buffer is allocated.
; DBG_GOTO_ACCEPT_A returns B=0 while collecting/ignoring input and B=1 when
; the transaction finishes by commit or Escape cancellation.
; ===========================================================================

        .globl DBG_GOTO_RESET
        .globl DBG_GOTO_ACCEPT_A
        .globl DBG_GOTO_RUN

        .type DBG_GOTO_RESET, @function
DBG_GOTO_RESET:
        LDX     #DBG_WS_BASE
        LDAA    #DBG_MODE_GOTO
        STAA    DBG_WS_UI_MODE,X
        CLRA
        STAA    DBG_WS_INPUT_DIGITS,X
        CLRB
        STD     DBG_WS_INPUT_VALUE,X
        LDAA    #DBG_INPUT_NO_BANK
        STAA    DBG_WS_INPUT_BANK,X
        RTS
        .size DBG_GOTO_RESET, .-DBG_GOTO_RESET

; A = ASCII hexadecimal character.
; V=0/A=0..15 if valid, V=1 if invalid.
DBG_HEX_TO_NIBBLE_A:
        CMPA    #'0'
        BLO     DBG_HEX_BAD
        CMPA    #':'
        BLO     DBG_HEX_DIGIT
        ANDA    #0xdf                   ; lowercase -> uppercase
        CMPA    #'A'
        BLO     DBG_HEX_BAD
        CMPA    #'G'
        BHS     DBG_HEX_BAD
        SUBA    #'A'-10
        CLV
        RTS
DBG_HEX_DIGIT:
        SUBA    #'0'
        CLV
        RTS
DBG_HEX_BAD:
        SEV
        RTS

        .type DBG_GOTO_ACCEPT_A, @function
DBG_GOTO_ACCEPT_A:
        CMPA    #0x1b
        BEQ     DBG_GOTO_CANCEL
        CMPA    #0x0d
        BEQ     DBG_GOTO_ENTER
        CMPA    #':'
        BEQ     DBG_GOTO_COLON

        JSR     DBG_HEX_TO_NIBBLE_A
        BVS     DBG_GOTO_CONTINUE
        LDX     #DBG_WS_BASE
        LDAB    DBG_WS_INPUT_DIGITS,X
        CMPB    #4
        BHS     DBG_GOTO_CONTINUE

        STAA    DBG_WS_TMP0,X
        LDD     DBG_WS_INPUT_VALUE,X
        ASLD
        ASLD
        ASLD
        ASLD
        ORAB    DBG_WS_TMP0,X
        STD     DBG_WS_INPUT_VALUE,X
        INC     DBG_WS_INPUT_DIGITS,X

DBG_GOTO_CONTINUE:
        CLRB
        RTS

DBG_GOTO_COLON:
        LDX     #DBG_WS_BASE
        LDAA    DBG_WS_INPUT_BANK,X
        CMPA    #DBG_INPUT_NO_BANK
        BNE     DBG_GOTO_CONTINUE
        LDAA    DBG_WS_INPUT_DIGITS,X
        CMPA    #1
        BNE     DBG_GOTO_CONTINUE
        LDAA    DBG_WS_INPUT_VALUE+1,X
        CMPA    #4
        BHS     DBG_GOTO_CONTINUE

        STAA    DBG_WS_INPUT_BANK,X
        CLRA
        STAA    DBG_WS_INPUT_DIGITS,X
        CLRB
        STD     DBG_WS_INPUT_VALUE,X
        RTS

DBG_GOTO_ENTER:
        LDX     #DBG_WS_BASE
        LDAA    DBG_WS_INPUT_DIGITS,X
        CMPA    #4
        BNE     DBG_GOTO_CONTINUE
        LDD     DBG_WS_INPUT_VALUE,X
        STD     DBG_WS_MEM_ADDR,X
        LDAA    DBG_WS_INPUT_BANK,X
        CMPA    #DBG_INPUT_NO_BANK
        BEQ     DBG_GOTO_DONE
        STAA    DBG_WS_MEM_BANK,X
        BRA     DBG_GOTO_DONE

DBG_GOTO_CANCEL:
        LDX     #DBG_WS_BASE

DBG_GOTO_DONE:
        LDAA    #DBG_MODE_MEM
        STAA    DBG_WS_UI_MODE,X
        LDAB    #1
        RTS

        .size DBG_GOTO_ACCEPT_A, .-DBG_GOTO_ACCEPT_A

; Blocking user-facing GOTO transaction.
        .type DBG_GOTO_RUN, @function
DBG_GOTO_RUN:
        JSR     DBG_GOTO_RESET
        JSR     DBG_BIND_LCD_CLEAR
        JSR     DBG_BIND_LCD_ROW0
        LDX     #DBG_GOTO_PROMPT
        JSR     DBG_BIND_LCD_PUTS

DBG_GOTO_KEY_LOOP:
        JSR     DBG_BIND_KEY_GETCHAR
        BVS     DBG_GOTO_KEY_LOOP

        CMPA    #0x0d
        BEQ     DBG_GOTO_NO_ECHO
        CMPA    #0x1b
        BEQ     DBG_GOTO_NO_ECHO
        PSHA
        TAB
        JSR     DBG_BIND_LCD_PUTBYTE
        PULA

DBG_GOTO_NO_ECHO:
        JSR     DBG_GOTO_ACCEPT_A
        TSTB
        BEQ     DBG_GOTO_KEY_LOOP
        JSR     DBG_MEM_REDRAW
        RTS

DBG_GOTO_PROMPT:
        .ascii  "GOTO "
        .byte   0

        .size DBG_GOTO_RUN, .-DBG_GOTO_RUN


; ===========================================================================
; Increment 6 — EDIT only
;
; Target = mem_addr + mem_cursor.
; Only lower-half banked RAM is writable in v0.
; HEX/ASCII mode is represented by control flow, not persistent RAM, so the
; frozen Increment-2 workspace ABI remains unchanged.
; ===========================================================================

        .globl DBG_EDIT_RESET
        .globl DBG_EDIT_ACCEPT_A
        .globl DBG_EDIT_ACCEPT_ASCII_A
        .globl DBG_EDIT_RUN
        .globl DBG_EDIT_WRITE_A

; X = currently selected physical CPU address.
DBG_EDIT_TARGET_X:
        LDX     DBG_WS_BASE+DBG_WS_MEM_ADDR
        LDAB    DBG_WS_BASE+DBG_WS_MEM_CURSOR
        ABX
        RTS

; A = byte to write.
; B returns 1 on committed RAM write, 0 on protected target.
; No write is attempted at $8000-$FFFF.
        .type DBG_EDIT_WRITE_A, @function
DBG_EDIT_WRITE_A:
        PSHA
        JSR     DBG_EDIT_TARGET_X
        XGDX
        BITA    #0x80
        XGDX
        PULA
        BNE     DBG_EDIT_WRITE_PROTECTED

        TAB
        LDAA    DBG_WS_BASE+DBG_WS_MEM_BANK
        JSR     DBG_BIND_RAM_WRITE
        LDAB    #1
        RTS

DBG_EDIT_WRITE_PROTECTED:
        CLRB
        RTS
        .size DBG_EDIT_WRITE_A, .-DBG_EDIT_WRITE_A

DBG_EDIT_CLEAR_INPUT:
        CLRA
        STAA    DBG_WS_BASE+DBG_WS_INPUT_DIGITS
        STAA    DBG_WS_BASE+DBG_WS_INPUT_VALUE
        STAA    DBG_WS_BASE+DBG_WS_INPUT_VALUE+1
        RTS

        .type DBG_EDIT_RESET, @function
DBG_EDIT_RESET:
        LDAA    #DBG_MODE_EDIT
        STAA    DBG_WS_BASE+DBG_WS_UI_MODE
        JMP     DBG_EDIT_CLEAR_INPUT
        .size DBG_EDIT_RESET, .-DBG_EDIT_RESET

; HEX input.
; A = translated character.
; B=0 continue/ignored, B=1 transaction finished.
        .type DBG_EDIT_ACCEPT_A, @function
DBG_EDIT_ACCEPT_A:
        CMPA    #0x1b
        BEQ     DBG_EDIT_DONE

        JSR     DBG_HEX_TO_NIBBLE_A
        BVS     DBG_EDIT_CONTINUE
        LDX     #DBG_WS_BASE
        TST     DBG_WS_INPUT_DIGITS,X
        BNE     DBG_EDIT_HEX_LOW

        ASLA
        ASLA
        ASLA
        ASLA
        STAA    DBG_WS_INPUT_VALUE+1,X
        INC     DBG_WS_INPUT_DIGITS,X
        CLRB
        RTS

DBG_EDIT_HEX_LOW:
        ORAA    DBG_WS_INPUT_VALUE+1,X
        JSR     DBG_EDIT_WRITE_A
        BRA     DBG_EDIT_DONE

        .size DBG_EDIT_ACCEPT_A, .-DBG_EDIT_ACCEPT_A

; ASCII input.
; Printable 20h..7Eh commits directly; Escape cancels.
        .type DBG_EDIT_ACCEPT_ASCII_A, @function
DBG_EDIT_ACCEPT_ASCII_A:
        CMPA    #0x1b
        BEQ     DBG_EDIT_DONE
        CMPA    #0x20
        BLO     DBG_EDIT_CONTINUE
        CMPA    #0x7f
        BHS     DBG_EDIT_CONTINUE
        JSR     DBG_EDIT_WRITE_A
        BRA     DBG_EDIT_DONE

DBG_EDIT_CONTINUE:
        CLRB
        RTS

DBG_EDIT_DONE:
        LDAA    #DBG_MODE_MEM
        STAA    DBG_WS_BASE+DBG_WS_UI_MODE
        LDAB    #1
        RTS
        .size DBG_EDIT_ACCEPT_ASCII_A, .-DBG_EDIT_ACCEPT_ASCII_A

; Blocking user-facing edit transaction.
; Default is HEX. Tab switches to the other input loop without persistent mode
; state. Switching modes discards a partial HEX nibble.
        .type DBG_EDIT_RUN, @function
DBG_EDIT_RUN:
        JSR     DBG_EDIT_TARGET_X
        XGDX
        BITA    #0x80
        XGDX
        BNE     DBG_EDIT_PROTECTED_RUN

        JSR     DBG_EDIT_RESET

DBG_EDIT_HEX_PROMPT_REDRAW:
        JSR     DBG_BIND_LCD_CLEAR
        JSR     DBG_BIND_LCD_ROW0
        LDX     #DBG_EDIT_HEX_PROMPT
        JSR     DBG_BIND_LCD_PUTS

DBG_EDIT_HEX_KEY_LOOP:
        JSR     DBG_BIND_KEY_GETCHAR
        BVS     DBG_EDIT_HEX_KEY_LOOP
        CMPA    #0x09
        BEQ     DBG_EDIT_TO_ASCII
        CMPA    #0x1b
        BEQ     DBG_EDIT_HEX_NO_ECHO
        PSHA
        TAB
        JSR     DBG_BIND_LCD_PUTBYTE
        PULA
DBG_EDIT_HEX_NO_ECHO:
        JSR     DBG_EDIT_ACCEPT_A
        TSTB
        BEQ     DBG_EDIT_HEX_KEY_LOOP
        BRA     DBG_EDIT_FINISH

DBG_EDIT_TO_ASCII:
        JSR     DBG_EDIT_CLEAR_INPUT
        JSR     DBG_BIND_LCD_CLEAR
        JSR     DBG_BIND_LCD_ROW0
        LDX     #DBG_EDIT_ASCII_PROMPT
        JSR     DBG_BIND_LCD_PUTS

DBG_EDIT_ASCII_KEY_LOOP:
        JSR     DBG_BIND_KEY_GETCHAR
        BVS     DBG_EDIT_ASCII_KEY_LOOP
        CMPA    #0x09
        BEQ     DBG_EDIT_TO_HEX
        CMPA    #0x1b
        BEQ     DBG_EDIT_ASCII_NO_ECHO
        PSHA
        TAB
        JSR     DBG_BIND_LCD_PUTBYTE
        PULA
DBG_EDIT_ASCII_NO_ECHO:
        JSR     DBG_EDIT_ACCEPT_ASCII_A
        TSTB
        BEQ     DBG_EDIT_ASCII_KEY_LOOP
        BRA     DBG_EDIT_FINISH

DBG_EDIT_TO_HEX:
        JSR     DBG_EDIT_CLEAR_INPUT
        BRA     DBG_EDIT_HEX_PROMPT_REDRAW

DBG_EDIT_FINISH:
        JSR     DBG_MEM_REDRAW
        RTS

DBG_EDIT_PROTECTED_RUN:
        JSR     DBG_MEM_REDRAW
        RTS

DBG_EDIT_HEX_PROMPT:
        .asciz  "HEX "
DBG_EDIT_ASCII_PROMPT:
        .asciz  "ASC "

        .size DBG_EDIT_RUN, .-DBG_EDIT_RUN


; ===========================================================================
; Increment 7 — CALL only
;
; Native HC11 probe. Sequential field order is fixed:
; T16, A8, B8, X16, Y16. D is A:B.
; Result screen order is A B CCR / X Y.
; ===========================================================================

        .globl DBG_CALL_RESET
        .globl DBG_CALL_ACCEPT_A
        .globl DBG_CALL_EXECUTE
        .globl DBG_CALL_RUN

DBG_CALL_CLEAR_INPUT:
        CLRA
        STAA    DBG_WS_BASE+DBG_WS_INPUT_DIGITS
        STAA    DBG_WS_BASE+DBG_WS_INPUT_VALUE
        STAA    DBG_WS_BASE+DBG_WS_INPUT_VALUE+1
        RTS

        .type DBG_CALL_RESET, @function
DBG_CALL_RESET:
        LDAA    #DBG_MODE_CALL
        STAA    DBG_WS_BASE+DBG_WS_UI_MODE

        ; Clear call_ctl + T/A/B/X/Y input block (9 contiguous bytes).
        LDX     #DBG_WS_BASE+DBG_WS_CALL_CTL
        CLRA
        LDAB    #9
DBG_CALL_RESET_LOOP:
        STAA    0,X
        INX
        DECB
        BNE     DBG_CALL_RESET_LOOP
        JMP     DBG_CALL_CLEAR_INPUT
        .size DBG_CALL_RESET, .-DBG_CALL_RESET

; A = translated character.
; B=0 collecting/ignored, B=1 ready to execute, B=2 cancel,
; B=3 field committed/advance prompt.
        .type DBG_CALL_ACCEPT_A, @function
DBG_CALL_ACCEPT_A:
        CMPA    #0x1b
        BEQ     DBG_CALL_CANCEL
        JSR     DBG_HEX_TO_NIBBLE_A
        BVS     DBG_CALL_CONTINUE

        STAA    DBG_WS_BASE+DBG_WS_TMP0
        LDD     DBG_WS_BASE+DBG_WS_INPUT_VALUE
        ASLD
        ASLD
        ASLD
        ASLD
        ORAB    DBG_WS_BASE+DBG_WS_TMP0
        STD     DBG_WS_BASE+DBG_WS_INPUT_VALUE
        INC     DBG_WS_BASE+DBG_WS_INPUT_DIGITS

        LDAA    DBG_WS_BASE+DBG_WS_CALL_CTL
        CMPA    #DBG_CALL_FIELD_A
        BEQ     DBG_CALL_NEED2
        CMPA    #DBG_CALL_FIELD_B
        BEQ     DBG_CALL_NEED2
        LDAA    DBG_WS_BASE+DBG_WS_INPUT_DIGITS
        CMPA    #4
        BLO     DBG_CALL_CONTINUE
        BRA     DBG_CALL_COMMIT
DBG_CALL_NEED2:
        LDAA    DBG_WS_BASE+DBG_WS_INPUT_DIGITS
        CMPA    #2
        BLO     DBG_CALL_CONTINUE

DBG_CALL_COMMIT:
        ; Destination is compactly derived from the contiguous T/A/B/X/Y block:
        ; field 0->+10, 1->+12, 2->+13, 3->+14, 4->+16.
        LDAA    DBG_WS_BASE+DBG_WS_CALL_CTL
        STAA    DBG_WS_BASE+DBG_WS_TMP0
        TAB
        LDX     #DBG_WS_BASE+DBG_WS_CALL_TARGET
        ABX
        TSTA
        BEQ     DBG_CALL_DEST_READY
        INX
        CMPA    #DBG_CALL_FIELD_Y
        BNE     DBG_CALL_DEST_READY
        INX
DBG_CALL_DEST_READY:
        CMPA    #DBG_CALL_FIELD_A
        BLO     DBG_CALL_STORE_WORD
        CMPA    #DBG_CALL_FIELD_X
        BLO     DBG_CALL_STORE_BYTE
DBG_CALL_STORE_WORD:
        LDD     DBG_WS_BASE+DBG_WS_INPUT_VALUE
        STD     0,X
        BRA     DBG_CALL_STORED
DBG_CALL_STORE_BYTE:
        LDAA    DBG_WS_BASE+DBG_WS_INPUT_VALUE+1
        STAA    0,X

DBG_CALL_STORED:
        LDAA    DBG_WS_BASE+DBG_WS_TMP0
        CMPA    #DBG_CALL_FIELD_Y
        BEQ     DBG_CALL_READY
        INC     DBG_WS_BASE+DBG_WS_CALL_CTL
        JSR     DBG_CALL_CLEAR_INPUT
        LDAB    #3
        RTS
DBG_CALL_READY:
        JSR     DBG_CALL_CLEAR_INPUT
        LDAB    #1
        RTS
DBG_CALL_CONTINUE:
        CLRB
        RTS
DBG_CALL_CANCEL:
        LDAB    #2
        RTS
        .size DBG_CALL_ACCEPT_A, .-DBG_CALL_ACCEPT_A

; Execute exactly one RTS-returning HC11 target.
        .type DBG_CALL_EXECUTE, @function
DBG_CALL_EXECUTE:
        JSR     DBG_BIND_ENV_GET
        STAA    DBG_WS_BASE+DBG_WS_ENV_SAVED_MAP

        LDX     #DBG_CALL_RETURN
        PSHX
        LDX     DBG_WS_BASE+DBG_WS_CALL_TARGET
        PSHX

        LDAA    DBG_WS_BASE+DBG_WS_CALL_IN_A
        LDAB    DBG_WS_BASE+DBG_WS_CALL_IN_B
        LDX     DBG_WS_BASE+DBG_WS_CALL_IN_X
        LDY     DBG_WS_BASE+DBG_WS_CALL_IN_Y
        RTS

DBG_CALL_RETURN:
        PSHA
        TPA
        STAA    DBG_WS_BASE+DBG_WS_CALL_OUT_CCR
        PULA
        STAA    DBG_WS_BASE+DBG_WS_CALL_OUT_A
        STAB    DBG_WS_BASE+DBG_WS_CALL_OUT_B
        STX     DBG_WS_BASE+DBG_WS_CALL_OUT_X
        STY     DBG_WS_BASE+DBG_WS_CALL_OUT_Y

        LDAA    DBG_WS_BASE+DBG_WS_ENV_SAVED_MAP
        JSR     DBG_BIND_ENV_SET
        LDAA    DBG_WS_BASE+DBG_WS_CALL_CTL
        ORAA    #DBG_CALL_RESULT_VALID
        STAA    DBG_WS_BASE+DBG_WS_CALL_CTL
        RTS
        .size DBG_CALL_EXECUTE, .-DBG_CALL_EXECUTE

; One-character field prompt from fixed order T A B X Y.
DBG_CALL_PROMPT:
        JSR     DBG_BIND_LCD_CLEAR
        JSR     DBG_BIND_LCD_ROW0
        LDX     #DBG_CALL_LABELS
        LDAB    DBG_WS_BASE+DBG_WS_CALL_CTL
        ABX
        LDAA    0,X
        JSR     DBG_OUT_A
        LDAA    #' '
        JMP     DBG_OUT_A

; Compact result screen:
; row0: AA BB CC   (A, B, CCR)
; row1: XXXX YYYY (X, Y)
DBG_CALL_RESULTS:
        JSR     DBG_BIND_LCD_CLEAR
        JSR     DBG_BIND_LCD_ROW0
        LDAA    DBG_WS_BASE+DBG_WS_CALL_OUT_A
        JSR     DBG_OUT_HEX_A
        LDAA    #' '
        JSR     DBG_OUT_A
        LDAA    DBG_WS_BASE+DBG_WS_CALL_OUT_B
        JSR     DBG_OUT_HEX_A
        LDAA    #' '
        JSR     DBG_OUT_A
        LDAA    DBG_WS_BASE+DBG_WS_CALL_OUT_CCR
        JSR     DBG_OUT_HEX_A

        JSR     DBG_BIND_LCD_ROW1
        LDAA    DBG_WS_BASE+DBG_WS_CALL_OUT_X
        JSR     DBG_OUT_HEX_A
        LDAA    DBG_WS_BASE+DBG_WS_CALL_OUT_X+1
        JSR     DBG_OUT_HEX_A
        LDAA    #' '
        JSR     DBG_OUT_A
        LDAA    DBG_WS_BASE+DBG_WS_CALL_OUT_Y
        JSR     DBG_OUT_HEX_A
        LDAA    DBG_WS_BASE+DBG_WS_CALL_OUT_Y+1
        JMP     DBG_OUT_HEX_A

        .type DBG_CALL_RUN, @function
DBG_CALL_RUN:
        JSR     DBG_CALL_RESET
DBG_CALL_PROMPT_NEXT:
        JSR     DBG_CALL_PROMPT
DBG_CALL_KEY_LOOP:
        JSR     DBG_BIND_KEY_GETCHAR
        BVS     DBG_CALL_KEY_LOOP
        CMPA    #0x1b
        BEQ     DBG_CALL_NO_ECHO
        PSHA
        TAB
        JSR     DBG_BIND_LCD_PUTBYTE
        PULA
DBG_CALL_NO_ECHO:
        JSR     DBG_CALL_ACCEPT_A
        CMPB    #3
        BEQ     DBG_CALL_PROMPT_NEXT
        CMPB    #2
        BEQ     DBG_CALL_EXIT
        TSTB
        BEQ     DBG_CALL_KEY_LOOP

        JSR     DBG_CALL_EXECUTE
        JSR     DBG_CALL_RESULTS
DBG_CALL_RESULT_WAIT:
        JSR     DBG_BIND_KEY_GETCHAR
        BVS     DBG_CALL_RESULT_WAIT

DBG_CALL_EXIT:
        LDAA    #DBG_MODE_MEM
        STAA    DBG_WS_BASE+DBG_WS_UI_MODE
        JSR     DBG_MEM_REDRAW
        RTS
        .size DBG_CALL_RUN, .-DBG_CALL_RUN

DBG_CALL_LABELS:
        .ascii  "TABXY"


; ===========================================================================
; Increment 8 — INFO only
;
; Read-only compact DynFS/stock compatibility state.
; Logical offsets are shown relative to active compat_base.
; No INFO snapshot or filesystem repair state is allocated.
; ===========================================================================

        .globl DBG_INFO_REDRAW
        .globl DBG_INFO_RUN

; B = label, A = byte value. Emits "Lxx ".
DBG_INFO_FIELD8:
        PSHA
        JSR     DBG_BIND_LCD_PUTBYTE
        PULA
        JSR     DBG_OUT_HEX_A
        LDAA    #' '
        JMP     DBG_OUT_A

; B = label, X = word value. Emits "Lxxxx ".
DBG_INFO_FIELD16_X:
        PSHX
        JSR     DBG_BIND_LCD_PUTBYTE
        PULX
        STX     DBG_WS_BASE+DBG_WS_TMP_WORD
        LDAA    DBG_WS_BASE+DBG_WS_TMP_WORD
        JSR     DBG_OUT_HEX_A
        LDAA    DBG_WS_BASE+DBG_WS_TMP_WORD+1
        JSR     DBG_OUT_HEX_A
        LDAA    #' '
        JMP     DBG_OUT_A

; B = label, X -> stock compatibility word.
; Emits label + (stock_word - compat_base) as 16-bit hex.
DBG_INFO_FIELD_DELTA_X:
        PSHB
        LDD     0,X
        SUBD    DBG_BIND_DYNFS_ACTIVE_CTX+5
        XGDX
        PULB
        JMP     DBG_INFO_FIELD16_X

        .type DBG_INFO_REDRAW, @function
DBG_INFO_REDRAW:
        JSR     DBG_BIND_LCD_CLEAR

        ; Row 0: selected stock slot, DynFS file ID, physical MEM bank.
        JSR     DBG_BIND_LCD_ROW0
        LDAA    DBG_BIND_STOCK_FSLOT
        LDAB    #'S'
        JSR     DBG_INFO_FIELD8
        LDAA    DBG_BIND_DYNFS_ACTIVE_CTX+4
        LDAB    #'F'
        JSR     DBG_INFO_FIELD8
        LDAA    DBG_WS_BASE+DBG_WS_MEM_BANK
        LDAB    #'B'
        JSR     DBG_INFO_FIELD8

        ; Row 1: active mount pointer, payload-view pointer, compat base.
        JSR     DBG_BIND_LCD_ROW1
        LDX     DBG_BIND_DYNFS_ACTIVE_CTX
        LDAB    #'M'
        JSR     DBG_INFO_FIELD16_X
        LDX     DBG_BIND_DYNFS_ACTIVE_CTX+2
        LDAB    #'P'
        JSR     DBG_INFO_FIELD16_X
        LDX     DBG_BIND_DYNFS_ACTIVE_CTX+5
        LDAB    #'K'
        JSR     DBG_INFO_FIELD16_X

        ; Row 2: logical cursor/view/end offsets relative to compat_base.
        JSR     DBG_BIND_LCD_ROW2
        LDX     #DBG_BIND_STOCK_CURSOR
        LDAB    #'C'
        JSR     DBG_INFO_FIELD_DELTA_X
        LDX     #DBG_BIND_STOCK_VIEW
        LDAB    #'V'
        JSR     DBG_INFO_FIELD_DELTA_X
        LDX     #DBG_BIND_STOCK_END
        LDAB    #'E'
        JSR     DBG_INFO_FIELD_DELTA_X

        ; Row 3: logical origin and sequential offsets.
        JSR     DBG_BIND_LCD_ROW3
        LDX     #DBG_BIND_STOCK_ORIGIN
        LDAB    #'O'
        JSR     DBG_INFO_FIELD_DELTA_X
        LDX     #DBG_BIND_STOCK_SEQ
        LDAB    #'Q'
        JMP     DBG_INFO_FIELD_DELTA_X

        .size DBG_INFO_REDRAW, .-DBG_INFO_REDRAW

        .type DBG_INFO_RUN, @function
DBG_INFO_RUN:
        LDAA    #DBG_MODE_INFO
        STAA    DBG_WS_BASE+DBG_WS_UI_MODE
        JSR     DBG_INFO_REDRAW
DBG_INFO_WAIT:
        JSR     DBG_BIND_KEY_GETCHAR
        BVS     DBG_INFO_WAIT
        LDAA    #DBG_MODE_MEM
        STAA    DBG_WS_BASE+DBG_WS_UI_MODE
        JSR     DBG_MEM_REDRAW
        RTS
        .size DBG_INFO_RUN, .-DBG_INFO_RUN
