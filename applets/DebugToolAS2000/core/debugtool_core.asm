; DebugTool-AS2000 portable core skeleton.
; Increment 3 contains NO functional MEM/EDIT/GOTO/CALL/INFO implementation.
; No absolute hardware, ROM, RAM, or DynFS addresses are permitted here.

        .include "debugtool_state.inc"
        .include "debugtool_bindings.inc"

        .text
        .globl DEBUGTOOL_ENTRY
        .type DEBUGTOOL_ENTRY, @function

DEBUGTOOL_ENTRY:
        ; Functional session initialization starts in Increment 4 with MEM.
        ; This RTS exists only to establish the relocatable core entry ABI.
        RTS

        .size DEBUGTOOL_ENTRY, .-DEBUGTOOL_ENTRY
