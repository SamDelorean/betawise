; Diagnostic/MAME skeleton binding for AlphaSmart 2000 v3.1.4.
;
; VERIFIED stock services/states are concrete aliases.
; RAM adapter and DynFS active context are link stubs/placeholders ONLY in
; Increment 3. This object is not yet a runnable DebugTool integration.

        .globl DBG_BIND_LCD_CLEAR
        .set DBG_BIND_LCD_CLEAR,     0xA3D6
        .globl DBG_BIND_LCD_PUTS
        .set DBG_BIND_LCD_PUTS,      0xA435
        .globl DBG_BIND_LCD_PUTBYTE
        .set DBG_BIND_LCD_PUTBYTE,   0xA44B
        .globl DBG_BIND_LCD_COMMAND
        .set DBG_BIND_LCD_COMMAND,   0xA440

        ; Row mapping is still candidate-level; aliases are isolated here.
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

        ; Synthetic link-only storage: linker chooses actual addresses.
        .bss
        .balign 1
        .globl DBG_WS_BASE
DBG_WS_BASE:
        .space 30

        .globl DBG_BIND_DYNFS_ACTIVE_CTX
DBG_BIND_DYNFS_ACTIVE_CTX:
        .space 7

        ; Mapping semantics remain intentionally unresolved until the adapter
        ; is measured. These RTS stubs exist solely to prove core/link binding.
        .text
        .globl DBG_BIND_RAM_ENTER
DBG_BIND_RAM_ENTER:
        RTS

        .globl DBG_BIND_RAM_RESTORE
DBG_BIND_RAM_RESTORE:
        RTS
