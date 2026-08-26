# FileOpen / FileClose ABI closure — A1C8 / A1CC

This note closes the public ABI contract of System 3 traps A1C8 and A1CC for the BetaWise SDK.

The mechanical ABI is confidence **A** from direct comparison of AlphaSmart 3000 System 3 (Nov 2005), NEO System 3 (Nov 2005), and NEO System 3 (Jul 2013). The `FileOpen` / `FileClose` names are confidence **B+**: they are longstanding BetaWise names and now match the complete observed lifecycle, although the early 2000 `FileModule.c` source predates these literal wrapper names.

## Public prototypes

```c
uint16_t FileOpen(uint16_t file_id, uint8_t reset_position);
void FileClose(void);
```

The public contract intentionally names the second argument by its demonstrated effect rather than by an unrecovered historical enum or mode name.

## Stable firmware implementations

A1C8 / FileOpen:

| Firmware | Handler | Length |
| --- | ---: | ---: |
| AS3000 Nov 2005 | `0x4E1B18` | `0x130` |
| NEO Nov 2005 | `0x5E4474` | `0x130` |
| NEO Jul 2013 | `0x43A876` | `0x130` |

The three 304-byte implementations are structurally equivalent. NEO 2005 and NEO 2013 share 293/304 byte positions directly; the differences are relocated globals/calls and trace metadata. AS3000 differs similarly only where platform addresses move.

A1CC / FileClose:

| Firmware | Handler bytes |
| --- | --- |
| AS3000 | `42 b9 00 00 34 42 4e 75` |
| NEO 2005 | `42 b9 00 00 34 9a 4e 75` |
| NEO 2013 | `42 b9 00 00 34 d2 4e 75` |

These eight-byte handlers are exactly `clear active-descriptor global; RTS`, with only the relocated global address changing.

## FileOpen argument ABI

The handler reads `file_id` from the first caller stack slot as a **16-bit word** and passes it to the shared descriptor resolver also used by `FileGetFileInfo`.

The second stack slot is inspected once as a **byte**. The only comparison is exactly:

```text
reset_position == 1
```

If true, the handler clears descriptor offset `+0x14`, already established as the current file cursor/position. If false, it does not modify that field at this point.

Official SmartApplet callers use only values `0` and `1`. Across the project corpus there are 25 direct A1C8 callers in the 2012-era AlphaWord Plus, Calculator, ControlPanel, KeyWords, and AlphaQuiz binaries: 17 pass `1` and 8 pass `0`. Calculator and ControlPanel binaries from 2005 independently preserve the `FileOpen(0x00CB, 1)` pattern.

Therefore `reset_position` is not a guessed read/write/create mode. Its demonstrated semantics are:

- value exactly `1`: rewind/reset the resolved descriptor cursor to zero;
- any other byte value: preserve the current cursor.

The SDK should normally use `0` or `1`, matching all official callers observed.

## FileOpen resolution and return contract

The resolver recognizes the file-token model already established elsewhere in the File API:

- ordinary file tokens resolve within the current `FileSetFolder` namespace;
- token `0x00CB` resolves the dedicated clipboard descriptor;
- token `0` means the currently active descriptor when used by the resolver internally.

A1C8 initializes its resolver-output token to zero. If `file_id` cannot be resolved, it returns **0 immediately**. Crucially, this branch occurs **before A1CC**, so a failed FileOpen does not discard or replace an already active descriptor.

On successful resolution, A1C8:

1. calls A1CC, detaching the previously active descriptor;
2. performs the normal descriptor/open-state preparation used by System 3;
3. stores the resolved descriptor in the active-descriptor global;
4. optionally clears cursor `+0x14` when `reset_position == 1`;
5. returns descriptor `+0x44`, the canonical resolved 16-bit file token, zero-extended in D0.

Thus the return type is `uint16_t`, with zero meaning failure/no resolution. On success, `FileGetCurrentFile()` returns the same canonical token while the context remains active.

## FileClose is detach, not commit

A1CC consumes no arguments and does not assign D0. Its entire state mutation is:

```text
active_descriptor = NULL
```

It does not itself:

- erase storage;
- change current size;
- change recoverable/previous size;
- change maximum size;
- reset the cursor;
- synchronize or clear the persistent `FileGetFileInfo` mirror pointers;
- perform a data copy or explicit flush/commit sequence.

Therefore `void FileClose(void)` is the correct ABI shape. Any code that treats its stale D0 as a return status is relying on undefined incidental register contents.

The name `FileClose` should be understood as **closing/detaching the active file context**, not as a guarantee of a transactional commit operation.

## Official caller behavior

Direct A1CC callers are common and lifecycle-shaped:

- AlphaWord Plus: 1
- AlphaQuiz: 16
- KeyWords: 13
- Calculator: 1
- ControlPanel: 1

Many occur after A1C8-centered file operations. Other callers explicitly close before switching namespace or opening another context. This matches the simple active-context lifecycle implemented by the handlers.

Representative A1C8 sequences include:

- `FileOpen(...,1)` -> `FileSmashFile` -> `FileWriteBuffer` for replacement/new-content paths;
- `FileOpen(...,1)` -> size/resize operations -> `FileClose`;
- `FileOpen(...,0)` around paths that deliberately preserve an existing cursor;
- `FileOpen(0x00CB,1)` in Calculator, ControlPanel, and AlphaQuiz for clipboard operations.

## Historical-source boundary

The original AS3000 `FileModule.c` from 2000 contains an explicit current-file concept and the operations that later map to `FileGetCurrentFile`, `FileGetFileInfo`, `FileSmashFile`, and `FileDeleteChars`, but it does **not** contain literal public functions named `FileOpen` or `FileClose`.

This is not a contradiction. The later System 3 runtime added a resolver-based active-descriptor abstraction and folder namespaces that are absent from the simpler early source. The modern names are retained because they describe the now-demonstrated System 3 lifecycle and are already established in BetaWise; they are not claimed to be recovered 2000 symbols.

## Regression invariants

The emulator regression matrix should verify:

1. valid token + `reset_position=1` -> nonzero canonical token, active token matches, cursor becomes zero;
2. valid token + `reset_position=0` -> cursor is preserved;
3. a non-1 value follows the preserve-cursor branch;
4. unresolved token -> return zero and preserve the previously active descriptor;
5. successful open replaces the previous active context;
6. `FileClose()` -> `FileGetCurrentFile()==0`;
7. closing does not modify the closed descriptor's storage/size/cursor metadata;
8. clipboard token `0x00CB` follows the same open/close lifecycle.

Invalid-token and non-1 edge cases should be exercised in the emulator before deliberate physical-hardware testing.

## Closure status

- A1C8 mechanics and ABI: **closed, confidence A**.
- A1CC mechanics and ABI: **closed, confidence A**.
- `FileOpen` / `FileClose` naming: **accepted for the SDK, confidence B+**.
- Remaining historical uncertainty about an original private symbol or enum name is no longer an ABI blocker.
