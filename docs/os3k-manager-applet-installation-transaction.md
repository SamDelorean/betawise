# OS3K Manager applet-installation transaction

This note records a source-first reconstruction of the Manager-side transaction used to install a SmartApplet. Host implementations were correlated first, then the transaction stages were verified directly in canonical AS3000, NEO 2005, and NEO 2013 firmware images.

## Source correlation

Public AlphaSync material, the current `neo-re` protocol layer, `alpha-core`, and the hardware-oriented `real-check` client converge on a multipart installation transaction. Selector `0x06` is only the initialization stage; it does not carry the applet payload itself.

The current host-side sequence is:

1. Send `0x06` (add/write applet begin) and require response/status `0x46`.
2. Split the image into chunks of at most `0x400` bytes.
3. For each chunk, send `0x02` with `argument = chunk_length` and `trailing = sum16(chunk)`, then require `0x42`.
4. Send the raw chunk bytes, then require `0x43`. This is a response after the raw transfer, not another Manager request selector.
5. Send `0x0B` with zero argument/trailing and require `0x47`.
6. Repeat steps 3–5 for every chunk.
7. Send `0x07` with zero argument/trailing and require `0x48`.

Therefore the source-supported transaction is:

`0x06 begin -> [0x02 announce -> raw bytes -> 0x43 response -> 0x0B program] * N -> 0x07 finalize`

with `N = ceil(image_size / 0x400)` in the current implementations.

## `0x06` start-field packing

The current `neo-re` Python and Rust implementations independently derive the begin fields from the 0x84-byte SmartApplet header as follows:

```text
combined_memory_size = base_memory_size + extra_memory_size
argument = file_size | ((combined_memory_size & 0xFFFF0000) << 8)
trailing = combined_memory_size & 0xFFFF
```

Under 32-bit arithmetic this means the low 24 bits of `argument` carry the file-size portion while bits 24..31 carry bits 16..23 of the combined memory requirement; `trailing` carries its low 16 bits. The host-side packing is confirmed independently; the exact firmware allocator interpretation and every range/error policy remain outside the closed contract.

For the common case where `combined_memory_size < 0x10000`, `argument` is simply `file_size` and `trailing` is the combined memory requirement. The `alpha-core` regression example `file_size=0x1234`, `base_memory_size=0x0100`, `extra_memory_size=0x2000` produces `(argument,trailing)=(0x1234,0x2100)`.

## Canonical-firmware correlation

The same transaction topology is present in all three canonical generations.

| Stage | AS3000 file offset | NEO 2005 file offset | NEO 2013 file offset | Mechanical result |
| --- | ---: | ---: | ---: | --- |
| `0x02` block announce | `0x000E68` | `0x000DE6` | `0x000F44` | 0x26-byte branch; busy guard; resets per-block checksum state; latches announced 16-bit length; accepted path enters raw-receive setup and produces `0x42`. |
| `0x06` begin | `0x00111E` | `0x000FC4` | `0x001122` | 0x128-byte branch; initializes installation/staging state and produces `0x46`. |
| `0x0B` program/commit | `0x001258` | `0x0010FE` | `0x00125C` | 0x30C-byte branch; requires active installation state, commits the accepted chunk into cumulative state, clears the per-chunk accumulator, re-arms staging, and produces `0x47`. |
| `0x07` finalize | `0x001564` | `0x00140A` | `0x001568` | Finalization/validation branch and `0x48`; 0x284 bytes in AS3000/NEO 2005, 0x290 bytes in NEO 2013. |

The raw-data completion path is not another Manager selector. It is reached through the receive transport and occurs at file offsets `0x002900`, `0x0027A6`, and `0x002B98` respectively. In all three generations it compares the received length with the length latched by `0x02`, advances the accepted-chunk accumulator on success, and prepares response `0x43`. A nearby mismatch/error route is distinct.

This gives a mechanically verified state handoff:

`0x06 -> 0x46 -> [0x02 -> 0x42 -> raw transport -> 0x43 -> 0x0B -> 0x47] * N -> 0x07 -> 0x48`

The exact RAM addresses of the state variables move between generations, while their roles in this transaction remain homologous.

### Generation difference in `0x07`

The finalizer is not byte-geometrically identical in all three images. AS3000 and NEO 2005 use a 0x284-byte physical branch; NEO 2013 uses 0x290 bytes, an additional 0x0C bytes. The existence of that extension is confirmed. Its source-level purpose is not yet demonstrated and remains `NO_RESUELTO / EVIDENCIA_INSUFICIENTE` rather than being assigned a speculative feature name.

## Cross-firmware correlation: principal OS vs NEO Small ROM

The NEO OS-update path shows that the block-transfer portion above is not applet-specific. The host first sends `0x18` while the principal OS is active and expects `0x56`, requesting entry into Small ROM. Once Small ROM is active, the host sends `0x16` and expects `0x54` to clear the OS segment map, then sends one `0x17(address, erase_kb)` per declared segment and expects `0x55` for each erase/registration operation.

After this Small-ROM-specific destination preparation, the host invokes the exact same block engine used for SmartApplet installation:

`[0x02(len,sum16) -> 0x42 -> raw bytes -> 0x43 -> 0x0B -> 0x47] * N -> 0x07 -> 0x48`

The same implementation routine in current `neo-re` is used for both applet images and full OS images, with chunks of at most `0x400` bytes. This is significant because it separates **destination preparation** from a **common transfer/programming layer**.

