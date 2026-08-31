# SYS_A470 / index 284 — ABI trace

Status: **mechanically reconstructed (A), source-level identity unknown**.

## Availability

| Platform | Slot disposition |
|---|---|
| AlphaSmart 3000 (2005) | raw slot value `0x25017501`; outside the demonstrated ROM/runtime window, so no callable contract is claimed |
| NEO (2005) | raw slot value `0x25017501`; outside the demonstrated ROM/runtime window, so no callable contract is claimed |
| NEO (2013) | slot resolves to runtime `0x0043F63C` |

## Confirmed mechanical ABI

The NEO 2013 handler takes no stack arguments and consists of two instructions:

```text
MOVE.W  $0000043E.L,D0
RTS
```

Therefore the demonstrated return channel is **`D0.W` / the low 16 bits of `D0`**. The upper 16 bits are not defined by this handler and must not be treated as part of a 32-bit return value.

A neutral C-level representation is:

```c
uint16_t SYS_A470(void);
```

The RAM word at `0x0000043E` remains semantically unnamed. No vendor function name or subsystem identity is asserted.

## Handler identity

For the canonical NEO 2013 image, A470 maps to file offset `0x02F63C`, runtime `0x0043F63C`. The handler is exactly 8 bytes with SHA-256 `fe894acb32cf64d3715bfc81758e3d27be2eb5749ef45b144e14b0738dbb9cef`.

A separate firmware path writes the same RAM word with `MOVE.W D0,$0000043E.L`; this establishes a distinct word-sized writer but does not identify the word's semantics.

## Official SmartApplet corpus

The complete 41-applet classification was covered for A470. Seven of the 16 EXTENDED applets expose an A470 veneer immediately after the late `A450,A454,A468,A46C` prefix:

- AcceleratedReader
- ControlPanel
- KeyWordsWireless
- MathFactsInAFlash
- MathFactsOffline
- Text2SpeechUpdate
- WirelessUpdate

The other nine EXTENDED applets do not expose A470 there. The 14 LEGACY applets terminate before this late range and 11 applets have no A-line table, yielding **7 positive / 34 negative structural results across 41/41**.

A prior assumption that the late prefix necessarily terminated at A46C was rejected by this A470 pass.

No executable caller to any of the seven A470 veneers was recovered with the validated indexed-PC, PC-relative, BSR.W, or absolute JSR forms. The NEO 2013 firmware also contains zero direct JSR.L, JMP.L, or BSR.W references to the handler.

Raw `A470` words outside the demonstrated veneers occur in resource-heavy regions of several applets. They are deliberately not promoted to callers without an executable boundary and a resolved call target.

## Adversarial regression

The first private regression run intentionally encoded the inherited terminal-tail assumption and failed **7 checks**, revealing the seven real A470 veneers. A later expanded harness also exposed additional raw `A470` words outside veneers; those were retained as raw-word inventory rather than misclassified as executable calls. The corrected final static regression passes **88/88** checks.

The final regression verifies canonical ROM hashes, vector resolution, exact handler bytes/hash, the word-width return property including preservation of `D0.upper16`, direct firmware-xref negatives, the word-sized writer to RAM `0x43E`, all 16 EXTENDED applet hashes, exact A470 veneer distribution, caller-search negatives, and conservative raw-word handling.

Dynamic regression has not been executed.

## Publication boundary

The public symbol remains neutral: `SYS_A470`. The word at RAM `0x43E`, its writer, and the higher-level purpose of this getter remain **DESCONOCIDO**. Private ROM bytes and extended workpapers are not published.
