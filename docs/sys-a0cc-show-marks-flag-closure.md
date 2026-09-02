# SYS_A0CC / index 51 — Show Marks flag getter

Status: **mechanically closed / source-first revalidated**.

## Recovered raw contract

```c
uint8_t SYS_A0CC(void);
```

**CONFIRMADO:** the raw A-line service consumes no arguments, performs no writes in its direct handler, and returns one byte in `D0.b`. Bits above the low byte are not contractual.

**INFERENCIA FUERTE:** the byte is the system **Show Marks** enable flag. The original vendor symbol/prototype has not been recovered, so the public neutral name remains `SYS_A0CC`.

## Source-first anchors

BetaWise preserves the slot only as `DEFINE_SYSCALL(51, SYS_A0CC)`; no public named prototype was recovered from the current `os3k.h` surface.

The official AlphaSmart UI/documentation independently identifies **Show Marks** as a system/AlphaWord display setting. AlphaWord Plus' own visible command help groups `Sticky Keys, Auto Repeat, and Show Marks status` under `option-cmd-K`, while Slow Keys has a separate command. The NEO user guide describes Show Marks as making normally invisible text structure visible (for example, a tab space is displayed with an arrow).

Those labels are anchors only. The ABI and behavioral link below come from firmware.

## Firmware confirmation

The canonical ROM hashes match the established corpus. Index 51 resolves to:

| Generation | Handler | File offset | Flag byte |
| --- | ---: | ---: | ---: |
| AS3000 2005 | `0x004CE068` | `0x00E068` | `0x4331` |
| NEO 2005 | `0x005D204C` | `0x01204C` | `0x57BB` |
| NEO 2013 | `0x0042303C` | `0x01303C` | `0x5D13` |

All three handlers normalize to the same instruction shape:

```text
MOVE.B  <relocated_flag>.L,D0
RTS
```

Normalized bytes: `10 39 00 00 00 00 4E 75`.
Normalized SHA-256: `43c97db25353073aaf7a135445cec6b0d561fa67b19102f7b3b0df1fda36e958`.

## Paired setter and storage

Index 52 / A0D0 is a distinct handler immediately preceding A0CC in physical code and writes the same byte:

```text
MOVE.B  7(SP),<same_flag>.L
RTS
```

The three A0D0 targets are `0x004CE05E`, `0x005D2042`, and `0x00423032` respectively. Firmware contains four direct calls to that setter per generation; the observed call family includes explicit false (`0`) and true (`1`) values plus a save/restore path. The shared byte is explicitly cleared during system initialization.

This establishes a boolean-like persistent system flag independent of any guessed name.

## Callers and behavioral identity

A0CC has exactly two direct firmware calls per canonical ROM, plus its vector-table entry.

One caller consumes `D0.b` as a predicate (`TST.B D0`). When enabled, the downstream rendering path performs an additional display operation. Independently, that rendering path compares the same backing flag explicitly with `1` before issuing the additional mark-related drawing call.

The second caller copies `D0.b` into a saved working byte, temporarily clears the flag through A0D0 while performing a larger operation, and later restores the saved value through A0D0. This confirms byte width and getter/setter pairing without relying on documentation.

Combining these primary facts with the official UI feature gives the strongest supported interpretation: **A0CC reads the Show Marks display flag**. The neutral syscall name is retained because no historical vendor function symbol has been recovered.

## Refutation checks

The audit tested the most plausible alternatives:

- **Auto Repeat** is already represented by the preceding A0B8 family and has separate state/callers.
- **Sticky Keys** is represented by the A0BC/A0C0 pair and has separate state/callers.
- **Slow Keys** is represented by A0C4/A0C8 and has separate active/delay storage.
- A0CC is not a word/long getter: the handler uses `MOVE.B`, one caller tests the low byte, and another saves exactly the low byte.
- The A0D0 writer is not an unrelated adjacent helper: it is the actual index-52 vector target and writes the exact same storage byte consumed by A0CC.

No competing documented system boolean found in this local setting cluster explains the direct conditional rendering behavior as well as Show Marks.

## Regression

Static regression was **EXECUTED: 45/45 PASS** across AS3000 2005, NEO 2005, and NEO 2013. It checks canonical ROM hashes, index-51/index-52 vector targets, exact getter/setter instruction forms, target-reference counts, two direct A0CC JSRs, byte-width caller consumption, four direct A0D0 JSRs, four backing-global references, zero initialization, the explicit `flag == 1` rendering predicate, save/disable/restore behavior, and observed true/false setter calls.

Harness SHA-256: `6f2adeee4707a14ced5ac4d6fb4f093e073c99bed600ad4e5f72c081ec492bec`.
Stdout SHA-256: `91efad0c41877d6e5df00273668e885b312d012ef4ad97ffe5707ddb2d9b0e54`.

A dynamic display probe is **SPECIFIED / NOT EXECUTED**: toggle the paired flag, render text containing tabs/returns or other invisible structure, and verify that only the marks layer changes while underlying text remains unchanged.
