# A500 nominal coordinate (index 320)

Status: **structurally classified; no uniform syscall/ABI entity demonstrated**.

This document supersedes both the earlier `SYS_A500 / ABI hole` interpretation and the intermediate interpretation that treated the NEO 2013 bytes as an aligned ordinal in a word-swapped function-pointer table. The expression `0xA000 + 4*index` is useful as a coordinate only after the firmware dispatcher and the physical structure at that coordinate have been established. It is not, by itself, proof that the coordinate denotes a system call.

## Dispatcher geometry

Direct firmware verification shows a real generational difference:

- AlphaSmart 3000 (2005) and NEO (2005) mask the A-line offset with `0x03FC`, giving a 256-entry mechanical dispatch window (`A000..A3FC`).
- NEO (2013) masks with `0x07FC`, giving a 512-entry mechanical dispatch window (`A000..A7FC`).

A mechanically indexable coordinate is still not automatically a valid callable handler: its physical contents and callable contract must be established independently.

## A500 classification by generation

| Platform | Structural disposition of nominal index 320 |
| --- | --- |
| AlphaSmart 3000 (2005) | Outside the real A-line dispatcher window; the coordinate lands inside USB descriptor data. |
| NEO (2005) | Outside the real A-line dispatcher window; the coordinate lands inside USB descriptor data. |
| NEO (2013) | Inside the larger mechanical A-line address window, but the four-byte coordinate is **not aligned to a function-pointer entry**. It crosses the boundary between two adjacent entries in an internal USB control-request dispatch table. Both entries are the same reserved-request handler, which caused the intermediate alignment error to look self-consistent. |

Therefore there is **no single cross-generation callable object named `SYS_A500`** supported by the evidence. On the 2005 systems the coordinate is not an A-line slot at all. On NEO 2013 it intersects another firmware structure out of phase with the A-line grid; mechanical indexability does not establish an A500 syscall contract.

## USB control-request dispatch tables

Source-first correlation with historical AlphaSmart USB source identifies the homologous 32-entry structure in all three canonical firmware generations as two contiguous 16-entry arrays:

- `DeviceRequest[16]` for standard USB requests; and
- `ClassRequest[16]` for class requests.

The historical `USB_ControlHandler()` selects between these arrays from the request type and indexes them using the masked USB request number. The sequence of concrete request handlers and reserved entries in source matches the firmware table pattern across all three generations.

**Correction:** the table entries are ordinary native 68k big-endian 32-bit pointers. The prior `[low16][high16]` / word-swapped interpretation resulted from beginning the table two bytes too early. Once correctly aligned, no half-word recombination is required.

At the nominal NEO 2013 A500 coordinate, the four-byte A-line-grid read begins halfway through one `DeviceRequest` entry and ends halfway through the next. Those two adjacent entries both select `USB_Reserved`; their identical pointer values made the misaligned four-byte sequence appear to decode back into the same valid handler when the 16-bit halves were swapped. That apparent confirmation is therefore refuted as an alignment artefact.

The exact per-image table offsets, ROM bytes, and detailed disassembly remain in the private reverse-engineering record rather than this repository.

## Caller evidence

The prior negative search remains useful secondary evidence: the official SmartApplet corpus was re-established as 16 EXTENDED + 14 LEGACY + 11 NO_TABLE = 41/41, and no executable A500 trap/caller was promoted. That negative result is not the primary classification argument; direct structural analysis explains why the nominal coordinate is not a uniform ABI entry.

## Confidence

- **CONFIRMED:** no uniform cross-generation `SYS_A500` contract is demonstrated.
- **CONFIRMED:** the homologous 32-entry structure is the USB control-request dispatch structure corresponding to historical `DeviceRequest[16]` + `ClassRequest[16]`.
- **CONFIRMED:** its entries are native big-endian 32-bit pointers, not word-swapped values.
- **CONFIRMED:** nominal NEO 2013 A500 crosses two adjacent reserved-request entries rather than identifying an aligned ordinal.
- **PROVISIONAL:** individual firmware helper semantics below the request-handler entrypoints remain subject to direct code reconstruction before vendor-level names are propagated beyond source-correlated functions.

## Methodological consequence

Future high-range work must maintain two separate views:

1. the functional ABI/syscall map; and
2. the structural ROM map (dispatcher windows, internal code, function-pointer tables, descriptors/resources, embedded payloads, and unknown regions).

A nominal `Axxx` label may be retained as a coordinate, but it must not be treated as a syscall identity until the corresponding firmware structure and callable handler have both been demonstrated. In particular, a structure can be offset from the A-line grid, so a nominal four-byte coordinate may cross two real entries rather than coincide with either one.
