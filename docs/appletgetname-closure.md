# System 3 / OS3K AppletGetName — A240 SDK consolidation

This note is the developer-facing consolidation record for A240, exposed by BetaWise as:

```c
uint8_t AppletGetName(uint32_t index, char *name_out);
```

## 0. Consolidation status

A240 reached this SDK-consolidation pass after the reverse-engineering process had already closed its mechanical contract with confidence **A**. This pass does not repeat ROM/disassembly work. It audits the delivered contract against the current BetaWise declaration, A-line stub and SmartApplet runtime reference, then records the contract application code should rely on.

Current state:

- mechanical contract: **A**, received from the reverse-engineering process;
- public identity: `AppletGetName`, name continuity **strong**;
- BetaWise declaration/stub: **implemented and audited**;
- prior `int` return declaration: **superseded** because failure only defines the low return byte;
- runtime index argument: full **32-bit** slot;
- additional wrapper: **not required**;
- dynamic emulator/hardware regression: **pending**.

If later primary evidence changes the upstream mechanical contract, this SDK contract must be reconciled and revised rather than silently preserving an obsolete conclusion.

## 1. Public contract

```c
uint8_t AppletGetName(uint32_t index, char *name_out);
```

`index` is a System 3 runtime SmartApplet table index. A240 validates the target before writing to the caller buffer:

- index `0` is rejected;
- index `>= 32` is rejected;
- an uninstalled/NULL runtime table entry is rejected.

For a valid installed index, the service copies the applet name field beginning at `AppletHeader_t + 0x18` and NUL-terminates the caller buffer.

The source field is exactly 36 bytes wide (`+0x18..+0x3B`). Therefore the largest valid output is 36 name bytes plus one NUL terminator. A caller that must safely handle a completely filled, non-NUL-terminated header name must provide at least **37 writable bytes**.

The implementation also carries a 40-byte destination-side guard, but the 36-byte header-field boundary is reached first for a normal `AppletHeader_t`, so 37 bytes is the practical minimum buffer size for the complete public contract.

`name_out` must point to writable memory for a valid installed index. The handler does not provide a useful NULL-output success path.

## 2. Return contract

Success executes a full `MOVEQ #1,D0`.

Failure is different: the handler clears only the **low byte** of D0. Upper D0 bits are not normalized and therefore are not contractual.

For that reason the portable SDK return type is:

```c
uint8_t
```

with the following low-byte meaning:

- `1` — name copied and terminated successfully;
- `0` — invalid index or uninstalled runtime slot.

Earlier BetaWise material used `int`. That declaration could expose stale upper D0 bits on a failed call and therefore did not describe the machine contract safely. The current `uint8_t` declaration is the canonical SDK form.

Application code must not depend on any upper return bits.

## 3. BetaWise integration

The current BetaWise branch already matches the reconstructed ABI:

```c
uint8_t AppletGetName(uint32_t index, char *name_out);
```

`os3k/syscall.c` maps A-line index 144 directly to `AppletGetName`, corresponding to trap A240.

No wrapper layer is required. The public SDK symbol is the direct System 3 resident service.

No behavioral code change is required by this consolidation pass because the corrected return width and 32-bit index declaration had already been incorporated into the working branch before consolidation.

## 4. Mechanical evidence received from the reverse-engineering process

The upstream closure compared the A240 handler in all three canonical firmware generations used by this project:

| ROM | A240 runtime address | Handler length |
| --- | ---: | ---: |
| AlphaSmart 3000, Nov. 2005 | `0x4E0E96` | `0x6C` |
| NEO, Nov. 2005 | `0x5E37F2` | `0x6C` |
| NEO/System 3.15, Jul. 2013 | `0x439BF4` | `0x6C` |

The implementations are structurally equivalent. Differences are relocation operands for the runtime applet table and helper/library addresses.

The delivered analysis established directly that:

1. the first stack slot is consumed as a 32-bit runtime index;
2. index 0, indices >=32 and NULL runtime entries are rejected;
3. the source begins at header offset `+0x18`;
4. copying cannot pass the 36-byte header-name field boundary at `+0x3C`;
5. the caller output is NUL-terminated on success;
6. success produces full D0=1;
7. failure clears only the low byte of D0.

This consolidation pass treats those machine-level results as delivered evidence and does not reopen the disassembly.

## 5. Runtime / C-standard relationship

`AppletGetName` is not an ISO C or hosted-libc routine. It is an OS3K SmartApplet-runtime service exposed through BetaWise's static `libos3k.a` A-line syscall layer.

The closest ordinary C operation would be bounded string copying from a known structure, but substituting `strcpy`/`strncpy` would lose the actual System 3 semantics: runtime-slot validation, installed-entry lookup, the fixed 36-byte `AppletHeader_t.name` source field, the explicit NUL termination and the byte-only return contract.

## 6. Safety and regression specification

A240 is suitable for a non-destructive emulator or hardware regression when the caller supplies a valid writable buffer.

Minimum regression matrix:

1. choose a known installed nonzero runtime index; call A240 with a >=37-byte buffer; verify low return byte `1` and that the copied string matches the header name;
2. verify the destination is NUL-terminated even when the source header name fills all 36 bytes;
3. index `0` -> low return byte `0`;
4. index `32` -> low return byte `0`;
5. an uninstalled slot in `1..31` -> low return byte `0`;
6. do not treat upper D0 bits on any failure as meaningful;
7. verify the operation does not change applet selection/current context or runtime-table contents.

This matrix is a **regression specification**, not evidence that emulator or hardware execution has already occurred.

## 7. Final SDK state

A240 is therefore classified as:

**RECEIVED FROM DEEP REVERSE ENGINEERING — MECHANICAL A → SDK CONSOLIDATED → IMPLEMENTATION/STUB AUDITED → PUBLISHED → REGRESSION PENDING**

Canonical SDK prototype:

```c
uint8_t AppletGetName(uint32_t index, char *name_out);
```

Primary deeper evidence remains in [`applet-runtime-api-closure.md`](applet-runtime-api-closure.md). This note is the developer-facing consolidation layer and does not replace the upstream research record.
