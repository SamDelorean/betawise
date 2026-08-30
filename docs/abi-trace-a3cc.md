# Forward-sweep trace — A3CC / index 243

- Status: mechanically reconstructed A; blocked exclusively on contractual return intent.
- AS3000 2005 raw slot: `0x5A0E004E`, non-pointer/unresolved.
- NEO 2005 raw slot: `0x7FC0005E`, non-pointer/unresolved.
- NEO 2013 handler: runtime `0x0043EFE8`, file `0x02EFE8`, length `0xD6`, 72 decoded instructions, SHA-256 `460e37673239f9d7fcc4feaaf0a606350f724c411bb0447e5c8076e11ef3ca4b`.
- Physical input ABI: zero caller arguments.
- Direct dependencies include A3B8 twice and A3B4 twice.
- Mechanical terminal D0.B domain: `{0,1}`; no wider return contract is implied.
- Firmware direct callers: 3; demonstrated return consumption: 0/3.
- Official SmartApplet corpus: 41/41 structurally checked; A3CC is not present as an A-line slot, callers 0/41.
- `uint8_t SYS_A3CC(void)` remains a strong inference only; no callable prototype is published.
- Static regression: OVERALL PASS. Dynamic regression: specified, not executed.

Public disposition: [`sys-a3cc-blocked.md`](sys-a3cc-blocked.md).

Detailed firmware/disassembly evidence remains private in Drive.
