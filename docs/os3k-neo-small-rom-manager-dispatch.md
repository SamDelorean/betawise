# NEO Small ROM Manager dispatcher and common block engine

This note records a direct, reproducible correlation of the canonical NEO Small ROM with the Manager protocol used by the principal OS. It complements the principal-OS applet-installation reconstruction and keeps firmware-specific policy separate from the shared transport envelope.

## Canonical image

The analyzed private firmware is `smallos3kneorom.os3kos` from NEO Manager 3.9.3.

- size: `0x581A` bytes
- SHA-256: `6402c40878ba28747642ce474361b1596555b2a1f137db79c7ea8bb5b35a9523`
- initial SSP vector: `0x0007FFF0`
- reset PC vector: `0x0040042A`

The reset-vector observation is useful for later emulator boot-map work but does not by itself establish the physical bank/overlay rule that selects Small ROM versus the principal OS.

## Small-ROM Manager dispatcher

The Small ROM contains a compact selector dispatcher whose explicit supported-selector list is:

`00 02 07 08 09 0A 0B 16 17 18 19`

The list is scanned forward while a reverse-indexed word table supplies PC-relative branch displacements. Reconstructing that mechanism yields these physical case entrypoints (file offsets equal runtime address minus `0x00400000`):

| Selector | File offset | Runtime | Direct observation |
| --- | ---: | ---: | --- |
| `0x00` | `0x1D1A` | `0x00401D1A` | version-response path |
| `0x02` | `0x1D6A` | `0x00401D6A` | block receive setup, response `0x42` |
| `0x07` | `0x1F8C` | `0x00401F8C` | sets exit/restart-related flag and returns `0x48` |
| `0x08` | `0x1F78` | `0x00401F78` | sets the same flag and returns `0x52` |
| `0x09` | `0x1FC2` | `0x00401FC2` | supported; detailed contract intentionally not promoted here |
| `0x0A` | `0x1DA2` | `0x00401DA2` | supported; detailed contract intentionally not promoted here |
| `0x0B` | `0x1DB4` | `0x00401DB4` | segment-aware staged-block program/commit, response `0x47` |
| `0x16` | `0x1C10` | `0x00401C10` | clears segment-map state, response `0x54` |
| `0x17` | `0x1C24` | `0x00401C24` | registers/erases segment context, response `0x55` |
| `0x18` | `0x1C04` | `0x00401C04` | returns `0x56` inside Small ROM |
| `0x19` | `0x1FA0` | `0x00401FA0` | supported; detailed contract intentionally not promoted here |

This is a firmware-specific namespace, not the 32-entry principal-OS Manager namespace. In particular, presence of `0x18` inside Small ROM means the selector remains accepted after the firmware-context transition; the transition semantics belong to the principal-OS side and must not be inferred solely from this Small-ROM acknowledgement path.

## Direct `0x02 -> raw completion -> 0x43` wiring

Selector `0x02` directly establishes the raw-data receive path:

- clears per-block state at Small-ROM RAM state `0x00001426`;
- latches the announced word at `0x00001428`;
- installs callback pointer `0x004020CA` into the receive-dispatch state;
- emits response `0x42`.

The callback entry is therefore mechanically fixed at:

- file `0x20CA`
- runtime `0x004020CA`

The callback compares the received quantity against the value latched by `0x02`; the successful path emits `0x43`, while a distinct mismatch/error path emits `0x8E`.

This closes the Small-ROM physical handoff:

`0x02 @ 0x00401D6A -> raw receive callback 0x004020CA -> 0x43`

and independently confirms the same two-stage block handshake reconstructed from the principal OS and host implementations.

## `0x0B` segment-aware commit/program stage

The `0x0B` branch begins at file `0x1DB4` / runtime `0x00401DB4`. Direct references tie it to the segment-map state prepared by `0x16/0x17`. Among the mechanically visible invariants:

