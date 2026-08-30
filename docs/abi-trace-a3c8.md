# Forward-sweep trace — A3C8 / index 242

- Status: mechanically reconstructed A; blocked on contractual return and semantic typing of its sole 32-bit input slot.
- AS3000 2005 raw slot: `0x5A0E004E`, non-pointer/unresolved.
- NEO 2005 raw slot: `0x7F10005E`, non-pointer/unresolved.
- NEO 2013 handler: runtime `0x0043DAFC`, file `0x02DAFC`, length `0x20`, SHA-256 `3b6f4e37605adf639cd0cd953ddd369ff9715f084343030cdc1d85566933fe4d`.
- One full 32-bit physical stack slot is read and stored verbatim on the active path; semantic C type is unknown because there are no executable callers.
- Private helper: length `0x76`, SHA-256 `fd2f78590e0ce14ea54354aaa6b7cb11ce5da979c169b06199c3820dfcfd062e`, 23 decoded instructions, closed CFG.
- D0 is not uniform across A3C8 paths: one path leaves incoming D0 untouched; the other preserves the final helper-chain D0. No return contract is inferred.
- Firmware direct callers: 0. Official corpus: 41/41, executable callers 0.
- Static regression: OVERALL PASS. Dynamic regression: not executed.

Public disposition: [`sys-a3c8-blocked.md`](sys-a3c8-blocked.md).

Detailed firmware/helper evidence remains private in Drive.
