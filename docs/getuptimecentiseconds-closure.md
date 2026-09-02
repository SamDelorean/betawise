# A0E0 / index 56 — `GetUptimeCentiseconds`

Status: **mechanically closed** (source-first audit, 2026-09-02).

## Public contract

```c
uint32_t GetUptimeCentiseconds(void);
```

BetaWise maps syscall index 56 / A0E0 to `GetUptimeCentiseconds`. Revalidation against the canonical AlphaSmart 3000 (2005), NEO (2005), and NEO (2013) firmware confirms the no-argument, 32-bit-return contract in all three generations.

The recovered early AlphaSmart `TimerModule.c` provides the source-first lineage: its timer tick is based on a 1 ms hardware timer and a 10 ms logical interval, and its `TimerGetTickCount()` protects the global timer count with a critical section and returns zero through a diagnostic path when the required timer mode is unavailable. The original DragonBall register header identifies Timer 1 compare at `TCMP1` and the live counter at `TCN1`.

## Confirmed behavior

The later OS3K firmware refines the early tick-count implementation. On the normal path A0E0:

1. enters a critical section;
2. loads a 32-bit elapsed-time accumulator;
3. reads the live 16-bit Timer 1 counter (`TCN1`) and adds it to that accumulator;
4. leaves the critical section; and
5. performs unsigned division by 10, returning the quotient in D0.

The corresponding timer ISR adds `TCMP1` to the same accumulator when an interval completes. Since the timer is configured on a 1 ms base, the internal expression is an elapsed-millisecond count. Thus the externally visible result is:

```text
centiseconds = floor((accumulated_milliseconds + TCN1) / 10)
```

A private state byte guards this calculation. When that state indicates the unavailable/disabled path, the handler invokes the firmware diagnostic route and returns zero. We intentionally do not assign a vendor name to that private state without an independent symbol.

## Cross-check against A0E4

A0E4 / `GetUptimeMilliseconds` is an immediate positive control. Its handler uses the same state test, critical-section pair, accumulator, and `TCN1` read, but returns the millisecond sum directly. A0E0 adds only the divide-by-10 conversion. This independently refutes the alternative interpretation that A0E0 simply exposes an already-centisecond internal counter.

## Cross-generation mechanics

The A0E0 handler is exactly `0x56` bytes in all three canonical ROMs. After neutralizing seven absolute-address relocation operands, the normalized handler is identical across AS3000 2005, NEO 2005, and NEO 2013 (normalized SHA-256 `ab7d2fba4e70bbf55d78fde3efcb749cf27d816127260ff930975ea840678acd`). The unsigned-division helper is byte-identical across the three generations and contains the expected `DIVU` path.

Firmware xrefs include two direct relative BSR calls and two absolute JSR calls per ROM. The two BSR calls are inside A0D4 / `SleepCentiseconds`, where the returned D0 value is used as the current-time reading for deadline construction and comparison. These callers require no arguments and consume a 32-bit scalar result.

## Confidence

- **CONFIRMED:** A0E0/index56 identity; `uint32_t GetUptimeCentiseconds(void)` ABI; output unit of 10 ms; normal-path formula; zero result on the guarded unavailable path; Timer 1 accumulator/partial-counter construction; cross-ROM handler equivalence.
- **STRONG INFERENCE:** direct functional lineage from the recovered early `TimerGetTickCount()` implementation to the later higher-resolution accumulator-plus-partial-counter implementation.
- **UNKNOWN:** original vendor name/semantic label for the private guard byte and diagnostic helper beyond their observed behavior.

## Regression

Static regression was **executed**, not merely specified: **50/50 checks passed**. It covers canonical ROM hashes, A0E0/A0E4 table resolution, handler boundaries, terminal return, state guard, critical-section flow, accumulator and `TCN1` accesses, addition, divisor 10, unsigned-division helper, direct JSR/BSR xrefs, and normalized cross-ROM identity.

Dynamic emulator-first validation remains **specified / not executed** and is not required for this mechanical closure.

Firmware bytes, full disassembly, and private reverse-engineering artifacts remain outside this repository by design.
