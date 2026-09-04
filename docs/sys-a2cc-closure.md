# OS3K A2CC closure

Status: **MECÁNICA_CERRADA A / PUBLICADO**. Dynamic regression is specified but not yet executed.

## Neutral contract

```c
uint32_t SYS_A2CC(void *state,
                  uint32_t limit,
                  uint8_t *out_bytes,
                  uint8_t arg4,
                  uint8_t arg5,
                  uint8_t arg6,
                  uint8_t arg7,
                  uint8_t arg8,
                  uint8_t arg9,
                  const uint8_t *arg10);
```

Every argument occupies a physical 32-bit stack slot. The handler consumes only the low byte of slots 4 through 9. `state` is mandatory and mutated. `out_bytes` is nullable and receives produced bytes sequentially. `arg10` is nullable and read only by comparison/match paths. The six byte arguments intentionally retain neutral names because their vendor meanings have not been recovered.

## Canonical handlers

| ROM | runtime | file offset | CFG-confirmed size | raw SHA-256 |
| --- | --- | --- | --- | --- |
| AS3000 2005 | `0x004D54A8` | `0x0154A8` | `0x558` | `9fb5a6f1e38580df631f15a325b2f3de322eabf213ff95edefc10bd63f916a39` |
| NEO 2005 | `0x005D7786` | `0x017786` | `0x560` | `1d4cf343490896ec2ffcd22fe199e5463034680146c1e33655e95fbe4f654f03` |
| NEO 2013 | `0x004291BE` | `0x0191BE` | `0x560` | `459cd5005daaa68ce706d13fbca7424af0505e7d61653ec7686208ba17eaec85` |

The three canonical ROM hashes are recorded in the private workpaper. Each handler has exactly one reachable `RTS`, at the final two bytes. A control-flow walk that treats both embedded jump tables as data reaches all 437 AS3000 code instructions and all 439 code instructions in each NEO body; there are no orphan code records inside the handler bounds.

NEO 2005 and NEO 2013 differ in exactly 19 three-byte relocation regions. Neutralizing those operands makes the bodies byte-identical, with normalized SHA-256 `ba788cef735bc163c1b0a6182ca4118387ad0988d81ff01e3897731940c4adc7`.

## Callers and return value

Six direct ROM references exist in each generation: one absolute `JSR` and five direct `BSR.W` callers. A standalone caller pushes ten longwords and removes exactly `0x28` bytes after the call, independently proving the ten-slot physical ABI.

More importantly, callers prove that **D0.L is contractual**. The AS3000 caller at `0x004C1F52` stores the complete D0 longword, subtracts it from a remaining-count global, advances a pointer by the same value, and subsequently iterates exactly that many bytes. The equivalent NEO callers at `0x005C1DF8` and `0x00411F62` perform the same full-longword accounting. A second caller family stores D0.L as a processed count and adds it to persistent progress fields.

The return therefore cannot be `void`, `uint8_t`, or `bool`. Normal exits return the accumulated D6 processed-byte count. Other terminal paths explicitly return `0` or `1`; those values are valid members of the same `uint32_t` count/result domain. No vendor error/status name is assigned to them.

Confirmed caller shapes include all-zero optional arguments; output-buffer use with `arg10 == NULL`; byte-option combinations; and `out_bytes == NULL`. No direct ROM caller found in this set supplies a non-NULL arg10, but the handler's non-NULL comparison paths are fully reachable and mechanically reconstructed.

## Internal state and dispatch

The routine is a byte-processing state machine. Confirmed mutable `state` offsets include `+0x04`, `+0x08`, `+0x0A`, `+0x0C`, `+0x0E`, `+0x14`, `+0x26`, and `+0x2A`; NEO internal helper calls additionally pass `state+0x10`.

A first 16-way dispatcher maps byte classes to concrete output actions. Mechanically observed examples include copying the source byte, substituting ASCII space (`0x20`), tab (`0x09`), or hyphen (`0x2D`), consuming an adjacent source byte, and entering special state transitions. These facts do not justify inventing vendor token names.

A second five-way dispatcher is selected by `state+0x2A`. It advances a byte cursor at `state+0x26`, emits bytes when `out_bytes` is non-NULL, updates D6, invokes a larger helper in one state, and transitions through states 0..4. Unknown selector values fall back into the surrounding loop rather than defining a new external ABI result.

## Helpers

There are 15 JSR sites but eight unique helper families per generation. The workpaper records all concrete targets. Two helpers have independently closed mechanics:

* `AS 0x004DDE24 / NEO05 0x005E0784 / NEO13 0x00436B82`: NUL-terminated byte-string length used to derive a comparison boundary.
* `AS 0x004DE7D8 / NEO05 0x005E1138 / NEO13 0x00437536`: ASCII `a..z` to uppercase conversion, otherwise identity; 0x1A bytes, common SHA-256 `517641aebad0e001ab132b7fa7064ebfe864a3d2549e25dc042c8777d1ead5e3`.

The remaining six helper families are catalogued by address, arguments, return use, and state fields in the private workpaper without speculative vendor names.

## Generational difference

The external ten-slot ABI is common to AS3000, NEO 2005, and NEO 2013. NEO adds exactly one extra internal `PEA state+0x10` to each of two major helper calls. Their cleanup sizes consequently grow by four bytes apiece, accounting for the eight-byte handler-size increase from AS3000 to NEO. This difference is preserved rather than normalized away semantically.

## Adversarial checks

The analysis explicitly rejected: deriving handler size solely from the next syscall; treating applet stub-table bytes as callers; interpreting caller cleanup larger than `0x28` as additional A2CC arguments; narrowing D0 from isolated `0`/`1` terminal paths; and assigning names to byte options from apparent behavior alone.

A2D0 was used only as an initial structural ceiling. Final A2CC bounds are instead supported by complete reachable CFGs, jump-table recovery, the unique terminal `RTS`, and absence of unreachable code inside the reconstructed bodies.

## Regression

Emulator-first regression is specified, **not executed**. It should exercise NULL/non-NULL `out_bytes`, the 16-way byte-class dispatch, states 0..4 at `state+0x2A`, explicit zero/one terminal paths, full processed-count returns, comparison paths with non-NULL `arg10`, and generation-differential state+0x10 behavior. For args 4..9, vary the upper 24 bits of the physical slot while holding the low byte constant; behavior must remain invariant.

## 2026-09-04 source-first re-audit

The closure was revalidated directly against the canonical ROMs after the A2C8 xref correction. Canonical ROM identities and exact A2CC handler fingerprints reproduced 3/3. A fresh whole-ROM control-transfer scan reproduced the complete direct-xref set in every generation: exactly one absolute `JSR`, exactly five `BSR.W` calls, and zero absolute `JMP` calls. The concrete callsite addresses match the private workpaper exactly.

Static structural regression: **15/15 PASS** (3 canonical ROM identities, 3 exact handler fingerprints, 3 exact handler lengths/terminal epilogues, 3 direct-xref sets, 3 absence-of-absolute-JMP checks). No contradiction was found in the ten-slot ABI, full-long processed-count return, helper catalogue, or the AS-vs-NEO internal `state+0x10` difference. Dynamic/emulator regression remains **specified / not executed**.

Private ROM bytes, complete disassemblies, CFG notes, caller traces, helper catalogue, and reproducibility material remain in Drive and are not published here.
