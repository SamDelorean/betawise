# System 3 / OS3K ClearScreen closure — A000

**Status (2026-08-31): mechanical confidence A / SOURCE-FIRST revalidated / static regression PASS.**

This note closes the portable ABI and observable state contract of trap A000
across the November 2005 AlphaSmart 3000 ROM, November 2005 NEO ROM, and July
2013 NEO/System 3.15 ROM. The historical/public name `ClearScreen` is retained
because BetaWise, early development material, debugger documentation, official
callers, and the firmware behavior agree on the identity of this service.

## Reconstructed raw System 3 form

```c
void _OS3K_ClearScreen(void);
```

A000 consumes no caller arguments and has no contractual return value. Any
register residue after the handler is implementation detail and must not be used
as a result.

## Mechanical contract

A000 clears the complete display, hides the System 3 cursor, and resets the
System 3 logical cursor position to row 1, column 1.

The hardware mechanism differs by platform while preserving that public
contract:

| Firmware | Display action | Cursor/state action |
| --- | --- | --- |
| AS3000, Nov 2005 | issues the LCD clear-display command to both character-LCD halves | hides cursor; row = 1; column = 1 |
| NEO, Nov 2005 | zero-writes both graphical LCD controllers and fills the OS character shadow with spaces (`0x20`) | hides cursor; row = 1; column = 1; graphical x/y restored from the current origins |
| NEO/System 3.15, Jul 2013 | same algorithm as NEO 2005 with relocated helpers/globals | same state contract |

The NEO handler does **not** establish a portable promise to reset LCD
start-line / scrolling state. That distinction matters because the higher-level
BetaWise wrapper performs additional display bookkeeping in its extended-font
path.

## BetaWise wrapper versus raw trap

BetaWise intentionally has two layers:

```c
void _OS3K_ClearScreen(void);  /* raw A000 */
void ClearScreen(void);        /* public BetaWise wrapper */
```

`ClearScreen()` calls raw A000. When the extended BetaWise font/display path is
active (`g_pCurFont != NULL`), the wrapper additionally resets its own logical
row/column state, resets `g_CurLcdRoll` to zero, and writes LCD start-line zero
through `_LCD_SetScreenRoll()`.

Therefore the wrapper must not be collapsed to a direct alias of A000. The raw
trap owns the System 3 clear/cursor contract; the wrapper additionally restores
BetaWise private scrolling/display bookkeeping. `BwProcessMessage(MSG_SETFOCUS)`
deliberately calls `_OS3K_ClearScreen()` directly rather than the wrapper,
further confirming that these are distinct layers.

## Return-register behavior

The AS3000 and NEO implementations do not establish a common returned value.
On the NEO path, D0 may retain residue from a helper; the handler does not
normalize it, and the AS3000 path does not expose an equivalent portable
result. Historical declarations and official callers use the service as
`void`.

The only portable contract is therefore:

```c
void _OS3K_ClearScreen(void);
```

## Canonical ROM and handler audit references

| Firmware | Canonical ROM SHA-256 | Runtime address | File offset | Handler size | Handler SHA-256 |
| --- | --- | ---: | ---: | ---: | --- |
| AS3000 System 3, Nov 2005 | `732dca63399d883071ce41cc07ad352fa19c506b7b8c307ace16d5c53e6f8708` | `0x004cd55e` | `0x0000d55e` | `0x6e` bytes | `8402c596ea9856f763f0c5abfff509cc4db3cf84d3ea21409f1a12dfed73bc6f` |
| NEO System 3, Nov 2005 | `5f550e48ad36892ee5f5a067e8c1b87f6bf504b6cb2c2dfc4098776fa8831a3d` | `0x005d1060` | `0x00011060` | `0xbc` bytes | `5b1c3b5edb3dc0a28217a838e7a98ead5820181aed1dca8a85f7c7b681e4d504` |
| NEO/System 3.15, Jul 2013 | `32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0` | `0x00421fe0` | `0x00011fe0` | `0xbc` bytes | `245479e2bb16f3c9d0a4e9fcdb090153800d35e6bf6e42eda1bebda9cb5cb971` |

The handlers were re-extracted from all three canonical ROMs and independently
disassembled as 68000 code. The AS3000 handler decodes to 27 instructions over
`0x6e` bytes; each NEO handler decodes to 54 instructions over `0xbc` bytes.
All three reproductions end at the independently delimited `RTS` boundary with
zero unknown opcodes. The two NEO handlers have the same control-flow algorithm;
their byte differences are relocations of helpers and runtime globals rather
than a change to the A000 contract.

