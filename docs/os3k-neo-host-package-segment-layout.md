# NEO System 3 host-package secondary segment layout

This note records a structural result for the NEO System 3 OS update package. It intentionally separates **host-file coordinates** from **installed flash addresses** and does not promote any A-line ABI entry.

## Scope

The canonical NEO 2013 package has length `0x60800` bytes. Its segment table declares three destinations:

| Segment | Installed address | Declared length |
| --- | ---: | ---: |
| Main System image | `0x00410000` | `0x00060000` |
| Secondary identity/name area | `0x00406000` | `0x00000014` |
| High fixed-tail area | `0x005FFC00` | `0x00000400` |

The updater erases/declares those destinations in table order and then streams the complete package in ordered `0x400`-byte chunks. A documented successful physical-device flash transferred 395264 bytes as 386 chunks across the three declared segments.

## Small ROM mapper

Direct analysis of the NEO Manager 3.9.3 Small ROM closes the placement ambiguity at the mechanism level.

The updater command dispatcher contains the independently observed commands `0x16`, `0x17`, and `0x18`. Command `0x16` clears the Small ROM segment-map count. Command `0x17` appends one map entry: the command's 32-bit address is stored in an address array, while its 16-bit erase length is converted from KiB to bytes and stored in a parallel length array. Entries retain declaration order.

The programming path indexes those arrays with the current segment index. It computes the current segment end as `base + recorded_length`; when a write reaches or crosses that boundary it advances the index and loads the next recorded base. The path also contains the special high-segment comparison against `0x005FFC00`. This establishes that the host stream is mapped sequentially over the rounded segment records rather than compacted according to the unrounded payload lengths.

For the canonical package, the host-side erase-length calculation and Small ROM recording produce:

```text
0x00410000 / 0x60000 -> recorded span 0x60000
0x00406000 / 0x00014 -> recorded span 0x00400
0x005FFC00 / 0x00400 -> recorded span 0x00400
```

The three recorded spans sum exactly to `0x60800`, the complete host package.

## Host-file partition and confirmed placement

The resulting mapping is:

| Host-file range | Installed range | Physical role |
| --- | --- | --- |
| `0x000000..0x05FFFF` | `0x00410000..0x0046FFFF` | main System image transfer span |
| `0x060000..0x0603FF` | `0x00406000..0x004063FF` | secondary transfer span |
| `0x060400..0x0607FF` | `0x005FFC00..0x005FFFFF` | high fixed-tail transfer span |

Accordingly, the final `0x800` host bytes are structurally two distinct transfer units:

| Host-file range | Physical role | Confirmed installed target |
| --- | --- | --- |
| `0x060000..0x060013` | 20-byte meaningful secondary payload | `0x00406000..0x00406013` |
| `0x060014..0x0603FF` | zero padding to the recorded `0x400` span | `0x00406014..0x004063FF` |
| `0x060400..0x06077F` | zero prefix of the high transfer span | `0x005FFC00..0x005FFF7F` |
| `0x060780..0x0607FF` | `0x80`-byte final trailer/revision variant | `0x005FFF80..0x005FFFFF` |

The 20-byte secondary payload identifies the System 3 NEO package.

## Final trailer / revision variant

The final `0x80` bytes are not an independent executable image rooted at `0x005FFF80`. Canonical comparison against the primary System trailer shows:

- the complete first `0x40` bytes are identical;
- all four absolute jumps target the same main-System entrypoints;
- revision major, System name, and build-date string are identical;
- the revision minor is `0x04` rather than `0x11`;
- only the final twelve bytes differ otherwise: zero in the final trailer versus erased-value bytes in the primary trailer.

In total, only thirteen bytes of the `0x80`-byte objects differ.

No package literal points to the installed final-trailer base or its revision-record subaddress. The four jumps all target the main System image range rather than the high fixed-tail segment. This refutes interpreting the object as a self-contained firmware bank executing from the `0x005F...` region.

It is also distinct from the updater Small ROM: that component has its own `OS 3KNeo Small ROM` identity and a different execution base. Therefore the conservative classification is **final trailer / revision-metadata variant**. The source-level purpose of the embedded `3.04` minor revision remains **DESCONOCIDO / EVIDENCIA_INSUFICIENTE**; no fallback, recovery-bank, or alternate-firmware policy is asserted without an independent caller or source symbol.

## Evidence classification

**CONFIRMADO**

- canonical package size and hash verification;
- the three segment-table address/length pairs;
- updater chunk size `0x400` and whole-image streaming;
- successful physical flash count of 386 chunks for a 395264-byte package;
- Small ROM command `0x16` resetting its segment-map state;
- Small ROM command `0x17` storing destination bases and KiB-rounded byte spans in parallel arrays, in declaration order;
- programming-path selection of the current map entry, `base + length` boundary calculation, segment-index advance, and next-base load;
- the three host-file transfer boundaries `0x000000 / 0x060000 / 0x060400 / 0x060800`;
- placement of the secondary block at `0x00406000` and the high block at `0x005FFC00`;
- installed final-trailer address `0x005FFF80`;
- final-trailer code/entrypoint identity with the main System trailer and the exact thirteen-byte difference set.

The vendor/source-level names of the Small ROM mapper routines, internal state objects, and the reason for the `3.04` metadata variant remain **DESCONOCIDO / EVIDENCIA_INSUFICIENTE** and are not invented here.

## Refutation attempt

A compact-concatenation model in which the third payload starts immediately after the 20-byte secondary payload is refuted both by package geometry and by the Small ROM mapper. The mapper operates on recorded KiB-sized spans and advances to the next declared base only at a recorded segment boundary. The `0x3EC` bytes after the 20-byte payload therefore belong to the second segment's `0x400` transfer span.

Likewise, treating `0x060014..0x06077F` as one undifferentiated zero-fill region crosses a confirmed map boundary at host offset `0x060400`.

The high final trailer also cannot be treated as an independent executable bank: its absolute jumps leave the high segment and enter the same main-System routines used by the primary trailer.

## Consequence for the revision-name comparator

Elsewhere in the NEO 2013 firmware, an internal routine compares the NUL-terminated buffer at `0x00406000` against the `name` field of the fixed revision record and returns Boolean equality. With the Small ROM placement now directly established, `0x00406000` is **CONFIRMADO** as the installed address of the 20-byte secondary package payload. The exact Boolean equality contract is documented separately; no vendor symbol is asserted.

## Regression status

The original private package-layout regression was **EJECUTADA: 15/15 PASS**. A second private regression over the actual NEO Manager 3.9.3 Small ROM plus the canonical NEO 2013 package was **EJECUTADA: 33/33 PASS**. It verifies the binary identities, dispatcher signatures, segment-map reset/append mechanics, address/length arrays, KiB-to-byte conversion, programming-path boundary transitions, canonical segment table, and the three derived host-to-flash spans.

A third private canonical-package regression covering the final trailer's installed placement, exact primary/final byte differences, four absolute targets, metadata equality, and negative absolute-reference searches was **EJECUTADA: 21/21 PASS**.

ROM bytes and extensive disassembly remain private with the project evidence set. This public note contains only functional, structural, and contract-level conclusions.

Status: **PARCIAL_CERRADO** for source-level naming/policy; placement mechanism and final-trailer structural classification **CONFIRMADOS**. No ABI promotion.