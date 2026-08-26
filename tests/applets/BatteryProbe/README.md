# BatteryProbe — A138 baseline

Minimal regression/research applet for `ShowBatteryPercentage` / A138.

The baseline invokes only:

```c
ShowBatteryPercentage(0);
```

using `Ctrl+Cmd+B`. This mirrors the known interactive system use and avoids mixing baseline validation with timed-mode experiments.

Record:

- exact screen contents;
- whether the call waits for a key;
- which key returns, if applicable;
- whether control returns cleanly to BatteryProbe;
- any difference among AS3000, NEO, and emulator.

The firmware analysis documented in `docs/ABI.md` already supports `time_seconds > 0` as a timed display mode. A later probe can exercise those values separately.

Build from this directory with `make`. Use `make SAVE_TEMPS=1` when intermediate compiler artifacts are wanted for ABI analysis.
