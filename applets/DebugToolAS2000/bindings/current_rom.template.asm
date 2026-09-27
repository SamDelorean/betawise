; Current-ROM binding template.
; DO NOT fill guessed addresses.
; Increment 10 owns these final assignments after RAM/ROM proof.
;
; Copy the verified service aliases from diagnostic_mame.asm only after the
; current patched ROM is frozen, then bind:
;   DBG_WS_BASE
;   DBG_BIND_RAM_ENTER / DBG_BIND_RAM_RESTORE
;   DBG_BIND_DYNFS_ACTIVE_CTX
; and any stock helper whose address changed.
;
; The portable core must not be edited to accommodate this binding.
