# A240 / AppletGetName — source-first closure

Status: **CLOSED A / SOURCE-FIRST**

A240/index144 was re-audited source-first against the canonical AS3000 2005, NEO 2005, and NEO 2013 firmware images. Historical SDK declarations and prior BetaWise reconstruction were treated as correlation sources only.

## ABI

```c
uint8_t AppletGetName(uint32_t index, char *name_out);
```

Firmware consumes `index` as a full 32-bit stack argument. Only the low return byte is contractual: success is 1 and failure is 0. The older SDK's narrower index declaration is therefore not the faithful raw ABI.

## Confirmed validation

A240 rejects the request when any of the following is true:

- `index == 0`;
- `index >= 32`;
- `AppletHeader_t *table[index] == NULL`.

The handler does **not** check `name_out` for NULL. A valid index with an invalid output pointer will therefore reach stores through that pointer.

## Confirmed copy semantics

For a valid installed index:

1. source is the inline applet-name field at `AppletHeader_t + 0x18`;
2. bytes are copied until the first source NUL or until the source cursor reaches `header + 0x3C`;
3. `0x3C - 0x18 == 36`, so at most 36 name bytes are copied;
4. the implementation also contains a destination guard at `name_out + 0x28` (40 bytes), but the 36-byte source-field bound dominates normal execution;
5. the current destination byte is then explicitly set to NUL;
6. success returns `1` via `MOVEQ #1,D0`.

A buffer of at least 37 bytes therefore covers the maximum 36-byte field plus terminator.

Failure executes `CLR.B D0`. Consequently the low byte is reliably zero but the upper 24 bits are not universally normalized (notably the immediate `index == 0` rejection can preserve prior upper bits). Consumers must treat the function as an 8-bit return contract.

## Cross-ROM primary validation

A240 entry / A244 boundary:

- AS3000 2005: `0x004E0E96` -> `0x004E0F02`
- NEO 2005: `0x005E37F2` -> `0x005E385E`
- NEO 2013: `0x00439BF4` -> `0x00439C60`

Exact handler length: `0x6C` bytes in all three ROMs.

Applet-header pointer table bases:

- AS3000: `0x0D82`
- NEO 2005: `0x0DDA`
- NEO 2013: `0x0E0A`

Handler SHA-256:

- AS3000: `e5d5966392a26466d7e8c370966b15beeb05f3b72e47ae3cdc2069d0c6b8405a`
- NEO 2005: `a8d8ed2941fcc963bad895fd5e28ac381b10bf4b435f698c2d2d9046ea44ede3`
- NEO 2013: `04729d45ed2de66b4cb985cffee5669ce711debb57015e288dc77c408710a363`

The implementations are instruction-for-instruction equivalent except for relocated table bases.

## Structural correlation

A238 uses the same runtime `AppletHeader_t *` table and the same inline name start at `+0x18`; A240 adds the fixed `+0x3C` field-end guard, confirming the 36-byte header name field directly from firmware.

## Callers / xrefs

A renewed full-ROM search for direct absolute `JSR` calls to A240 found `0/0/0`. This is retained as negative evidence; no inferred callers are substituted.

## Refutation

Firmware refutes the interpretations that A240 accepts slot 0, uses an 8-bit raw index, returns a normalized 32-bit Boolean, copies an unbounded C string, or validates a NULL destination pointer.

## Regression

Static source-first regression **EXECUTED: 60/60 PASS** (20 checks per ROM): canonical ROM identity, exact handler bounds, full-width index load, output-pointer load/cursor, index-zero rejection, index>=32 rejection, table base, NULL-slot rejection, `+0x18` source construction, byte-copy loop, source-NUL stop, `+0x3C` source-field bound, `name_out+0x28` destination guard, explicit terminator, success return, low-byte failure return, frame restore/RTS, and direct-JSR negative search.

Dynamic regression remains **SPECIFIED / NOT EXECUTED**. Safe future tests: valid installed index with short name; maximum 36-byte field; invalid indices 0, 32, and larger; uninstalled in-range index; verify terminator placement. NULL output should be tested only in an emulator/sandbox.

## Confidence

- **CONFIRMED:** `uint8_t AppletGetName(uint32_t index, char *name_out)`; valid installed range 1..31; table presence requirement; source `header+0x18`; max 36 copied bytes; explicit NUL; low-byte 1/0 return; no NULL-output check; 0/0/0 direct callers; handler 0x6C 3/3.