The most conservative recovered semantics are therefore:

- `0x02`: announce/arm reception of the next raw block, including length and checksum. This is a generic transport primitive, not an applet-only operation. AlphaSync independently uses the same request for file/attribute payloads and identifies the `0x42`/`0x43` two-stage handshake.
- raw-data completion / `0x43`: transport completion after the announced payload has actually arrived; not a selector-table command.
- `0x0B`: program/commit the currently staged block in the active programming context. In the principal OS that context is the SmartApplet installation transaction; in Small ROM it is the OS flash transaction prepared by `0x16/0x17`.
- `0x07`: finalize the active programming transaction. The older name “finalize applet update” is too narrow when viewed across both firmware contexts.
- `0x06`: principal-OS SmartApplet destination/setup stage; it should not be generalized into the common block engine.
- `0x16`/`0x17`: Small-ROM OS destination-map/erase setup; these are not part of normal SmartApplet installation.
- `0x18`: transition request from the principal OS into Small ROM, preceding the Small-ROM command context.

Private direct decoding of the canonical NEO Small ROM has already established the `0x16` reset of the segment map, `0x17` storage of ordered segment base/erase extent, and a programming route that advances through the registered segments and splits transfers at segment boundaries. The source-level cross-correlation above now explains how the common `0x02/raw/0x0B/0x07` transport feeds that mapper.

This does **not** yet promote guessed vendor names or claim that the principal-OS and Small-ROM handlers are byte-identical. Their policy/state backends differ by design. Exact physical Small-ROM handler boundaries for the common selectors remain a separate firmware-correlation task and should be reported only after reproducible direct decoding.

### Consequence for the recovered state machine

A better model is now:

```text
principal OS SmartApplet install:
    0x06 destination/setup
      -> COMMON_BLOCK_ENGINE
      -> 0x07 transaction finalization

principal OS -> Small ROM OS update:
    0x18 firmware-context transition
      -> 0x16 clear map
      -> 0x17 register/erase segments
      -> COMMON_BLOCK_ENGINE
      -> 0x07 transaction finalization

COMMON_BLOCK_ENGINE per block:
    0x02 announce(len,sum16)
      -> raw payload
      -> 0x43 completion
      -> 0x0B program/commit
```

This model is more stable than assigning applet-specific names to the shared selectors and is directly useful for emulator work: an emulated updater must preserve the firmware-context transition and the destination-specific state while retaining the same Manager transport envelope.

## Important correction: obsolete `0xFF` PoC step

An older `poc/neotools` helper constructs an extra packet `command(0xFF,0,0)` after raw applet data and labels it `add_applet_chunk_commit`. That helper must not be promoted into the recovered Manager transaction.

The newer `real-check` client and current Rust `alpha-core`/`alpha-cli` installation path do **not** send this `0xFF` packet. Instead they send the raw chunk and immediately read status `0x43`, then send `0x0B`. The firmware correlation now reinforces this distinction: raw completion and response `0x43` are present as a transport path independent of the 32-entry Manager selector table, while the normal Manager selector namespace is `0x00..0x1F`.

Accordingly, absent separate transport-level evidence, `0xFF` remains **PROVISIONAL / obsolete PoC artifact**, not part of the canonical applet-installation state machine.

## Verification boundary

Status: **structurally/mechanically closed across AS3000, NEO 2005, and NEO 2013 for the principal-OS SmartApplet transaction; cross-firmware Small-ROM correlation is SOURCE-FIRST CONFIRMED with mapper mechanics already confirmed, while exact Small-ROM handler boundaries for the common selectors remain pending direct publication-quality correlation**.

Confirmed:

- stage order and response codes `0x46`, `0x42`, `0x43`, `0x47`, `0x48` for principal-OS SmartApplet installation;
- `0x02` block-announcement role and handoff into raw reception;
- raw `0x43` completion as a transport result, not a request selector;
- `0x06` establishment of staging/install state;
- `0x0B` per-block commit/program stage and cumulative-state advance;
- `0x07` finalization/validation stage;
- physical branch geometry for the four selectors in each canonical principal OS generation;
- the NEO 2013 principal-OS `0x07` branch is 0x0C bytes longer than the earlier two generations;
- NEO OS update enters Small ROM with `0x18 -> 0x56`, prepares its segment map with `0x16 -> 0x54` and `0x17 -> 0x55`, then reuses the same `0x02/raw/0x0B/0x07` host-side block sequence and responses;
- canonical Small-ROM mapper mechanics for `0x16/0x17` and segment-aware programming.

Still unresolved or intentionally unnamed:

- exact firmware unpacking/range policy for every `0x06` size field;
- allocator and rollback policy;
- source/vendor names of internal globals and helpers;
- complete interpretation of internal status/error paths;
- exact final validation policy;
- source-level meaning of the NEO 2013 finalizer's additional 0x0C bytes;
- exact direct Small-ROM physical handler boundaries and helper graph for `0x02`, raw completion, `0x0B`, and `0x07`.

A private reproducible regression over the three canonical principal-OS firmware hashes passes 111/111 assertions. That regression is not being reused as proof of still-pending Small-ROM handler-boundary assertions. Firmware bytes and extended disassembly remain private; this public note contains only the functional reconstruction.

No A-line ABI entry is implied. The Manager request namespace remains independent of the A-line syscall frontier, whose demonstrated endpoint remains A470.