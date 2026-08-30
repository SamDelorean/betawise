# A3C0 / index 240 — generational closure

Status: **mechanically closed** from primary firmware, firmware callers, the official SmartApplet corpus, and reproducible static regression. The vendor symbol and the semantic name of the backing state byte remain unknown, so the neutral `SYS_A3C0` name is retained.

## Platform availability

- AlphaSmart 3000 (2005): index-240 slot contains `0x89909192`, not a demonstrated code pointer.
- NEO (2005): the same slot contains `0x89909192`, not a demonstrated code pointer.
- NEO (2013): index 240 points to runtime `0x0043DAD2` (file offset `0x02DAD2`).

No meaning such as `reserved` or `unimplemented` is assigned to the older-generation marker.

## NEO 2013 contract

The handler reads no input arguments. It tests one mutable global byte and returns a normalized zero/non-zero result. Four direct firmware callers independently establish that only `D0.B` is contractual: one performs `CMPI.B #1,D0` and three perform `TST.B D0` immediately after A3C0.

```c
/* NEO 2013 only; vendor name and state meaning unknown. */
uint8_t SYS_A3C0(void);
```

The upper bits of `D0` are not contractual. The false path clears only `D0.B`, whereas the true path uses `MOVEQ #1,D0`.

## Handler and state

NEO 2013 handler:

- runtime `0x0043DAD2`
- file offset `0x02DAD2`
- exact length `0x10` bytes, ending immediately before A3C4
- SHA-256 `f3ebf233e2aba5b86449eac3049f4376acbf9902ba628177308aee75d1085bc7`

The backing global is byte-sized. Writers for at least zero and one are present, and neighboring firmware writes other values as well. Therefore the state is deliberately **not** published as a C `bool`, enum, status code, or named subsystem state. A3C0 itself only normalizes zero versus non-zero.

## Callers

NEO 2013 firmware contains four direct absolute JSRs to A3C0 and no direct absolute JMP or direct BSR.W. Every demonstrated firmware caller consumes only the byte result.

The official SmartApplet corpus was checked 41/41. Sixteen extended-table applets physically contain the A3C0 slot and were rematerialized/rehashed; corrected PC-index analysis finds no executable A3C0 invocation. Fourteen legacy table-bearing applets terminate before this extension and eleven applets are structural negatives. Official executable-caller count is therefore 0/41.

This distinguishes table **presence** from executable **use**.

## Adversarial conclusions

- Rejected: assigning a callable A3C0 contract to AS3000/NEO 2005 from the NEO 2013 implementation.
- Rejected: interpreting `0x89909192` as a demonstrated old-generation code pointer.
- Rejected: `uint16_t` or `uint32_t` return width; all four real firmware consumers use `D0.B`, while upper bits are not consistently defined.
- Rejected: implicit input parameters; the complete handler reads none.
- Rejected: naming the backing byte as boolean/status/state merely from normalized return behavior.
- Historical/repository searches for `A3C0` / `SYS_A3C0` did not recover an independent vendor symbol or prototype.

## Regression and private evidence

Static regression revalidates the three canonical ROM hashes, the generational slot values, exact NEO 2013 handler bytes/hash, the four firmware call sites and byte-sized consumers, representative backing-state writers, and the sixteen extended official applets with zero executable A3C0 calls. Result: **OVERALL PASS**.

Dynamic/emulator regression is specified but has not been executed. Full ROM bytes, detailed caller contexts, workpapers, and private reverse-engineering evidence remain in Drive.
