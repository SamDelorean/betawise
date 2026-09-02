# A0DC / index 55 — `GetUptimeSeconds`

Status: **mechanically closed** (source-first audit, 2026-09-02).

## Public contract

```c
uint32_t GetUptimeSeconds(void);
```

The historical BetaWise SDK maps syscall index 55 / A0DC to `GetUptimeSeconds`. Revalidation against the canonical AlphaSmart 3000 (2005), NEO (2005), and NEO (2013) firmware confirms that all three entry handlers implement the same no-argument, 32-bit-return contract.

A stronger source-first anchor is also available in the recovered early AlphaSmart source tree: `TimerModule.c` implements `TimerGetClockSeconds(void)` and explicitly describes it as the number of seconds since power-on. Its algorithm reads the DragonBall RTC day/time fields and converts day/hour/minute/second to a scalar second count. The same module initialization routine zeros the RTC day and time registers. This historical implementation mechanically matches the canonical A0DC firmware algorithm and initialization strategy.

## Confirmed behavior

The firmware reads the DragonBall RTC day counter and time-of-day fields, masks/extracts day, hour, minute, and second, and returns their scalar total in seconds:

```text
seconds = day * 86400 + hour * 3600 + minute * 60 + second
```

The entry handler is read-only with respect to memory/MMIO and returns the value in D0. The complete A0DC entry routine is mechanically identical across the three audited firmware generations; only its ROM location differs.

A separate initialization path in the firmware clears the RTC day/time counters and is linked from early startup. Internal callers then use A0DC values as baselines and deadlines. Together with the recovered `TimerGetClockSeconds` source, this confirms the historical uptime interpretation for cold-initialized operation. We deliberately do **not** claim that every possible software-reset path necessarily clears the counter, because the broader initialization path is conditional.

## Caller evidence

Direct absolute-long JSR searches found 33 callers in AS3000 2005, 28 in NEO 2005, and 42 in NEO 2013. Representative uses store a baseline/deadline, add whole-second constants, and compare later readings. No caller contradicts the no-argument / `uint32_t` return contract.

## Confidence

- **CONFIRMED:** A0DC/index55 identity; `uint32_t GetUptimeSeconds(void)` ABI; scalar-seconds construction; historical uptime semantics for the initialized timer model; read-only A0DC handler; same handler mechanics across AS3000 2005, NEO 2005, and NEO 2013.
- **UNKNOWN:** whether every reset/restart mode clears those counters.

## Regression

Static regression was **executed**, not merely specified: **37/37 checks passed**, covering canonical ROM hashes, table-to-handler resolution, complete cross-ROM handler identity, RTC-field extraction/masks, read-only behavior, direct caller searches, and the independent initialization clear of the RTC day/time counters.

Firmware bytes, full disassembly, and private reverse-engineering artifacts remain outside this repository by design.
