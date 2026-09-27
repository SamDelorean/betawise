; Diagnostic/MAME binding for AlphaSmart 2000 v3.1.4.
; Increment 4 provides the real RAM enter/restore adapter used by MEM.
; Workspace/context addresses remain linker-selected and are not resident
; placement decisions.

        .include "debugtool_state.inc"

        .globl DBG_BIND_LCD_CLEAR
        .set DBG_BIND_LCD_CLEAR,     0xA3D6
        .globl DBG_BIND_LCD_PUTS
        .set DBG_BIND_LCD_PUTS,      0xA435
        .globl DBG_BIND_LCD_PUTBYTE
        .set DBG_BIND_LCD_PUTBYTE,   0xA44B
        .globl DBG_BIND_LCD_COMMAND
        .set DBG_BIND_LCD_COMMAND,   0xA440

        ; v3.1.4 stock display traversal order: physical rows 0..3.
        .globl DBG_BIND_LCD_ROW0
        .set DBG_BIND_LCD_ROW0,      0xA54B
        .globl DBG_BIND_LCD_ROW1
        .set DBG_BIND_LCD_ROW1,      0xA556
        .globl DBG_BIND_LCD_ROW2
        .set DBG_BIND_LCD_ROW2,      0xA561
        .globl DBG_BIND_LCD_ROW3
        .set DBG_BIND_LCD_ROW3,      0xA56C

        .globl DBG_BIND_BYTE_TO_HEX
        .set DBG_BIND_BYTE_TO_HEX,   0x9350
        .globl DBG_BIND_KEY_DEQUEUE
        .set DBG_BIND_KEY_DEQUEUE,   0x938C

        .globl DBG_BIND_STOCK_FSLOT
        .set DBG_BIND_STOCK_FSLOT,   0x018E
        .globl DBG_BIND_STOCK_VIEW
        .set DBG_BIND_STOCK_VIEW,    0x0120
        .globl DBG_BIND_STOCK_CURSOR
        .set DBG_BIND_STOCK_CURSOR,  0x0122
        .globl DBG_BIND_STOCK_END
        .set DBG_BIND_STOCK_END,     0x0124
        .globl DBG_BIND_STOCK_CAP
        .set DBG_BIND_STOCK_CAP,     0x0126
        .globl DBG_BIND_STOCK_ORIGIN
        .set DBG_BIND_STOCK_ORIGIN,  0x0128
        .globl DBG_BIND_STOCK_SEQ
        .set DBG_BIND_STOCK_SEQ,     0x0067

        ; Synthetic link-only storage. Increment 10 assigns proven addresses.
        .bss
        .balign 1
        .globl DBG_WS_BASE
DBG_WS_BASE:
        .space DBG_WS_SIZE

        .globl DBG_BIND_DYNFS_ACTIVE_CTX
DBG_BIND_DYNFS_ACTIVE_CTX:
        .space 7

        .text

; Input A = RAM bank 0..3.
; X is preserved. Y is scratch.
;
; To remain independent of workspace visibility while another RAM bank is
; selected, CCR and PORTA are carried in a two-byte hidden stack frame across
; the ENTER/RESTORE pair. Interrupts are masked before changing the view and
; the exact incoming CCR is restored after PORTA is restored.
        .globl DBG_BIND_RAM_ENTER
DBG_BIND_RAM_ENTER:
        TAB
        PULY                    ; remove our return address temporarily
        TPA
        PSHA                    ; hidden saved CCR
        LDAA    0x0000
        PSHA                    ; hidden saved PORTA
        PSHY                    ; restore return address above hidden frame

        ANDA    #0x8f          ; clear PA6:PA4
        ORAA    #0x40          ; RAM view
        ASLB
        ASLB
        ASLB
        ASLB
        ANDB    #0x30
        ABA
        SEI
        STAA    0x0000
        RTS

        .globl DBG_BIND_RAM_RESTORE
DBG_BIND_RAM_RESTORE:
        PULY                    ; matching caller return
        PULA                    ; saved PORTA
        STAA    0x0000
        PULA                    ; saved CCR
        TAP
        PSHY
        RTS
