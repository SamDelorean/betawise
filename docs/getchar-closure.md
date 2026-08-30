# A330 / `_OS3K_getchar` — ABI closure

Status: **mechanically closed (A) / published raw contract**.

## Raw OS3K ABI

```c
int32_t _OS3K_getchar(void);
```

BetaWise maps A-line index 204 (A330) to the raw stub `_OS3K_getchar`. Its public `int getchar(void)` implementation in `os3k.c` is a separate wrapper/reimplementation and must not be conflated with the trap entry. The wrapper is nevertheless useful independent corroboration because it reproduces the same high-level keyboard-wait and key-to-character flow.

Independent reconstruction of the three canonical firmware generations confirms the `getchar` identity of the raw service. A330 consumes no caller arguments. Its complete handler is 0x7C bytes in AS3000 2005, NEO 2005, and NEO 2013 and is mechanically equivalent after relocations.

The handler polls keyboard state, obtains a key, translates it to a character, and continues waiting while translation yields zero. Raw Backspace key code `0x03` is explicitly converted to character value `0x08`; the Applets key (`0x29`) takes a system-handling path and then returns to the wait loop. Other non-translatable keys are passed through an internal special-key path and likewise resume waiting.

Immediately before the sole `RTS`, the selected byte is zero-extended to the full D0 longword. Because zero causes another iteration rather than return, the observed mechanical result domain is **1..255**. No `EOF`/negative return path is present in these firmware bodies; this closure does not invent one. `int32_t` records the full 32-bit ABI result and remains compatible with the target's C `int` calling convention.

## Correlated internal calls

The handler reaches the already-correlated keyboard services `ScanKeyboard`, `IsKeyReady`, `GetKey(1)`, and `TranslateKeyToChar`. Two additional internal helpers participate in special-key handling but are not required to establish the external ABI.

The firmware also references the same current-applet runtime state already correlated through A244/A278 while processing the Applets-key path. That state is internal and is not part of A330's caller ABI.

## Caller survey

The official SmartApplet corpus was exhausted: 30 applets with the relevant A-line table plus 11 structural negatives without it. Exactly three executable A330 callers were found, all in Accelerated Reader. One propagates D0 directly to its own return, one tests D0 with `TST.L`, and one later overwrites it. No caller-side cleanup occurs, matching zero arguments.

This caller evidence independently rejects a `void` contract and confirms that the full longword result is observable at the ABI boundary.

## Generational comparison

AS3000 2005, NEO 2005, and NEO 2013 have the same control flow and result behavior after relocation of the current-applet global and six absolute helper targets. No contractual generation difference was found. The common relocation-normalized handler SHA-256 is `9f1de8eb71942cd9a9911c538d5226947c77d9ed534e068408ea7b6bc23a70ad`.

## Adversarial conclusions

- `void` is rejected by both the deliberate D0 result construction and official callers.
- A byte-only physical return is rejected: the producer defines D0.L and an official caller performs `TST.L`; the *value domain* is byte-sized, but the ABI result register is a full longword.
- The raw trap `_OS3K_getchar` and BetaWise's public `getchar()` wrapper are distinct implementation layers; they are not documented as the same symbol.
- The `getchar` functional identity is retained because the historical raw-stub name, reconstructed firmware behavior, neighboring C-library block, official callers, and independent wrapper behavior all converge.
- Standard-C `EOF == -1` behavior is **not** asserted for these ROMs because no negative path is present in the reconstructed handler.

Private workpapers retain ROM hashes, exact offsets and handler hashes, the 41/41 caller scan, correlated helper details, and static regression.

Static regression: **OVERALL PASS**. Dynamic regression: **not executed**.
