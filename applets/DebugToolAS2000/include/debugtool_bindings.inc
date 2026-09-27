; DebugTool-AS2000 binding contract.
; Portable core code may reference ONLY these environment symbols.
; Their concrete addresses/implementations belong to one binding object.

        .extern DBG_WS_BASE

        .extern DBG_BIND_LCD_CLEAR
        .extern DBG_BIND_LCD_PUTS
        .extern DBG_BIND_LCD_PUTBYTE
        .extern DBG_BIND_LCD_COMMAND
        .extern DBG_BIND_LCD_ROW0
        .extern DBG_BIND_LCD_ROW1
        .extern DBG_BIND_LCD_ROW2
        .extern DBG_BIND_LCD_ROW3

        .extern DBG_BIND_BYTE_TO_HEX
        .extern DBG_BIND_KEY_DEQUEUE

        ; Mapping adapter contract, implementation intentionally late-bound.
        ; ENTER_RAM receives requested bank in A (0..3).
        ; It must make banked RAM visible and preserve enough environment
        ; state for RESTORE to reinstate the prior mapping.
        .extern DBG_BIND_RAM_ENTER
        .extern DBG_BIND_RAM_RESTORE

        ; Stock/DynFS compatibility-state references.
        .extern DBG_BIND_STOCK_FSLOT
        .extern DBG_BIND_STOCK_VIEW
        .extern DBG_BIND_STOCK_CURSOR
        .extern DBG_BIND_STOCK_END
        .extern DBG_BIND_STOCK_CAP
        .extern DBG_BIND_STOCK_ORIGIN
        .extern DBG_BIND_STOCK_SEQ

        ; 7-byte active DynFS compatibility context.
        .extern DBG_BIND_DYNFS_ACTIVE_CTX
