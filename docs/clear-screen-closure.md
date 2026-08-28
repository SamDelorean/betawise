# System 3 / OS3K ClearScreen closure — A000

This note closes the mechanical ABI and observable state contract of trap A000
across the November 2005 AlphaSmart 3000 ROM, November 2005 NEO ROM, and July
2013 NEO/System 3.15 ROM. The historical/public name `ClearScreen` is retained
because BetaWise, early development material, and the firmware behavior agree on
the identity of this service.

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

The NEO handler does **not** reset the LCD start-line / scrolling state. That
omission is observable and is the reason the higher-level BetaWise wrapper has
additional work to do.

## BetaWise wrapper versus raw trap

BetaWise intentionally has two layers:

```c
void _OS3K_ClearScreen(void);  /* raw A000 */
void ClearScreen(void);        /* public BetaWise wrapper */
```

`ClearScreen()` always calls raw A000. When the extended BetaWise font/display
path is active (`g_pCurFont != NULL`), the wrapper additionally resets its own
logical row/column state, resets `g_CurLcdRoll` to zero, and writes LCD
start-line zero through `_LCD_SetScreenRoll()`.

Therefore the wrapper must not be collapsed to a direct alias of A000. The raw
trap owns the System 3 clear/cursor contract; the wrapper additionally restores
BetaWise's private scrolling/display bookkeeping.

`BwProcessMessage(MSG_SETFOCUS)` deliberately calls `_OS3K_ClearScreen()`
directly rather than the wrapper, further confirming that these are distinct
layers.

## Return-register behavior

The AS3000 and NEO implementations do not establish a common returned value.
On the NEO path, the final substantial helper is the resident `memset` used to
fill the character shadow, so D0 may retain that helper's destination pointer.
The handler does not normalize this residue, and the AS3000 path does not expose
an equivalent result. Historical declarations and BetaWise callers also use the
service as `void`.

The only portable contract is therefore:

```c
void _OS3K_ClearScreen(void);
```

## Cross-ROM audit references

| Firmware | Runtime address | File offset | Handler size | Handler SHA-256 |
| --- | ---: | ---: | ---: | --- |
| AS3000 System 3, Nov 2005 | `0x004cd55e` | `0x0000d55e` | `0x6e` bytes | `8402c596ea9856f763f0c5abfff509cc4db3cf84d3ea21409f1a12dfed73bc6f` |
| NEO System 3, Nov 2005 | `0x005d1060` | `0x00011060` | `0xbc` bytes | `5b1c3b5edb3dc0a28217a838e7a98ead5820181aed1dca8a85f7c7b681e4d504` |
| NEO/System 3.15, Jul 2013 | `0x00421fe0` | `0x00011fe0` | `0xbc` bytes | `245479e2bb16f3c9d0a4e9fcdb090153800d35e6bf6e42eda1bebda9cb5cb971` |

The two NEO handlers have the same control-flow algorithm. Their byte
differences are relocations of helpers and runtime globals rather than a change
to the A000 contract.

## Historical genealogy and caller evidence

The early AS3000 source function `LCDClearDisplay(void)` states that it clears
the LCD, moves the cursor to `1,1`, and hides it. Its implementation hides the
current cursor, sends `LCD_CMD_CLEAR_DISPLAY` to the top and bottom displays,
and stores row/column 1. This is genealogy/name evidence; the 2005/2013 ROM
handlers above are the authority for the modern contract.

BetaWise maps syscall index 0 directly to `_OS3K_ClearScreen`, and its public
SDK historically declares `ClearScreen`. The early debugging tool also names
system call `!0` as `ClearScreen`. These independent callers/names agree with
the firmware semantics.

## Relation to standard C / the BetaWise runtime

`ClearScreen` is not an ISO C or `stdio.h` service. It is an OS3K display
primitive reached through the A-line syscall ABI. BetaWise applications are
linked against `libos3k.a` and resident System 3 services rather than relying on
a conventional hosted C library to provide terminal/display semantics.

A desktop terminal clear operation, curses/conio routine, or ANSI escape such
as `ESC[2J` is **not** a substitute for A000. The A000 contract includes OS3K
cursor/display state in addition to making the visible display blank.

This function is classified as:

- OS3K-specific raw service: `_OS3K_ClearScreen(void)`;
- BetaWise compatibility/convenience wrapper: `ClearScreen(void)`;
- no ISO C standard equivalent;
- host/AlphaSmart Manager evidence: not relevant to this local display syscall.

## Safety and preconditions

A000 has no caller-supplied pointer, length, selector, or other input to
validate. No application-level failure status is returned. Normal use assumes
the firmware display subsystem has already been initialized by System 3.

## Regression / probe status

Mechanical closure is complete; dynamic regression is **specified, not claimed
as executed**. A later probe should separately exercise the raw syscall and the
BetaWise wrapper:

1. place visible content on screen and select a non-`1,1` visible cursor;
2. call raw A000 and verify blank display, hidden OS cursor and `1,1` position;
3. on NEO, exercise a nonzero LCD start-line before raw A000 and verify that raw
   A000 does not promise to normalize the start-line;
4. call public `ClearScreen()` in the extended BetaWise font path and verify its
   private row/column state and `g_CurLcdRoll` return to `1,1,0` and LCD
   start-line zero;
5. do not treat D0 residue as a testable return value.

Until that regression is executed, A000 is **mechanically closed / probe
pending**, not dynamically validated.
