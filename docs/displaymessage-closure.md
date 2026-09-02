# A090 / index 36 — `DisplayMessage`

Status: source-first closure, firmware revalidated on the three canonical OS3K ROM generations.

## Public contract

```c
void DisplayMessage(const char *str);
```

The historical SDK name and prototype are consistent with the firmware implementation. A090 takes one string pointer; it does not take a StringID or timeout.

The effective operation is:

```c
ClearScreen();
PutString(str);
SetCursor(4, 1, SHOW);
PutString(resource_lookup(0x003A));
```

The fixed resource resolves in the NEO generations to the prompt `(Press a key to continue)` with leading padding spaces.

`DisplayMessage` itself does **not** wait for a key. No `WaitForKey`/A088 call is present in the handler. Callers that require blocking input must perform that operation separately.

## ABI and return

A090 consumes exactly one 32-bit external stack slot, used as `const char *str`. Official KeyWords callers clean four bytes after the call. The SDK declares `void`, and examined callers do not consume a return value. Any incidental register value left by the final `PutString` call is not part of the API contract.

A090 has no independent NULL guard visible before forwarding `str` to `PutString`; no NULL-tolerance guarantee is therefore asserted.

## Cross-generation behavior

The handlers are 0x40 bytes at the independently recovered A090/index-36 vectors:

- AlphaSmart 3000 2005: `0x004CC43C`
- NEO 2005: `0x005CBF94`
- NEO 2013: `0x0041C9B4`

After normalizing only the five relocated absolute call operands, all three handlers are byte-identical. Normalized SHA-256:

`7b0d29bd6c56652da4afa34874d68046a2c52b0f47b9f15ed549245d949ad841`

The internal resource lookup helper also preserves the same operation across generations: a word index is zero-extended, scaled by four, and used to select a pointer from a generation-specific table. AlphaSmart 3000 packages the resource data differently from NEO, but that storage/segmentation difference does not alter the public A090 ABI or call sequence.

## Caller evidence

Official KeyWords applets provide positive structural callers:

- KeyWords 2005: 5 calls
- KeyWords NEO 2012: 7 calls

Examined contexts pass one pointer and clean one 32-bit argument after A090. AlphaWordPlus 2005 and NEO 2012 provide reproducible negative results for structural A090 callers.

## Refutation and regression

The earlier `(StringID, timeout)` interpretation is refuted by the firmware and callers. The hypothesis that A090 is itself modal/blocking is also refuted: its five calls are `ClearScreen`, `PutString`, `SetCursor`, the fixed resource lookup, and `PutString`, followed by return.

A private static contractual regression covering per-ROM handler invariants, the reconstructed call sequence, argument count, fixed cursor/resource constants, return semantics, caller cleanup, absence of embedded A088, and cross-generation resource behavior completed **31/31 PASS**. Dynamic 68k probing was not executed and is not claimed.

## Confidence

- Identity / SDK mapping: **CONFIRMED**
- One-argument `const char *` ABI: **CONFIRMED**
- `void` public return contract: **CONFIRMED**
- Rendering/cursor sequence: **CONFIRMED**
- Fixed resource index `0x3A`: **CONFIRMED**
- Absence of embedded key wait: **CONFIRMED**
- NEO resource text: **CONFIRMED**
- Exact AlphaSmart 3000 physical resource-data mapping: **not promoted to public contract**
- NULL behavior beyond inherited `PutString` preconditions: **not guaranteed**
