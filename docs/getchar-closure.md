# A330 / `getchar` — ABI closure

Status: **mechanically closed (A) / published contract**.

## Public ABI

```c
int getchar(void);
```

The historical BetaWise SDK already places `int getchar(void)` at A-line index 204 (A330). Independent reconstruction of the three canonical firmware generations confirms that assignment.

A330 consumes no caller arguments. Its complete handler is 0x7C bytes in AS3000 2005, NEO 2005, and NEO 2013 and is mechanically equivalent after relocations. It polls the keyboard path, obtains a key, translates it to a character, and continues waiting until a nonzero character is available. Raw Backspace key code `0x03` is explicitly converted to character value `0x08`; the Applets key takes a system-handling path and then returns to the wait loop.

Immediately before the sole `RTS`, the handler zero-extends the result byte to the full D0 longword. The observed mechanical result domain is therefore 0..255. No `EOF`/negative return path is present in these firmware bodies; this closure does not invent one.

## Correlated internal calls

The handler reaches the already-correlated keyboard services `ScanKeyboard`, `IsKeyReady`, `GetKey(1)`, and `TranslateKeyToChar`. Two additional internal helpers participate in special-key handling but are not required to establish the external ABI.

## Caller survey

The official SmartApplet corpus was exhausted: 30 applets with the relevant A-line table plus 11 structural negatives without it. Exactly three executable A330 callers were found, all in Accelerated Reader. One propagates D0 directly to its own return, one tests D0 with `TST.L`, and one later overwrites it. No caller-side cleanup occurs, matching zero arguments.

This caller evidence independently rejects a `void` contract and confirms that the full longword result is observable at the ABI boundary.

## Generational comparison

AS3000 2005, NEO 2005, and NEO 2013 have the same control flow and result behavior after relocation of the current-applet global and six absolute helper targets. No contractual generation difference was found.

## Adversarial conclusions

- `void` is rejected by both the deliberate D0 result construction and official callers.
- A byte-only ABI return is not used publicly: the producer defines D0.L and an official caller performs `TST.L`.
- The standard SDK name `getchar` is retained because it is independently present in the historical public prototype and matches the reconstructed behavior.
- Standard-C `EOF == -1` behavior is **not** asserted for these ROMs because no negative path is present in the reconstructed handler.

Private workpapers retain ROM hashes, exact offsets and handler hashes, the 41/41 caller scan, correlated helper details, and static regression.

Static regression: **OVERALL PASS**. Dynamic regression: **not executed**.
