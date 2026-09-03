# A244 / AppletSendMessage — source-first closure

Status: **CERRADO A / SOURCE-FIRST**  
Date: 2026-09-03 10:20 America/Mexico_City

## Contract

```c
uint8_t AppletSendMessage(
    uint32_t index,
    Message_e message,
    uint32_t param,
    uint32_t *status);
```

Only the low byte of the return value is contractual: `1` after a dispatched callback and `0` when the target is rejected.

## Source-first correlation

Historical `os3k.pdf`/early SDK and BetaWise identify A244 as `AppletSendMessage`; the early header used an 8-bit index, while prior firmware work had already suggested a 32-bit index. The canonical firmware was re-extracted and checked independently before accepting either declaration.

## Firmware-confirmed behavior

The three canonical ROMs have the same A244 control-flow shape and an exact handler length of `0xAA` bytes.

* `index` is consumed as a full longword.
* **Valid dispatch targets are indices 1..31. `index == 0` is rejected.** This corrects the inherited note that index 0 uses a System route.
* `index >= 32`, an uninstalled table slot, or failure of the target-eligibility helper returns low byte `0` without entering the callback.
* Except for raw messages `0x1C` and `0x1D`, the delivered message is normalized to `(message & 0x00FFFFFF) | 0x01000000` (`MSG_MOD_SYNTHETIC`).
* The target AppletHeader entry point is read from offset `+0x84` and resolved relative to the applet header base before the indirect callback.
* The handler saves the current-applet runtime index, installs the target index, switches the applet-owned execution/A5 context through the private context helper, calls the applet entry point with `(message, param, status)`, restores that context, and then restores the previous current-applet index.
* No firmware-side NULL guard for `status` is visible before the callback; the pointer is forwarded to the applet entry point.

## Canonical primary evidence

ROM SHA-256 values match the project canonical set:

* AS3000 2005: `732dca63399d883071ce41cc07ad352fa19c506b7b8c307ace16d5c53e6f8708`
* NEO 2005: `5f550e48ad36892ee5f5a067e8c1b87f6bf504b6cb2c2dfc4098776fa8831a3d`
* NEO 2013: `32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0`

Handler SHA-256 (`0xAA` bytes):

* AS3000 2005: `59faa1ae05d3b9d572f7e46278ded9ccf74f77f9dff5336a45d265edb1895f1a`
* NEO 2005: `b95344412da5415f7eb5b09a0fa340ccc352efa7a37729a5b8f3b1c491a472dd`
* NEO 2013: `218af8b4ad7d028c180d34f8c47ee7e2fa7867c688e95b744aea8fc81b8cb515`

Direct absolute `JSR` xrefs to A244 in the complete canonical ROM images are `0 / 8 / 9` for AS3000 2005 / NEO 2005 / NEO 2013. The NEO caller families pass four stack arguments and provide independent ABI corroboration. The AS3000 negative result is preserved rather than replaced with inferred callers.

## Refutation pass

* “Index 0 dispatches to System” — **REFUTED by firmware 3/3**.
* 8-bit target index — **REFUTED**; the handler loads and scales a longword.
* Unbounded index — **REFUTED**; `index >= 32` is rejected.
* Message forwarded unchanged in all cases — **REFUTED**; only `0x1C` and `0x1D` bypass synthetic-message normalization.
* Callback without execution-context switch — **REFUTED**; private context switch/restore brackets the indirect call.

## Regression

Static source-first regression: **EXECUTED 72/72 PASS** (24 checks per ROM), covering canonical ROM hashes, handler boundary, argument widths, target validation, table lookup, eligibility gate, message normalization/exceptions, entry-point offset, context switch/restore, current-applet save/restore, epilogue, and direct-JSR xref counts.

Dynamic regression: **SPECIFIED / NOT EXECUTED**. Emulator/sandbox cases should cover indices `0`, `1`, `31`, `32`, an uninstalled slot, an eligibility-gated slot, raw messages `0x1C`/`0x1D`, a normal message requiring `MSG_MOD_SYNTHETIC`, status propagation, and restoration of current applet/A5 state after callback.

## Compatibility note

The public prototype already present on `sdk/abi-automation` has the firmware-correct 32-bit index. Any documentation saying that index 0 follows a System dispatch path must be corrected to state that A244 accepts installed/eligible applet targets only at indices 1..31.
