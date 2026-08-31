# SYS_A3D0 — forward-sweep disposition

Status: **mechanically reconstructed; callable contract blocked on return type**.

## Generational availability

- AlphaSmart 3000 (2005): A-line slot contains `0x5A06004E`, not a demonstrated callable pointer.
- NEO (2005): A-line slot contains `0x7F08005E`, not a demonstrated callable pointer.
- NEO (2013): A-line slot resolves to a real handler.

No callable contract is assigned to the 2005 generations.

## NEO 2013 mechanical ABI

Primary firmware analysis demonstrates two physical 32-bit argument slots:

1. a full-width pointer, used as the source of a bounded byte copy;
2. a value whose low 16 bits are consumed as the byte count.

The handler deliberately defines the complete `D0` register on every exit. Mechanically it yields either the zero-extended low-16-bit count on its success path or the literal values `8`, `9`, or `10` on other paths. These literals are recorded only as observed numeric results; they are **not** promoted to named errors or an enum, and they numerically overlap possible successful lengths.

One direct NEO 2013 firmware caller has been demonstrated. It builds exactly two argument slots, removes eight argument bytes after the call, and does **not** consume `D0` before transferring control elsewhere. The official SmartApplet corpus does not physically expose an A-line A3D0 slot.

Consequently the firmware proves the argument shape and mechanical `D0` behavior, but available evidence does not independently prove the intended C return type, width, or signedness. This repository therefore does **not** publish a callable `SYS_A3D0` prototype.

## Evidence discipline

The private workpapers retain ROM hashes, exact vector/file/runtime mappings, the complete NEO 2013 disassembly and CFG, helper/global correlation, direct-caller reconstruction, adversarial analysis, and an executed static regression. Firmware images and extensive disassembly remain private and are not reproduced here.

Static regression: **PASS**. Dynamic/emulator-first regression: **not executed**.
