# A0D4 / `SleepCentiseconds` closure

Status: **mechanically closed (confidence A)** under the source-first ABI audit.

## Contract

```c
void SleepCentiseconds(uint32_t centiseconds);
```

`syscall.c` assigns index 53 (`A0D4`) to `SleepCentiseconds`, and the historical public header declares the same prototype. The identity is additionally confirmed against all three canonical firmware generations rather than accepted from the source label alone.

## Firmware confirmation

The A0D4 handler is 0x4A bytes in AlphaSmart 3000 (2005), NEO (2005), and NEO (2013). After normalizing one relocated internal-state address, all three handlers are byte-identical; the normalized handler SHA-256 is:

`9f07da7c9ba575b0e266f2f842779a2f9ffb3b402163e01fccb462f4e1dc83de`

The handler reads one complete 32-bit ABI argument from the caller stack. Its ordinary software-timing path obtains the centisecond uptime counter, constructs a deadline from the requested delay, and busy-waits on that counter. Both internal calls on that path resolve to the same implementation used by `GetUptimeCentiseconds` (`A0E0`).

A second internal path is selected by a relocated firmware-state byte. It scales the low 16 bits of the delay by ten, invokes an unnamed private helper, and polls a hardware-status bit. The exact vendor meaning of that private state/helper is not established, so no public symbolic name is assigned to it.

The handler does not produce a path-independent result in D0. That firmware behavior agrees with the historical `void` prototype.

## Caller correlation

Direct absolute JSR references to A0D4 were found in all three firmware generations: 38 in AS3000 2005, 21 in NEO 2005, and 30 in NEO 2013. Concrete immediate arguments include values such as 1, 2, 20, 100, 200, 300, 400, 500, and 700, consistent with centisecond units.

An independent internal use also converts seconds to centiseconds before invoking this service, reinforcing the unit assignment without relying on the historical function name alone.

## Cross-generation result

The public ABI is unchanged across the three canonical generations:

```c
void SleepCentiseconds(uint32_t centiseconds);
```

The observable implementation is blocking/busy-waiting. Internal hardware-path details are intentionally left neutral where firmware semantics do not recover a vendor symbol.

## Regression status

A private static regression was **executed: 25/25 PASS**. It verifies canonical firmware hashes, exact handler size and fixed opcodes, the relocated state field, both calls to the centisecond uptime service, the private-helper relative target, cross-ROM normalized identity, direct-call counts, and representative immediate arguments.

Dynamic/emulator regression is specified but has not been executed for this closure.

## Evidence policy

Firmware images, raw handler bytes, extensive disassembly, and private workpapers remain outside the repository. This document records only the reconstructed functional contract and reproducible non-firmware-derived conclusions.
