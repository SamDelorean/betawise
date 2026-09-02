# A0DC / index 55 — `GetUptimeSeconds`

Status: **mechanically closed** (source-first audit, 2026-09-02).

## Public contract

```c
uint32_t GetUptimeSeconds(void);
```

The historical BetaWise SDK maps syscall index 55 / A0DC to `GetUptimeSeconds`. Revalidation against the canonical AlphaSmart 3000 (2005), NEO (2005), and NEO (2013) firmware confirms that all three entry handlers implement the same no-argument, 32-bit-return contract.

## Confirmed behavior

The firmware reads the DragonBall RTC day counter and time-of-day fields, masks/extracts day, hour, minute, and second, and returns their scalar total in seconds:

```text
seconds = day * 86400 + hour * 3600 + minute * 60 + second
```

The entry handler is read-only with respect to memory/MMIO and returns the value in D0. The complete A0DC entry routine is mechanically identical across the three audited firmware generations; only its ROM location differs.

A separate initialization path in the firmware clears the RTC day/time counters and is linked from early startup. Internal callers then use A0DC values as baselines and deadlines. This supports the historical `GetUptimeSeconds` identity for cold-initialized operation. We deliberately do **not** claim that every possible software-reset path necessarily clears the counter, because the broader initialization path is conditional.

## Caller evidence

Direct absolute-long JSR searches found 33 callers in AS3000 2005, 28 in NEO 2005, and 42 in NEO 2013. Representative uses store a baseline/deadline, add whole-second constants, and compare later readings. No caller contradicts the no-argument / `uint32_t` return contract.

## Confidence

- **CONFIRMED:** A0DC/index55 identity; `uint32_t GetUptimeSeconds(void)` ABI; scalar-seconds construction; read-only A0DC handler; same handler mechanics across AS3000 2005, NEO 2005, and NEO 2013.
- **STRONG INFERENCE:** effective uptime semantics after cold initialization clears the RTC counters.
- **UNKNOWN:** whether every reset/restart mode clears those counters.

## Regression

Static regression was **executed**, not merely specified: **37/37 checks passed**, covering canonical ROM hashes, table-to-handler resolution, complete cross-ROM handler identity, RTC-field extraction/masks, read-only behavior, direct caller searches, and the independent initialization clear of the RTC day/time counters.

Firmware bytes, full disassembly, and private reverse-engineering artifacts remain outside this repository by design.
