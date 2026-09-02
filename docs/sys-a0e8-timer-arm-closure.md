# A0E8 / index 58 — timer arm service

Status: **mechanically closed (A)**. Public name remains `SYS_A0E8`; no vendor-era symbol has been recovered for this 2005-era interface.

## Source-first lineage

The early AlphaSmart `TimerModule` is a clear genealogical ancestor: it maintains a fixed timer array, allocates a free timer record, starts timers under a critical section, and dispatches client callbacks from the timer interrupt path. That older source uses three countdown records and a 10 ms software tick. The OS3K ROM implementation at A0E8 is not byte/API-equivalent to that early module: it has evolved into five records, absolute hardware deadlines, completion signaling, callback events, and generation-tagged tokens.

BetaWise historically labels syscall index 58 only as `SYS_A0E8 // set timer`. Independent `neo-re` work later described a five-slot timer record. Those were treated as anchors, not as primary proof.

## Primary ROM result

Canonical handlers:

| ROM | A0E8 entry | Handler size |
| --- | ---: | ---: |
| AS3000 2005 | `0x004CFA56` | `0x150` |
| NEO 2005 | `0x005D3B54` | `0x150` |
| NEO 2013 | `0x00424B6A` | `0x150` |

After relocation operands are neutralized, the three handlers are mechanically identical.

The raw ABI is best represented as:

```c
typedef void (*OS3KTimerCallback)(uint32_t event);

int32_t SYS_A0E8(
    uint16_t delay_ms,
    volatile uint8_t *completion_status,
    OS3KTimerCallback callback,
    uint16_t *token_out);
```

The four arguments occupy normal four-byte ABI slots. The handler consumes the low word of slot 1, the full pointers from slots 2–4, and returns status in `D0`.

### Confirmed mechanics

- `delay_ms > 10000` returns `-302`.
- Delays below 3 are clamped to 3.
- Exactly five timer records are scanned for a free slot.
- A record is 14 bytes: active byte at `+0`, 16-bit deadline at `+2`, 16-bit token at `+4`, completion pointer at `+6`, callback pointer at `+10`.
- The absolute deadline is formed in the timer-1 hardware time domain using the current accumulated timer state plus `TCN1` and the requested delay.
- The token is `(generation_byte << 8) | slot_index`; the generation byte increments after allocation.
- When the new deadline is earlier than the current scheduled compare, the handler makes the new record current and programs `TCMP1` from its deadline.
- If `token_out` is non-null, it is preinitialized to `0xFFFF` and replaced with the generated token on success.
- Success returns `0`; no available slot and the timer-family blocked state return `-301`.
- A0EC independently validates the low-byte slot and the full token against record `+4`, corroborating the token interpretation without closing A0EC itself.

The surrounding scheduler confirms the pointer roles: on ordinary expiry it writes byte `1` through the optional completion pointer and invokes the optional callback with a four-byte ABI event value whose low byte is `1`. The timer-family shutdown/cancel-all path uses `0xFF` for both channels. The exact source-level integer type originally used for the callback event has not been recovered; `uint32_t` above expresses the observed four-byte call slot rather than claiming a vendor typedef.

## Callers / xrefs

Direct absolute JSRs to the handler inside the canonical ROMs: AS3000 `7`, NEO 2005 `5`, NEO 2013 `13`. Concrete wrappers pass the four arguments in the order shown above; examples include a null completion pointer, a callback function pointer and a token-output pointer. Searches of stock AlphaWordPlus applet images find A0E8 only in their syscall opcode tables, not as direct inline A-line calls.

## Refutation attempts

The historical interpretation “set timer” is directionally correct but materially incomplete. A0E8 is not the old `TimerStartTimer(timer_number, ticks)` API: it performs allocation and arming together, returns a generation-tagged handle, supports two completion channels, and uses the millisecond hardware timer domain. The early source's three-record/10 ms countdown design therefore cannot be copied forward as the modern contract.

An initial cross-generation discrepancy observed during this audit was refuted: it came from indexing the dispatch table base as if it were already the index-58 entry. Recomputing every entry as `table_base + 4 * index` yields the entries above and relocation-only equivalence across all three ROMs.

## Evidence classification

**CONFIRMED:** four-slot raw ABI; delay bounds/clamp; five records; 14-byte record layout; deadline/token construction; completion and callback pointers; success and error returns; `TCN1`/`TCMP1` scheduling; token validation relationship with A0EC; 3/3 cross-ROM mechanical equivalence.

**STRONG INFERENCE:** the first argument's source-level unit/name is milliseconds. It is added directly in the same 1 ms hardware timer domain already established for `GetUptimeMilliseconds`, with no scaling.

**PROVISIONAL:** `OS3KTimerCallback` and its `uint32_t event` spelling. Firmware proves one four-byte event slot and low-byte values `1`/`0xFF`, but no vendor typedef has been recovered.

**UNKNOWN:** vendor-era public name for A0E8 and semantic names for event values `1` and `0xFF`.

## Regression

Static regression was **EXECUTED: 94/94 PASS** against the three canonical ROMs. It verifies hashes, dispatch entries, exact handler length, argument loads, bounds, five-slot scan, 14-byte stride, record fields, generation token, timer hardware accesses, return values, direct JSR counts, A0EC token cross-checks, scheduler signaling and relocation-normalized equivalence.

No ROM bytes, firmware image, Ghidra project or extended disassembly are published here.
