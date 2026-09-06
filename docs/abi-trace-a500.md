# A500 nominal coordinate (index 320)

Status: **structurally classified; no uniform syscall/ABI entity demonstrated**.

This document supersedes the earlier `SYS_A500 / ABI hole` interpretation. The expression `0xA000 + 4*index` is useful as a coordinate only after the firmware dispatcher and the physical structure at that coordinate have been established. It is not, by itself, proof that the coordinate denotes a system call.

## Dispatcher geometry

Direct firmware verification shows a real generational difference:

- AlphaSmart 3000 (2005) and NEO (2005) mask the A-line offset with `0x03FC`, giving a 256-entry mechanical dispatch window (`A000..A3FC`).
- NEO (2013) masks with `0x07FC`, giving a 512-entry mechanical dispatch window (`A000..A7FC`).

A mechanically indexable entry is still not automatically a valid callable handler: the target value must independently resolve to executable firmware and the contract must be established from code/callers.

## A500 classification by generation

| Platform | Structural disposition of nominal index 320 |
| --- | --- |
| AlphaSmart 3000 (2005) | Outside the real A-line dispatcher window; the coordinate lands inside the firmware's USB descriptor data block. |
| NEO (2005) | Outside the real A-line dispatcher window; the coordinate lands inside the firmware's USB descriptor data block. |
| NEO (2013) | Inside the larger mechanical A-line address window, but the bytes belong to a separate 32-entry function-pointer structure stored as swapped 16-bit halves. Reconstructing the entry yields a real internal 68k function homologous to the corresponding functions in the 2005 ROMs. |

Therefore there is **no single cross-generation callable object named `SYS_A500`** supported by the evidence. On the 2005 systems the coordinate is not an A-line slot at all. On NEO 2013 it intersects another firmware structure; being indexable by the broad dispatcher mask does not establish an A500 syscall contract.

## Secondary 32-entry function table

A homologous 32-entry table exists in all three canonical ROM generations. Its entries are stored as two 16-bit halves in the opposite order from a normal big-endian 32-bit pointer. Recombining those halves produces valid, closely homologous 68k code addresses across the three firmware generations.

The table's structure and cross-generation correspondence are confirmed. Its exact vendor name and functional role are intentionally left **unknown** pending source-first correlation with historical USB/hardware sources and concrete xrefs. No subsystem name is promoted solely from spatial proximity.

## Caller evidence

The prior negative search remains useful secondary evidence: the official SmartApplet corpus was re-established as 16 EXTENDED + 14 LEGACY + 11 NO_TABLE = 41/41, and no executable A500 trap/caller was promoted. That negative result no longer serves as the primary reason for classifying A500, because direct structural analysis now explains why the nominal coordinate is not a uniform ABI entry.

## Methodological consequence

Future high-range work must maintain two separate views:

1. the functional ABI/syscall map; and
2. the structural ROM map (dispatcher windows, internal code, function-pointer tables, descriptors/resources, embedded payloads, and unknown regions).

A nominal `Axxx` label may be retained as a coordinate, but it must not be treated as a syscall identity until the corresponding firmware structure and callable handler have both been demonstrated.

Private ROM bytes, detailed disassembly, absolute per-image structural offsets, and reverse-engineering workpapers remain outside this repository.
