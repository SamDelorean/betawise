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
| `0x060780..0x0607FF` | `0x80`-byte fixed trailer | `0x005FFF80..0x005FFFFF` |

The 20-byte secondary payload identifies the System 3 NEO package. The last `0x80` bytes are the fixed trailer/revision-record homolog described separately in the structural map.

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
- installed fixed-trailer address `0x005FFF80`.

The vendor/source-level names of the Small ROM mapper routines and its internal state objects remain **DESCONOCIDO / EVIDENCIA_INSUFICIENTE** and are not invented here.

## Refutation attempt

A compact-concatenation model in which the third payload starts immediately after the 20-byte secondary payload is refuted both by package geometry and by the Small ROM mapper. The mapper operates on recorded KiB-sized spans and advances to the next declared base only at a recorded segment boundary. The `0x3EC` bytes after the 20-byte payload therefore belong to the second segment's `0x400` transfer span.

Likewise, treating `0x060014..0x06077F` as one undifferentiated zero-fill region crosses a confirmed map boundary at host offset `0x060400`.

## Consequence for the revision-name comparator

Elsewhere in the NEO 2013 firmware, an internal routine compares the NUL-terminated buffer at `0x00406000` against the `name` field of the fixed revision record and returns Boolean equality. With the Small ROM placement now directly established, `0x00406000` is **CONFIRMADO** as the installed address of the 20-byte secondary package payload. Calling that payload an identity/name object at source level remains conservative unless an independent vendor symbol is recovered.

## Regression status

The original private package-layout regression was **EJECUTADA: 15/15 PASS**. A second private regression over the actual NEO Manager 3.9.3 Small ROM plus the canonical NEO 2013 package was **EJECUTADA: 33/33 PASS**. It verifies the binary identities, dispatcher signatures, segment-map reset/append mechanics, address/length arrays, KiB-to-byte conversion, programming-path boundary transitions, canonical segment table, and the three derived host-to-flash spans.

ROM bytes and extensive disassembly remain private with the project evidence set. This public note contains only functional, structural, and contract-level conclusions.

Status: **PARCIAL_CERRADO** for source-level naming; placement mechanism **CERRADO A / CONFIRMADO**. No ABI promotion.