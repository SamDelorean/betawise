# A0D8 / `SleepCentimilliseconds` closure

Status: **mechanically closed (confidence A)** under the source-first ABI audit.

## Contract

```c
void SleepCentimilliseconds(uint16_t centimilliseconds);
```

The historical syscall table assigns index 54 (`A0D8`) to `SleepCentimilliseconds`, and the public OS3K header declares the same prototype. The identity has now been revalidated against the canonical AS3000 2005, NEO 2005, and NEO 2013 firmware implementations.

## Firmware behavior

All three handlers are 0x22 bytes. They save a local counter register, compare a 16-bit counter directly with the low word of the argument slot, and execute a compact nested busy loop until the requested count is reached. An argument of zero returns immediately.

The inner delay loop contains the only material implementation difference between platforms:

- AS3000 2005: calibration count 2
- NEO 2005: calibration count 9
- NEO 2013: calibration count 9

After normalizing that one calibration constant, the handler is identical across all three generations. The normalized SHA-256 is:

`a250628681a0a7e25097535a7cbfb9c89ff235bf25ea451f87ba6dea10feee0e`

The routine explicitly clears D0 before returning, but the recovered historical ABI defines no return value, so the public contract remains `void`.

## Call correlation

No direct absolute JSR references were found inside the AS3000 2005 or NEO 2005 system ROMs. NEO 2013 contains three direct internal calls, each with the immediate argument 1000. This is consistent with the documented centimillisecond unit and does not change the ABI.

The negative 2005 system-ROM search is recorded as such; it does not imply that SmartApplets cannot invoke the service through the A-line syscall interface.

## Relationship to A0D4

A0D8 is not an alias or thin wrapper around `SleepCentiseconds`. It does not call A0D4 or the uptime service. It performs a local calibrated busy loop and consumes a 16-bit argument, whereas A0D4 uses a 32-bit centisecond argument and system timing state.

## Regression status

Private static regression: **executed, 20/20 PASS**. It verifies canonical hashes, exact handler size and ABI access pattern, the AS/NEO calibration constants, normalized cross-ROM identity, direct system-ROM call counts, and the three NEO 2013 immediate arguments.

Dynamic/emulator timing regression is specified but not yet executed.

## Evidence policy

Firmware images, raw handler dumps, and extended private disassembly remain outside the repository. This document contains only reconstructed contract and functional behavior suitable for publication.
