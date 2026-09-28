# DebugTool-AS2000 porting map

Reference: `applets/DebugTool/DebugTool.c` on the BetaWise master branch.

Status legend:

- **REWRITE**: retain the behavior, implement directly in HC11 assembly.
- **ROM-REUSE**: behavior should be provided mainly by existing AS2000 ROM code.
- **DROP**: not applicable or not worth the byte/RAM cost in v0.
- **DEFER**: useful only if the core fits comfortably.

| Original element | AS2000 action | Notes |
|---|---|---|
| Applet header / MSG_INIT / MSG_SETFOCUS lifecycle | DROP | No OS3K applet ABI in AS2000 implementation. |
| `g_scratch[256]` | DROP for v0 | Reconsider only if CALL requires a temporary data area. |
| Seven 16-byte argument/input buffers | DROP | Replace with one small shared input buffer or direct nibble editing. |
| `g_pAddress`, `g_pPrevAddress` | REWRITE | Represent as bank + 16-bit address as required by AS2000 memory map. |
| `g_mode`, `g_cursor` | REWRITE | Compact byte/bit state. |
| 68k bus-error handler | DROP | Replace with AS2000-specific valid-range/bank rules where needed. |
| `DumpSetCursor*` | ROM-REUSE + REWRITE | Use stock cursor/LCD services where smaller than direct I/O. |
| `DumpRedrawByteHex` | ROM-REUSE | Consolidated v0 reuses stock `$A3A0`, which converts and emits both hexadecimal digits. |
| `DumpRedrawByteAscii` | REWRITE | Minimal printable/non-printable mapping. |
| `DumpWriteAndRedrawCur` | REWRITE | Byte write + local redraw. |
| `DumpRedrawScreen` | REWRITE + ROM-REUSE | Fixed 40x4 layout, no general formatting library. |
| `DumpMoveCursor` | REWRITE | Preserve line/screen navigation only if byte cost remains small. |
| `DumpSetAddress` | REWRITE | Core GOTO primitive. |
| `HexCharToNibble` | REWRITE | Tiny local helper unless a suitable stock conversion exists. |
| `NumberFromString` | REWRITE/SIMPLIFY | Remove decimal, 32-bit, syscall and scratch syntax unless justified. |
| `NumberPrompt` | ROM-REUSE/SIMPLIFY | Prefer native key input; no TextBox/dialog abstraction. |
| Tab hex/ASCII toggle | REWRITE | Retain. |
| Arrow/Home/End navigation | REWRITE | Retain basic arrows; Home/End conditional on size. |
| Ctrl+G GOTO | REWRITE | Retain; bank/address syntax to be defined. |
| Ctrl+Shift+G indirect jump | DEFER | Add only if cheap after core implementation. |
| Backspace previous address | DEFER | Costs little but not required for first usable build. |
| Ctrl+R refresh | REWRITE | Retain if redraw entry point already exists. |
| Ctrl+I generic six-argument call | REWRITE/REDESIGN | Replace with HC11-native CALL interface; do not emulate 68k ABI. |
| Clear scratch | DROP | Scratch removed from v0. |
| Battery shortcut | DROP | Unrelated to debugger core. |

## First-pass functional core

The smallest useful monitor is expected to consist of these internal
primitives:

1. `view_redraw`
2. `cursor_move`
3. `byte_edit`
4. `goto_address`
5. `call_target`
6. `hex_to_nibble`
7. `byte_to_hex`
8. thin wrappers around selected stock ROM LCD/keyboard/bank routines

No additional primitive is to be added unless one of the five frozen v0
features cannot be implemented without it.

## Open technical decisions before assembly starts

1. Exact ROM routines to reuse for LCD positioning/output.
2. Exact keyboard scan/key decode entry points.
3. Bank-selection/read/write mechanism for the address ranges we actually
   intend the monitor to inspect.
4. HC11 `CALL` contract: registers, stack arguments, and return-value display.
5. Entry/exit mechanism from the patched AS2000 firmware.

These are discovery tasks, not new feature work.
