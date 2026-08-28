# System 3 / OS3K battery display API — A138 closure

This note records the current contract for A138, exposed by BetaWise as:

```c
void ShowBatteryPercentage(uint8_t time_seconds);
```

The operation is closed at ABI/parameter-semantics level with confidence **A**
for the analyzed AS3000 and NEO System 3 generations. Historical genealogy is
also strong, but the historical implementation used a different parameter
meaning and must not be treated as ABI-identical.

## 0. SDK consolidation status

A138 reached this SDK-consolidation pass **after** the reverse-engineering work
had already closed its mechanical contract with confidence A. This pass does
not repeat the ROM/disassembly investigation; it audits the result against the
existing BetaWise public declaration and A-line stub and records the developer
contract that should be used going forward.

The existing BetaWise implementation already matches the reconstructed ABI:

```c
void ShowBatteryPercentage(uint8_t time_seconds);
```

and `os3k/syscall.c` maps A-line index 78 directly to
`ShowBatteryPercentage`. No additional BetaWise wrapper is required: unlike
`ClearScreen`, the public SDK symbol is itself the direct firmware service.

Consolidation state:

- mechanical contract: **A**, received from the reverse-engineering process;
- SDK declaration/stub: **implemented and audited**;
- canonical developer documentation: **this document**;
- dynamic emulator/hardware regression: **pending**.

If later primary evidence changes the upstream mechanical contract, this
consolidated SDK contract must be reconciled rather than silently preserving an
obsolete conclusion.

## 1. Purpose

A138 presents battery status. It has two user-visible modes selected by the
single byte argument:

- `time_seconds == 0`: show the full battery-status presentation and wait for a
  key before returning;
- `time_seconds > 0`: show the battery graphic/status transiently for the
  requested number of seconds, then return.

The argument is read as an **8-bit value** by the firmware.

## 2. Timed mode (`time_seconds > 0`)

The firmware converts the byte argument to centiseconds:

```text
sleep_time = time_seconds * 100
```

and passes that value to the same implementation used by A0D4 /
`SleepCentiseconds` after building/displaying the battery indicator.

Therefore the public name `time_seconds` is not an inference from UI behavior;
it follows directly from the handler's unit conversion.

The effective input range is the natural `uint8_t` range. Extreme durations
are not required for normal regression coverage.

## 3. Full-status mode (`time_seconds == 0`)

With zero, the routine does not take the timed-sleep exit. Instead it builds
the complete battery-status screen, obtains localized status text, presents the
capacity/percentage information and ultimately calls `WaitForKey` before
returning.

In the analyzed NEO 2013 firmware, the localized string used by this path is:

```text
The battery capacity is at 
```

This mode corresponds to the normal interactive battery-information screen.

## 4. Percentage calculation and internal helpers

A138 invokes internal routines associated with A120, A128 and A130. The value
returned by the A128 path participates in a calculation equivalent to:

```text
percentage = 100 - value
```

The individual public meanings/names of A120/A128/A130 are **not** sufficiently
closed and remain intentionally unnamed. A138's contract does not require
inventing names for its internal helpers.

## 5. Cross-ROM evidence

The A138 handler was identified through validated A-line tables in all three
reference firmware images:

| Firmware | Handler |
| --- | ---: |
| AS3000 System 3, Nov 2005 | `0x4D1A32` |
| NEO System 3, Nov 2005 | `0x5D5A60` |
| NEO/System 3.15, Jul 2013 | `0x426EC0` |

The same byte-argument and timed/full-mode logic is present across these
versions. The surrounding addresses and internal helper locations differ by
firmware and are not part of the portable ABI.

The A-line tables used for identification were cross-checked against multiple
known traps rather than accepted from a single structural match.

## 6. Official caller evidence

Official ControlPanel code contains an A138 call that pushes a zero argument.
That exactly selects the interactive full-status path described above and is
consistent with the native battery-status shortcut behavior.

This caller evidence independently confirms that zero is an intentional public
mode, not merely a degenerate timer value.

## 7. Historical genealogy

Original AS3000 object/source material contains the symbol:

```text
PowerShowBatteryPercentage
```

This provides strong nominal and functional continuity with modern A138.
However, the early implementation's parameter was used as a **display row**
(`ubLine`), not a time in seconds. The early simulator path positioned the
cursor and displayed battery text rather than implementing the later timed
behavior.

Therefore the correct historical conclusion is:

```text
PowerShowBatteryPercentage  ->  later A138 / ShowBatteryPercentage
```

with an **evolved ABI**. The old parameter meaning must not be copied into the
modern SDK.

## 8. Developer usage

For the full interactive status screen:

```c
ShowBatteryPercentage(0);
```

The call waits for a key before returning, so do not use this mode in a path
that must remain non-blocking.

For a transient display, for example two seconds:

```c
ShowBatteryPercentage(2);
```

The firmware handles the delay internally through `SleepCentiseconds`.

## 9. Side effects and return

The reconstructed public prototype is `void`. No contractual return value is
used by the analyzed callers/implementation.

The routine owns the presentation flow while active and, in zero mode, consumes
a key through `WaitForKey`. Application code should therefore treat it as a UI
operation rather than as a pure battery-percentage query.

A138 should not be used when the application only needs a numeric battery value;
its internal A120/A128/A130 helpers require separate reconstruction before such
a lower-level API can be documented safely.

## 10. Relationship to C and the BetaWise runtime

`ShowBatteryPercentage` is **not** an ISO C or hosted-libc facility. BetaWise
exposes it because `libos3k.a` maps the C call directly to the System 3 A-line
trap. There is no standard-C equivalent that preserves the same firmware UI,
keyboard wait, timing path or hardware-specific presentation semantics.

Accordingly, application code should call the SDK service directly rather than
substituting terminal escape sequences, `stdio` output or a host-side battery
API. Such alternatives would describe a different runtime contract.

## 11. Regression status

A BatteryProbe baseline was created during the reconstruction work with the
safe known call:

```c
ShowBatteryPercentage(0);
```

The existence of that probe is an executable specification, not an assertion
that all emulator/hardware tests have already passed.

Minimum useful regressions are:

- `0` -> full status presentation and WaitForKey exit;
- `1` -> transient display followed by approximately one second of firmware
  sleep;
- `2` -> same path with two-second sleep;
- trace confirms the `time_seconds * 100` value passed to A0D4;
- AS3000 and NEO render/return without ABI divergence for the same public mode.

Further characterization of A120/A128/A130 is a separate milestone and must not
be conflated with the already closed A138 interface.
