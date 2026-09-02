# A0EC / index 59 — timer cancellation

Status: **mechanically closed (A)**. Public name remains `SYS_A0EC`; no vendor-era modern symbol has been recovered.

## Source-first correlation

BetaWise labels index 59 only as `SYS_A0EC // clear timer`. The early AlphaSmart `TimerModule` provides genealogical context through `TimerStopTimer` and `TimerDestroyTimer`: both operate under a critical section and disable/deallocate a timer by changing record state. That older API uses numbered timers, however, and is not the OS3K A0EC ABI.

Independent `neo-re` work describes A0EC as validating the slot encoded in the low byte of the token returned by A0E8 and then clearing that record. This was treated as a secondary anchor and rechecked against all canonical ROMs.

## Primary ROM result

| ROM | A0EC entry | Handler size |
| --- | ---: | ---: |
| AS3000 2005 | `0x004CFBA6` | `0x7E` |
| NEO 2005 | `0x005D3CA4` | `0x7E` |
| NEO 2013 | `0x00424CBA` | `0x7E` |

After relocation operands are neutralized, the three handlers are mechanically identical.

Best raw ABI representation:

```c
void SYS_A0EC(uint16_t token);
```

The token occupies the low word of one normal four-byte ABI slot. A stable return value does not exist: rejection paths leave `D0` without a defined result and the valid path uses it as part of critical-section bookkeeping. Direct callers do not consume a return value.

### Confirmed mechanics

- The low byte of `token` is interpreted as a slot index.
- Slot values `>= 5` are silently rejected.
- If the timer-family blocked-state byte is set, the call is silently rejected.
- The selected record address is `timer_array + slot * 14`.
- The full 16-bit token must equal record `+4`; this prevents a stale token from cancelling a later occupant of the same slot.
- Record mutation occurs inside the same critical-section helpers used by A0E8.
- If the matching record is inactive, no record state is changed.
- If active, its active byte is cleared.
- If the record being cancelled is also the scheduler's current record, the current-record pointer is redirected to the timer-family scheduler sentinel; that sentinel is marked active and its completion/callback pointers are cleared before the target record is deactivated.
- Cancellation itself does **not** signal the record's completion pointer and does **not** invoke its callback.

A0E8 independently proves the other half of the contract: allocation writes `(generation << 8) | slot` to record `+4` and returns that token to the caller.

## Callers / xrefs

Direct absolute JSRs to A0EC in canonical firmware: AS3000 `6`, NEO 2005 `6`, NEO 2013 `18`. Representative callers pass a single four-byte ABI slot whose low word is the previously stored timer token, and do not branch on or otherwise consume `D0` as a result.

Stock AlphaWordPlus applet images contain A0E8/A0EC only once each as adjacent entries in their syscall opcode tables; no additional inline A-line call site was found.

## Refutation

The historical label “clear timer” is directionally correct but underspecified. A0EC does not accept the old `TimerNumber` from the early three-slot `TimerModule`; it accepts the generation-tagged token produced by A0E8 and validates both slot and generation before mutation. It is therefore a token-safe cancellation/release operation in the modern five-record timer service.

The hypothesis that cancellation might report completion or run the callback is refuted by the handler: neither record `+6` nor `+10` is dereferenced on the cancellation path. Those channels are used by the expiry and timer-family shutdown paths instead.

## Evidence classification

**CONFIRMED:** one low-word ABI argument; token low-byte slot; full-token validation; five slots; 14-byte record stride; critical-section mutation; active-record clearing; current-record scheduler redirection; no completion/callback delivery during cancellation; no stable return value; 3/3 relocation-normalized equivalence.

**STRONG INFERENCE:** semantic operation is timer cancellation/release rather than merely clearing a numerical countdown.

**PROVISIONAL:** none required for the raw ABI.

**UNKNOWN:** vendor-era modern function name and vendor typedef for the timer token.

## Regression

Static regression was **EXECUTED: 91/91 PASS** against the three canonical ROMs. It verifies canonical hashes, dispatch entries, exact handler length, token extraction and validation, slot bounds, record stride/base, critical-section helpers, current-record/sentinel handling, record deactivation, direct JSR counts, A0E8 token linkage, caller non-use of a return value, AlphaWordPlus negative inline-call search, and relocation-normalized 3/3 equivalence.

No ROM bytes, firmware image, Ghidra project or extended disassembly are published here.
