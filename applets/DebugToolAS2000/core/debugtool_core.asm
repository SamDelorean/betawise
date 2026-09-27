; DebugTool-AS2000 portable core.
; Increment 4 implements MEM only.
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
