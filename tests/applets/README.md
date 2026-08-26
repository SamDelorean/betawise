# ABI probe applets

These applets are research and regression fixtures, not normal end-user applications.

Each probe should isolate one ABI question, keep unrelated parameters constant, and record expected observations in its README. When possible, results should be compared across:

- AlphaSmart 3000 hardware;
- NEO/NEO2 hardware;
- the emulator.

Compiler intermediates can be preserved with `SAVE_TEMPS=1` for caller-side ABI inspection.

Current probes:

- `DialogProbe` — A0F4 / dialog item indexing, IDs, and shortcut behavior.
- `BatteryProbe` — A138 / baseline interactive battery display.
