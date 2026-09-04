# OS3K A370 / index 220 — `rand` closure

Status: **mechanically closed / confidence A**.

## Reconstructed contract

```c
int rand(void);
```

The three canonical System 3 ROMs implement the same 32-bit linear-congruential generator. A370 reads no arguments, updates one persistent 32-bit state as

```text
state = state * 0x41C64E6D + 0x3039   (mod 2^32)
```

and returns `(state >> 16) & 0x7FFF` in `D0.L`, i.e. the mechanically demonstrated range is `0..32767`.

## Cross-ROM evidence

- AS3000 2005: runtime `0x004E71F8`, file `+0x0271F8`.
- NEO 2005: runtime `0x005E9540`, file `+0x029540`.
- NEO 2013: runtime `0x00444414`, file `+0x034414`.
- Handler length: `0x28` bytes in all three images, terminal `RTS` at `+0x26`.
- Raw handler SHA-256: AS `022a468d0b899770fc57da78beb2b09216aa875fdc2cfbf8f0bf4e5488de4015`; NEO05 `34054716813a14afd8bc9966e2b31360ba7cbcecdda7629c1b16cae08ea027e9`; NEO13 `81a4f966d25ec94b3ee16c682500cb128d651bd36ca31f07ec4de46a256be80d`.
- After neutralizing only the relocated state-global operand and multiply-helper target, all three normalize to SHA-256 `364c91b9e81c26bc51de9681723945c481e57179758d19b18400ff7362d3eef0`.

The multiply helper is the equivalent 32-bit product routine in each generation, implemented from 16-bit partial products. No platform-specific change in the PRNG recurrence or return transform was found.

## Seed-state correlation

The immediately adjacent A37C / index 223 handler (not closed as part of this block) writes its one 32-bit argument directly to the exact same state global and returns. That primary mechanical relationship is the expected seed setter for A370 and corroborates the historical `rand` / `srand` mapping. A374 / index 221 is **not** the seed setter; it is a separate noncontiguous block historically mapped as `scanf`.

## Callers and adversarial checks

The official SmartApplet corpus was exhausted: 30 table-bearing applets were hash-verified and scanned with the corrected PC-index detector; 11 further applets are structural negatives without the relevant table. A370 has **0 executable callers in 41/41 applets**. ROM-wide direct JSR/JMP/BSR searches are also negative. The same detector finds 99 A36C calls and 598 A378 calls, so the negative A370 result is not detector blindness.

Alternatives were rejected as follows: A370 does not return the raw state (the high half is shifted and masked), does not consume a seed argument, and is not a generic unknown status helper because the exact LCG recurrence, 15-bit result construction, shared state with the adjacent seed setter, and established C-library slot mapping all converge on `rand`.

## Regression

A private reproducible static regression revalidates the three ROM hashes, handler hashes and boundaries, state globals, multiplication helper operands, LCG constants, output transform, cross-ROM normalization, and A37C shared-state seed setter. Result: **OVERALL PASS**. Emulator-first dynamic regression is specified but has not been executed.

Extended ROM bytes, full disassemblies, and caller workpapers remain private in Drive.

## SOURCE-FIRST re-audit — 2026-09-04

The historical source layer was consulted first: `os3k/syscall.c` maps index 220 to `rand`, and `os3k/os3k.h` declares `int rand(void)`. These names were used only as hypotheses. Primary firmware evidence independently reconfirms the contract: zero arguments, a persistent 32-bit state, the exact recurrence `state = state * 0x41C64E6D + 0x3039`, and an explicit return transform `(state >> 16) & 0x7FFF`.

The adjacent A37C handler strengthens the correlation without being used as a naming shortcut: it writes one full 32-bit argument to the exact same PRNG state global. This also refutes the earlier transient possibility that A374 might be the seed setter. The negative caller result remains well controlled: A370 has 0 executable calls in the complete 41-app corpus and no direct ROM xrefs, while the same detector finds known-positive neighboring calls. No source/firmware contradiction was found.

Classification after re-audit: **CLOSED A / SOURCE_FIRST / PUBLISHED**. Previously executed static regression remains `OVERALL PASS`; dynamic/emulator-first regression remains **NOT EXECUTED**.
