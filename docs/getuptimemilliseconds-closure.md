# A0E4 / index 57 — `GetUptimeMilliseconds`

Status: **mechanically closed** (source-first audit, 2026-09-02).

## Public contract

```c
uint32_t GetUptimeMilliseconds(void);
```

BetaWise maps syscall index 57 / A0E4 to `GetUptimeMilliseconds`. Revalidation against the canonical AlphaSmart 3000 (2005), NEO (2005), and NEO (2013) firmware confirms the no-argument, 32-bit-return contract.

The recovered early AlphaSmart timer source establishes the hardware lineage: Timer 1 is configured on a 1 ms base and the logical timer count is read under a critical section. The original DragonBall register header identifies Timer 1 compare as `TCMP1` and the live counter as `TCN1`. A0E0 / `GetUptimeCentiseconds`, independently closed immediately before this entry, provides a differential control because it reads the same internal value and divides it by ten.

## Confirmed behavior

On its normal path A0E4:

1. enters a critical section;
2. loads a 32-bit elapsed-time accumulator;
3. reads the live 16-bit `TCN1` value, zero-extends it, and adds it to the accumulator;
4. leaves the critical section; and
5. returns the resulting 32-bit value directly in D0.

The corresponding timer ISR adds `TCMP1` to the same accumulator at each completed interval. With the timer configured on a 1 ms base, both the completed-period accumulator and the current partial count are in milliseconds:

```text
milliseconds = accumulated_milliseconds + TCN1
```

A private state byte guards the read. On the unavailable/disabled path the firmware invokes its diagnostic route and returns zero. No vendor name is assigned to that private state without an independent symbol.

## Differential check against A0E0

A0E0 uses the same state guard, critical-section pair, elapsed-time accumulator, `TCN1` read, and addition. It then performs unsigned division by 10 to produce centiseconds. A0E4 omits that conversion and returns the underlying millisecond value directly. This refutes the alternatives that A0E4 returns centiseconds or merely the current hardware counter.

## Cross-generation mechanics

The A0E4 handler is exactly `0x4E` bytes in all three canonical ROMs. After neutralizing six absolute-address relocation operands, the normalized handler is identical across AS3000 2005, NEO 2005, and NEO 2013 (normalized SHA-256 `6402566c325ac856207add7f54f24a0f2bb0e539f798fb958b2bf37f45f4fac0`).

Direct absolute-long JSR searches found 11 firmware callers in AS3000 2005, 15 in NEO 2005, and 15 in NEO 2013. Representative callers consume D0 as a temporal scalar for baselines, deadline arithmetic, storage, and later comparisons. No caller requires arguments.

## Confidence

- **CONFIRMED:** A0E4/index57 identity; `uint32_t GetUptimeMilliseconds(void)` ABI; millisecond output; accumulator-plus-partial-counter formula; guarded zero-return path; cross-ROM handler equivalence.
- **STRONG INFERENCE:** direct functional lineage from the recovered early TimerModule implementation to this later high-resolution OS3K time reader.
- **UNKNOWN:** original vendor names/semantic labels for the private state byte and diagnostic helper beyond their observed behavior.

## Regression

Static regression was **executed**, not merely specified: **47/47 checks passed**, covering canonical ROM hashes, A0E4/A0E8 vector resolution, exact handler boundaries and terminal return, state guard, critical-section flow, accumulator and `TCN1` accesses, addition, absence of A0E0's divide-by-10 conversion, direct caller counts, and normalized cross-ROM identity.

Dynamic emulator-first validation remains **specified / not executed** and is not required for this mechanical closure.

Firmware bytes, full disassembly, and private reverse-engineering artifacts remain outside this repository by design.