On AS3000 the LCD helper reached by A000 was also reconstructed far enough to
confirm the clear mechanism: it accepts four arguments, polls the LCD ready bit,
selects display half/controller and command-versus-data access, and A000 invokes
it with the clear-display command for both halves after hiding the cursor.
Extended firmware disassembly and ROM bytes remain in the private audit record;
they are intentionally not reproduced here.

## Historical genealogy and source correlation

The early AS3000 source function `LCDClearDisplay(void)` states that it clears
the LCD, moves the cursor to `1,1`, and hides it. Its implementation hides the
current cursor, sends `LCD_CMD_CLEAR_DISPLAY` to the top and bottom displays,
and stores row/column 1. This is genealogy/name evidence; the 2005/2013 ROM
handlers above are the authority for the modern contract.

BetaWise maps syscall index 0 directly to `_OS3K_ClearScreen`, and its public
SDK historically declares `ClearScreen`. The early debugging material also
names system call `!0` as `ClearScreen`. These independent anchors were
correlated before the firmware revalidation rather than being treated as proof
by themselves.

## Official caller evidence

Executable cross-references were resolved by their effective 68k destination,
not by counting raw `0xA000` byte coincidences. The official application audit
found 239 executable references to the A000 veneer: 120 in AlphaWord Plus 2005
and 119 in AlphaWord Plus NEO 2012.

The caller shape independently supports the ABI above: no caller arguments are
supplied for A000 and no caller treats a post-call register value as a portable
result. Raw `0xA000` appearances that could not be established as executable
references were excluded from the caller count.

## Refutation checks and contract boundaries

The dominant `ClearScreen` hypothesis was tested against three plausible
misreadings:

- **Start-line normalization belongs to raw A000:** rejected. The firmware does
  not support that as a portable raw contract; BetaWise performs the additional
  normalization in its wrapper path.
- **D0 is a return value:** rejected. Cross-generation implementations do not
  establish a common returned value and callers use the service as `void`.
- **NEO 2005 and NEO 2013 differ semantically:** rejected for A000. Their
  control-flow algorithm is equivalent; observed differences are relocations.

These negative checks are part of the closure, not merely absence of contrary
evidence.

## Relation to standard C / the BetaWise runtime

`ClearScreen` is not an ISO C or `stdio.h` service. It is an OS3K display
primitive reached through the A-line syscall ABI. BetaWise applications are
linked against `libos3k.a` and resident System 3 services rather than relying on
a conventional hosted C library to provide terminal/display semantics.

A desktop terminal clear operation, curses/conio routine, or ANSI escape such
as `ESC[2J` is **not** a substitute for A000. The A000 contract includes OS3K
cursor/display state in addition to making the visible display blank.

Classification:

- raw OS3K service identity/signature: **CONFIRMED** — `_OS3K_ClearScreen(void)`;
- raw display/cursor contract: **CONFIRMED**;
- BetaWise wrapper distinction: **CONFIRMED**;
- common D0 result: **rejected / not contractual**;
- dynamic hardware/emulator probe result: **UNKNOWN until executed**;
- ISO C equivalent: none.

## Safety and preconditions

A000 has no caller-supplied pointer, length, selector, or other input to
validate. No application-level failure status is returned. Normal use assumes
the firmware display subsystem has already been initialized by System 3.

## Regression / probe status

Two validation layers are intentionally tracked separately.

**Static regression: EXECUTED / PASS.** The persisted regression harness checks
the canonical ROM identities, handler boundaries and selected mechanical
invariants used by this closure. It completed successfully. This is a static
binary/contract regression; it must not be described as a runtime display test.

**Dynamic probe: SPECIFIED / NOT EXECUTED.** A later emulator or hardware probe
should separately exercise the raw syscall and the BetaWise wrapper:

1. place visible content on screen and select a non-`1,1` visible cursor;
2. call raw A000 and verify blank display, hidden OS cursor and `1,1` position;
3. on NEO, exercise a nonzero LCD start-line before raw A000 and determine the
   observed raw behavior without promoting it beyond the portable contract;
4. call public `ClearScreen()` in the extended BetaWise font path and verify its
   private row/column state and `g_CurLcdRoll` return to `1,1,0` and LCD
   start-line zero;
5. do not treat D0 residue as a testable return value.

The missing dynamic probe is useful additional validation but is **not a blocker
for the source-first mechanical/ABI closure of A000**.
