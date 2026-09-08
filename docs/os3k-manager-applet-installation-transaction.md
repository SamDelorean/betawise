# OS3K Manager applet-installation transaction

This note records a source-first reconstruction of the Manager-side transaction used to install a SmartApplet. It intentionally stops short of assigning firmware-handler semantics that have not yet been verified against the canonical images.

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

Under 32-bit arithmetic this means the low 24 bits of `argument` carry the file-size portion while bits 24..31 carry bits 16..23 of the combined memory requirement; `trailing` carries its low 16 bits. This is a source-level wire-packing observation, not yet a firmware-side semantic proof. The exact range checks, overflow policy, and allocator interpretation still require canonical-ROM verification.

For the common case where `combined_memory_size < 0x10000`, `argument` is simply `file_size` and `trailing` is the combined memory requirement. The `alpha-core` regression example `file_size=0x1234`, `base_memory_size=0x0100`, `extra_memory_size=0x2000` produces `(argument,trailing)=(0x1234,0x2100)`.

## Important correction: obsolete `0xFF` PoC step

An older `poc/neotools` helper constructs an extra packet `command(0xFF,0,0)` after raw applet data and labels it `add_applet_chunk_commit`. That helper must not be promoted into the recovered Manager transaction.

The newer `real-check` client and current Rust `alpha-core`/`alpha-cli` installation path do **not** send this `0xFF` packet. Instead they send the raw chunk and immediately read status `0x43`, then send `0x0B`. This also fits the independently reconstructed Manager request dispatcher, whose normal selector namespace is `0x00..0x1F`; treating `0xFF` as another ordinary dispatcher case would contradict that boundary.

Accordingly, until direct firmware evidence proves a separate transport-level role, `0xFF` is classified as **PROVISIONAL / obsolete PoC artifact**, not part of the canonical Manager applet-installation sequence.

## Verification boundary

Status: **SOURCE-FIRST CONFIRMED / firmware correlation pending**.

Confirmed from current independent host implementations:

- `0x06` begins applet installation and expects `0x46`;
- `0x02` announces each raw block with byte count and 16-bit additive checksum and expects `0x42`;
- raw block data are limited to `0x400` bytes per chunk by the current installers;
- `0x43` is consumed after raw chunk transmission;
- `0x0B` follows every raw block and expects `0x47`;
- `0x07` finalizes installation and expects `0x48`;
- the `0x06` host fields are deterministically packed from `file_size`, `base_memory_size`, and `extra_memory_size` as documented above.

Not yet promoted to firmware contracts:

- exact branch bounds for cases `0x06`, `0x02`, `0x0B`, and `0x07`;
- firmware-side unpacking and validation of the `0x06` packed sizes;
- transfer-state globals shared by those cases;
- allocation, rollback, and failure semantics;
- exact validation performed during finalization;
- internal helper/vendor names.

The next canonical-ROM pass should reconstruct the four selectors as one state machine and explicitly test the transitions `begin -> block announce -> raw-data completion -> program -> finalize` across AS3000, NEO 2005, and NEO 2013.

No A-line ABI entry is implied. The Manager request namespace is independent of the A-line syscall frontier.
