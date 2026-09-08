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

## Important correction: obsolete `0xFF` PoC step

An older `poc/neotools` helper constructs an extra packet `command(0xFF,0,0)` after raw applet data and labels it `add_applet_chunk_commit`. That helper must not be promoted into the recovered Manager transaction.

The newer `real-check` client and current Rust `alpha-core`/`alpha-cli` installation path do **not** send this `0xFF` packet. Instead they send the raw chunk and immediately read status `0x43`, then send `0x0B`. The firmware correlation now reinforces this distinction: raw completion and response `0x43` are present as a transport path independent of the 32-entry Manager selector table, while the normal Manager selector namespace is `0x00..0x1F`.

Accordingly, absent separate transport-level evidence, `0xFF` remains **PROVISIONAL / obsolete PoC artifact**, not part of the canonical applet-installation state machine.

## Verification boundary

Status: **structurally/mechanically closed across AS3000, NEO 2005, and NEO 2013 for transaction topology and state handoff**.

Confirmed:

- stage order and response codes `0x46`, `0x42`, `0x43`, `0x47`, `0x48`;
- `0x02` block-announcement role and handoff into raw reception;
- raw `0x43` completion as a transport result, not a request selector;
- `0x06` establishment of staging/install state;
- `0x0B` per-block commit/program stage and cumulative-state advance;
- `0x07` finalization/validation stage;
- physical branch geometry for the four selectors in each canonical generation;
- the NEO 2013 `0x07` branch is 0x0C bytes longer than the earlier two generations.

Still unresolved or intentionally unnamed:

- exact firmware unpacking/range policy for every `0x06` size field;
- allocator and rollback policy;
- source/vendor names of internal globals and helpers;
- complete interpretation of internal status/error paths;
- exact final validation policy;
- source-level meaning of the NEO 2013 finalizer's additional 0x0C bytes.

A private reproducible regression over the three canonical firmware hashes passes 111/111 assertions. Firmware bytes and extended disassembly remain private; this public note contains only the functional reconstruction.

No A-line ABI entry is implied. The Manager request namespace remains independent of the A-line syscall frontier, whose demonstrated endpoint remains A470.
