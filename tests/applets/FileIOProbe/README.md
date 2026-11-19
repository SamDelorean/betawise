# FileIOProbe — dynamic regression for the binary File API

Executes the dynamic cases that the source-first closures list as
**SPECIFIED / NOT EXECUTED** for A190, A194, A198, A19C, A1C8 and A1CC.
A1E8 creates a private file `FIOPROBE` (group 0, min 0, requested 0x200) and
A1EC removes it at the end, so a passing run leaves no file behind.

Runs on `MSG_SETFOCUS`. Screen:

```text
FILEIO <failures>/<checks> fail
ids <first failing check numbers>
tok <A1E8 return>
```

A full pass reads `FILEIO 0/45 fail`. If check 1 fails, `tok` shows the raw
A1E8 status (-9 table full, -10 group, -12 allocation, -13 name, -15 duplicate).

## Names

Neutral names follow the closures. Proposed names are given in comments only,
taken from isotherm/betawise `os3k/os3k_file.h`: A190 `FileSeek`,
A194 `FileTell`, A1E8 `FileCreate`, A1EC `FileDelete`. A190/A194 are declared
locally in the probe because `os3k.h` does not declare them yet.

## Checks

| # | Call | Expected | Source |
|---|---|---|---|
| 1 | A1E8 create | token > 0 | A1E8 closure |
| 2–3 | A1C8 open(tok,1); A194 | returns tok; cursor 0 | A1C8, A194 |
| 4 | A198 "ABCDE" mode 1 | 5 | A198 append |
| 5 | A194 | 0 — append does not move cursor | A198 (isotherm states it explicitly) |
| 6 | A190(0, end) | 5 | A190 origin 2 |
| 7–11 | A190 start; A19C 3 then 10 | "ABC", cursor 3; "DE" (EOF clamp), cursor 5 | A19C |
| 12 | A19C at EOF | 0 | A19C |
| 13–14 | A19C NULL buffer | −6, cursor unchanged | A19C |
| 15–18 | seek 1; A198 "xy" mode 2 | 2; cursor 3; size 7 | A198 insert |
| 19–24 | seek 5; A198 "QRS" mode 3; read all | 3; cursor 8; size 8; "AxyBCQRS" | A198 overwrite extends EOF |
| 25 | A198 mode 4 | −5 | A198 invalid mode |
| 26–27 | A198 count 0 | 0; size unchanged | A198 |
| 28–30 | seek 2; A190 origin 3 | −2; cursor stays 2 | A190 invalid origin |
| 31–34 | A190 −100 cur / 100 start / −3 end / +2 cur | 0 / 8 / 5 / 7 | A190 clamps and origins |
| 35 | A194(token 0) | 7 | A194 active token |
| 36–40 | seek 4; A1CC; open(tok,0); open(tok,1) | cursor 4 kept; then 0 | A1C8 reset_position |
| 41–42 | open(invalid,1) | 0; active cursor still 6 | A1C8 failure preserves context |
| 43 | A19C invalid token | ≤ 0 (no byte count) | resolver status (sign not asserted) |
| 44 | A1CC; A194(tok) | 6 | A1CC only detaches |
| 45 | A1EC | 0 | A1EC closure |

Coverage gaps, deliberately left for later probes: capacity growth and partial
writes (A198), live-mirror updates, clipboard token `0x00CB`, and NEO hardware.

Build with `make` (`make SAVE_TEMPS=1` keeps intermediates). Not claimed as
passed until a run is recorded.