- it references the active segment-state byte used by the mapper;
- it contains an explicit comparison against segment base `0x005FFC00`, one of the canonical NEO13 OS-package destinations;
- accepted programming paths emit `0x47`;
- the branch advances segment-related state as data are consumed/programmed.

Combined with the previously closed Small-ROM mapper mechanics—ordered segment registration, base+extent tracking, and transfer splitting at segment boundaries—this establishes `0x0B` as the commit/program backend for the staged block in the current OS-update context.

No vendor function name is assigned.

## `0x07` and `0x08`

The Small-ROM cases are compact and adjacent:

- `0x08 @ 0x00401F78` sets byte `0x00001C5C = 1` and returns `0x52`;
- `0x07 @ 0x00401F8C` sets the same byte and returns `0x48`.

Host protocol behavior identifies `0x08` as restart and uses `0x07` after the final programmed block. The shared flag strongly links both to leaving/completing the updater state, but the exact downstream policy represented by byte `0x00001C5C` remains unnamed pending a complete consumer graph. Thus the response codes and shared-state write are **CONFIRMED**, while a vendor-level flag name is **DESCONOCIDO**.

## Cross-firmware transport homology

The Small-ROM raw receive code is not merely semantically similar to the principal NEO13 OS. Direct byte comparison finds substantial exact common blocks in the raw transport path, including:

- 148 consecutive identical bytes between principal-OS file `0x2BAA..0x2C3D` and Small-ROM `0x2146..0x21D9`;
- 69 consecutive identical bytes between principal-OS file `0x2C40..0x2C84` and Small-ROM `0x21DC..0x2220`.

This is strong mechanical evidence that the same transport implementation lineage is reused across the two firmware contexts, while the programming destination/backend is specialized.

The resulting model is therefore:

```text
principal OS SmartApplet install
    0x06 destination setup
       -> common block transport
       -> principal-OS applet programming policy

principal OS -> Small ROM OS update
    0x18 context transition request
       -> Small ROM 0x16/0x17 destination map + erase setup
       -> common block transport
       -> Small-ROM segment-aware programming policy

common block transport
    0x02 announce(len, sum16) -> 0x42
    raw payload -> callback -> 0x43
    0x0B commit/program current staged block -> 0x47
    ...
    0x07 finish active transaction -> 0x48
```

## Verification

A Small-ROM-specific private static regression was executed against the canonical image and passed **40/40 assertions**. It verifies independently:

- size and SHA-256;
- SSP and reset-PC vectors;
- dispatcher selector load, upper bound, explicit 11-selector list, and all 11 reconstructed targets;
- response fingerprints for `0x18`, `0x16`, and `0x17`;
- `0x02` state reset, length latch, callback pointer, and `0x42`;
- raw callback entry, length comparison, `0x43` success and `0x8E` mismatch route;
- `0x0B` segment-state references, `0x005FFC00` comparison, `0x47`, and state advance;
- `0x07`/`0x08` shared flag plus `0x48`/`0x52`;
- two exact raw-transport homology blocks against canonical NEO13 principal OS.

This 40/40 run is separate from the principal-OS 111/111 applet-installation regression; neither result is counted as proof for assertions that belong only to the other firmware context.

## Status and remaining questions

**CONFIRMED / structurally and mechanically closed** for the Small-ROM dispatcher, `0x02` raw handoff, `0x0B` segment-aware commit role, `0x16/0x17` mapper setup, and `0x07`/`0x08` response/state writes.

Still unresolved or intentionally unnamed:

- complete source-level contracts for Small-ROM selectors `0x09`, `0x0A`, and `0x19`;
- full helper/consumer graph of flag `0x00001C5C`;
- complete error/status taxonomy inside `0x0B` and the raw transport;
- exact hardware bank/overlay rule that makes Small ROM visible/executable after the principal-OS transition;
- vendor names for internal functions and globals.

No A-line ABI promotion follows from this work. The demonstrated A-line frontier remains A470.
