# DialogProbe — A0F4 baseline

Minimal regression/research applet for `DialogAddItem` and the surrounding dialog API.

The probe intentionally keeps `marker=' '` and `file_size=-1` constant and varies only text, caller ID, and shortcut key. It records the three `DialogAddItem` return values plus `DialogRun`, `DialogGetChoice`, `DialogGetChoiceId`, and `DialogGetItemId`.

Expected working hypothesis:

- `ALPHA` → choice 1 → id 100
- `BETA` → choice 2 → id 200
- `GAMMA` → choice 3 → id 300

The A/B/G shortcut behavior and visual rendering are experimental observations, not assumptions.

Open with `Cmd+H`. Test arrows + Enter, A/B/G, and Escape separately when possible. Record results independently for AS3000, NEO, and emulator.

Build from this directory with `make`. Use `make SAVE_TEMPS=1` when intermediate compiler artifacts are wanted for ABI analysis.

The baseline source should remain stable until it can be executed. Follow-up experiments are defined in [`PLAN.md`](PLAN.md) and deliberately isolate `A10C`, shortcut behavior, markers, `file_size`, and capacity/error cases in separate stages.
