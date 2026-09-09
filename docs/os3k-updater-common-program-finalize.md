# OS3K updater: common program/finalize commands 0x0B and 0x07

## Status

`CERRADO A` for the shared updater protocol role described here. This document does **not** assign vendor-internal function names and does not promote these protocol selectors into the A-line ABI.

## Source-first correction

A previous shorthand described selector `0x07` as an applet-write/finalize operation. That is too narrow.

Reproducible host-side evidence from `ioma8/neo-re`, pinned at commit `732814871fe493e80ed1ab1f959d8dad174ee540`, shows that the same updater primitives are used for both SmartApplet installation and full OS-image programming:

- `alpha-core/src/neo_client.rs` calls the same `write_chunks_and_program()` routine for `install_smart_applet_with_progress()` and `install_neo_os_image_with_progress()`.
- That common routine announces each chunk with selector `0x02`, sends the raw bytes, then sends selector `0x0B` and requires response `0x47`.
- Both SmartApplet and OS-image flows subsequently send the same selector `0x07` and require response `0x48`.
- The function names used by neo-re (`program_applet_command`, `finalize_applet_update_command`) are analyst nomenclature, not demonstrated AlphaSmart vendor symbols.

Therefore the narrow applet-only interpretation is refuted. The demonstrated cross-flow contract is:

| Phase | Selector / transfer | Response | Demonstrated role |
|---|---:|---:|---|
| announce chunk | `0x02` | `0x42` | provide accepted chunk length/checksum context |
| transfer chunk | raw bytes | `0x43` | raw payload accepted |
| program/commit current chunk | `0x0B` | `0x47` | common updater block-program/commit step |
| finalize transaction | `0x07` | `0x48` | common updater finalization step |

Selector `0x06` remains distinct: the reproduced host path uses it as the begin step for SmartApplet installation. OS-image programming instead enters Small ROM and prepares OS segments (`0x18`, `0x16`, `0x17`) before converging on the common chunk/program/finalize engine.

## Firmware correlation

This semantic correction reuses already executed canonical-firmware evidence rather than claiming a new binary regression. Existing private regressions establish the mechanical transaction in AS3000, NEO05 and NEO13 main OS (`111/111 PASS`) and the homologous Small ROM block engine across generations.

Accordingly:

- `0x0B` = common updater **program/commit current accepted block**: `CONFIRMADO` at protocol-role level.
- `0x07` = common updater **finalize transaction**: `CONFIRMADO` at protocol-role level.
- Exact vendor names, internal helper names, and generation-specific policy details: `DESCONOCIDO` / `NO_RESUELTO` where not independently demonstrated.
- No new A-line syscall is implied. The A-line ABI frontier remains unchanged.

## Reproducibility reference

Primary correlation source: `ioma8/neo-re` commit `732814871fe493e80ed1ab1f959d8dad174ee540`, especially `alpha-core/src/protocol.rs` and `alpha-core/src/neo_client.rs`.

Firmware binaries, full disassembly and private regression artifacts are intentionally not included in this repository.
