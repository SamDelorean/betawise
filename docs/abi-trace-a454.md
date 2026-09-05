# SYS_A454 / index 277 — ABI trace

Status: **MECANICA_RECONSTRUIDA A / SOURCE_FIRST / PUBLICADO**.

## Source correlation and availability

`betawise-master/os3k/syscall.c` maps index 277 to neutral placeholder `SYS_A454`; `os3k.h` contains no independent vendor prototype.

- AlphaSmart 3000 (2005): slot 277 is `0xF4070301`; no callable contract is claimed.
- NEO (2005): slot 277 is `0xF4070301`; no callable contract is claimed.
- NEO (2013): slot 277 resolves to runtime `0x00434980`.

The vector is non-monotonic here: A454 lies below A450 in runtime address, so neighboring vector order is not used as a handler-boundary assumption.

## Confirmed mechanics

NEO 2013 handler: `0x00434980..0x004349BD`, length `0x3E`, SHA-256 `426efdc938c9aa2a946de992ff1b1346aff2775ad186fd1336ce4526bb0e5c37`.

The four 32-bit physical slots are consumed as follows:

1. slot 1: selector low byte; only values 1 through 8 invoke the helper, and the helper receives selector-1;
2. slot 2: full 32-bit value, stored verbatim;
3. slot 3: only its low byte ultimately affects state;
4. slot 4: only its low byte ultimately affects state.

Helper `0x00432852` is `0x34` bytes, SHA-256 `199711fadffa5fb2a55617e0716ad8e96330ee6ba67987bc17ac01a712e84d27`. It addresses one of eight 8-byte records rooted at RAM `0x00007E52` and stores slot3.low8 zero-extended as a word at +0, slot4.low8 zero-extended as a word at +2, and slot2 as a longword at +4.

A454 does **not** preserve or use A1, independently refuting automatic continuation of the six-byte A1 aggregate convention from A43C–A450.

## Callers and return boundary

The official SmartApplet corpus contains three executable A454 callers: one in Responder and two in Wireless File Transfer, with observed tuples `(1, pointer, 0, 2)`, `(1, pointer, 0, 11)`, and `(2, pointer, 0, 2)`. NEO 2013 firmware contributes one additional direct BSR caller at `0x00433B70`. All recovered callers discard D0.

For valid selectors the helper leaves a mechanically observable D0 value, but the invalid-selector path does not initialize D0. Therefore no uniform scalar return contract is published.

## Regression and publication boundary

- Earlier full static regression: **74/74 PASS**.
- Fresh canonical-ROM directed regression on 2026-09-05: **23/23 PASS**.
- Dynamic validation: **specified, not executed**.

The former source-identity/return/table-semantic block is reclassified as an **evidence limit, not a sequencing blocker**. No vendor function name, table-field names, enum names or callable C prototype are asserted. ROM bytes and extended disassembly remain private.
