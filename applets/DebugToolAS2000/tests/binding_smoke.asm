; Link-time contract smoke test.
; Not part of the product image.
        .include "debugtool_state.inc"
        .include "debugtool_bindings.inc"

        .section .bindcheck,"a"
        .word DBG_WS_BASE
        .word DBG_BIND_LCD_CLEAR
        .word DBG_BIND_LCD_PUTS
        .word DBG_BIND_LCD_PUTBYTE
        .word DBG_BIND_LCD_COMMAND
        .word DBG_BIND_LCD_ROW0
        .word DBG_BIND_LCD_ROW1
        .word DBG_BIND_LCD_ROW2
        .word DBG_BIND_LCD_ROW3
        .word DBG_BIND_BYTE_TO_HEX
        .word DBG_BIND_KEY_DEQUEUE
        .word DBG_BIND_RAM_ENTER
        .word DBG_BIND_RAM_RESTORE
        .word DBG_BIND_STOCK_FSLOT
        .word DBG_BIND_STOCK_VIEW
        .word DBG_BIND_STOCK_CURSOR
        .word DBG_BIND_STOCK_END
        .word DBG_BIND_STOCK_CAP
        .word DBG_BIND_STOCK_ORIGIN
        .word DBG_BIND_STOCK_SEQ
        .word DBG_BIND_DYNFS_ACTIVE_CTX

        ; Force assembler-time agreement with the frozen RAM ledger.
        .if DBG_WS_SIZE-30
        .error "DBG_WS_SIZE must remain 30 bytes in Increment 3"
        .endif
