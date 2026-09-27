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

        ; Converts one stock raw keyboard-queue code in A to the firmware's
        ; translated character in A. V=0 on a character, V=1 when the raw key
        ; has no character translation.
        .extern DBG_BIND_KEY_CHAR

        ; Complete non-blocking stock keyboard poll:
        ; scan/pump hardware, dequeue one raw key, translate to character.
        ; Returns A=translated character with V=0, or V=1 if none/invalid.
        .extern DBG_BIND_KEY_GETCHAR

        ; Immediate, non-nestable RAM mapping pair used by MEM/EDIT.
        ;
        ; DBG_BIND_RAM_ENTER:
        ;   input A = requested bank 0..3
        ;   preserves X
        ;   may clobber A/B/Y/CCR
        ;   makes lower-half RAM visible and leaves a private mapping frame on
        ;   the internal CPU stack for the matching RESTORE.
        ;
        ; DBG_BIND_RAM_RESTORE:
        ;   must be the matching next mapping operation;
        ;   preserves B and X;
        ;   restores the pre-ENTER mapping and CCR and removes the private
        ;   mapping frame.
        ;
        ; Portable code must not access DebugTool workspace between ENTER and
        ; RESTORE because that workspace may itself live in banked memory.
        .extern DBG_BIND_RAM_ENTER
        .extern DBG_BIND_RAM_RESTORE

        ; Atomic banked-RAM byte write used by EDIT.
        ; input A = bank 0..3, B = byte value, X = lower-half CPU address.
        ; May clobber A/X/Y/CCR. The mapping is restored before return.
        .extern DBG_BIND_RAM_WRITE

        ; CALL environment mapping snapshot/restore.
        ; GET returns the complete current mapping byte in A.
        ; SET receives the mapping byte in A and restores it.
        .extern DBG_BIND_ENV_GET
        .extern DBG_BIND_ENV_SET

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
