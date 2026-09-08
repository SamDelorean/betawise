# NEO System 3 host-package secondary segment layout

This note records a structural result for the NEO System 3 OS update package. It intentionally separates **host-file coordinates** from **installed flash addresses** and does not promote any A-line ABI entry.

## Scope

The canonical NEO 2013 package has length `0x60800` bytes. Its segment table declares three destinations:

| Segment | Installed address | Declared length |
| --- | ---: | ---: |
| Main System image | `0x00410000` | `0x00060000` |
| Secondary identity/name area | `0x00406000` | `0x00000014` |
| High fixed-tail area | `0x005FFC00` | `0x00000400` |

The updater implementation erases those declared destinations and then programs the **whole package** in ordered `0x400`-byte chunks. A documented successful physical-device flash transferred 395264 bytes as 386 chunks across the three declared segments.

## Host-file partition

Rounding each declared segment length to the updater's `0x400` transfer/erase quantum gives:

```text
0x60000 -> 0x60000
0x00014 -> 0x00400
0x00400 -> 0x00400
                     total = 0x60800
```

This fixes the host-package transfer-unit boundaries at:

```text
0x000000
0x060000
0x060400
0x060800
```

Accordingly, the final `0x800` bytes are structurally two distinct transfer units rather than one continuous padding object:

| Host-file range | Physical role | Installed target under ordered segment placement |
| --- | --- | --- |
| `0x060000..0x060013` | 20-byte meaningful secondary payload | `0x00406000..0x00406013` |
| `0x060014..0x0603FF` | zero padding to one `0x400` transfer unit | remainder of the secondary transfer unit |
| `0x060400..0x06077F` | zero prefix of the high transfer unit | `0x005FFC00..0x005FFF7F` |
| `0x060780..0x0607FF` | 0x80-byte fixed trailer | `0x005FFF80..0x005FFFFF` |

The 20-byte secondary payload identifies the System 3 NEO package. The last `0x80` bytes are the fixed trailer/revision-record homolog already described separately in the structural map.

## Evidence classification

**CONFIRMED**

- canonical package size and hash verification;
- the three segment-table address/length pairs;
- updater chunk size `0x400` and whole-image chunk streaming;
- successful physical flash count of 386 chunks for a 395264-byte package;
- the host-file boundaries and zero-padding geometry shown above;
- the final trailer's host offset `0x060780`.

**INFERENCIA FUERTE**

- ordered target placement of host block `0x060000..0x0603FF` into the declared `0x00406000` segment;
- ordered target placement of host block `0x060400..0x0607FF` into the declared `0x005FFC00` segment;
- therefore the installed fixed trailer address `0x005FFF80` for this package.

The remaining gap is deliberately narrow: direct decoding of the Small ROM's segment-to-programming-address mapper would raise the ordered placement itself from strong inference to confirmed mechanism.

## Refutation attempt

A compact-concatenation model in which the third payload starts immediately after the 20-byte secondary payload is inconsistent with the actual package geometry. The package contains exactly `0x3EC` bytes of zero padding after those 20 bytes, completing a `0x400` transfer unit. The rounded segment sizes then sum exactly to the complete package size (`0x60800`).

Likewise, treating `0x060014..0x06077F` as one undifferentiated zero-fill region hides a demonstrated physical boundary at host offset `0x060400`.

## Consequence for the revision-name comparator

Elsewhere in the NEO 2013 firmware, an internal routine compares the NUL-terminated buffer at `0x00406000` against the `name` field of the fixed revision record and returns Boolean equality. With the secondary segment correlation above, interpreting the buffer as the installed package identity/name payload is **INFERENCIA FUERTE**. The comparison behavior itself is mechanically confirmed; no vendor symbol is asserted for that routine.

## Regression status

A private canonical-ROM regression covering package hash/size, segment table, transfer-unit arithmetic, tail boundaries, zero padding, trailer placement arithmetic, and the compact-layout refutation was **EJECUTADA: 15/15 PASS**. The regression and ROM-derived evidence remain private with the project evidence set; this public note contains only structural conclusions and reproducible contract-level facts.

Status: **PARCIAL_CERRADO**. No ABI promotion.