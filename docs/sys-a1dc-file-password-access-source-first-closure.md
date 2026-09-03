# SYS_A1DC file-password access — source-first closure

Status: mechanically and semantically closed from primary firmware evidence; prior five-character boundary corrected.

```c
int32_t SYS_A1DC(uint16_t file_id, char *password, uint8_t read_back);
```

No reliable modern vendor symbol has been recovered, so the public name remains `SYS_A1DC`.

## Source-first correlation

Historical `PasswordModule` source supplies useful lineage but not the modern ABI: the early module defined a five-character file-password limit and setter-only interfaces. Existing Ghidra/SDK work and official callers identified A1DC as password access. Those references were treated as hypotheses and then checked directly against AS3000 2005, NEO 2005 and NEO 2013 firmware.

A1D8 independently anchors descriptor offset `+0x18` as the per-file password buffer and uses the same private string-copy helper.

## Firmware confirmation

A1DC is vector index 119 in all three canonical generations. Entry points are `0x004E2B26` (AS3000), `0x005E5482` (NEO 2005) and `0x0043B884` (NEO 2013). A1E0 begins exactly `0x7A` bytes later in each image, so the public handler length is `0x7A`.

The handler consumes three ABI slots:

- `file_id` as a word from the first 32-bit slot;
- `password` as a pointer from the second slot;
- `read_back` as the low byte of the third slot.

It resolves `file_id` through the common File API resolver. Resolver failure leaves the initialized result at zero.

### Write mode (`read_back == 0`)

The handler calls a private helper on `password`. Direct re-extraction of that helper in all three ROMs confirms standard `strlen` mechanics: advance byte-by-byte until NUL and return the character count.

The returned length is compared with immediate `6`, followed by 68k `BLS` (unsigned lower-or-same) into the copy path. Therefore:

```text
strlen(password) <= 6  -> accepted
strlen(password) > 6   -> return -8, no copy
```

This is a material correction to earlier project documentation that inherited the historical five-character limit and stated `length >= 6 -> -8`. The modern firmware does not implement that rule.

For an accepted write, the handler calls the private `strcpy` helper with caller `password` as source and `descriptor+0x18` as destination.

### Read mode (`read_back != 0`)

The same `strcpy` helper is called in the opposite direction: `descriptor+0x18` is the source and the caller's `password` buffer is the destination.

No safe NULL-pointer branch exists before the `strlen`/`strcpy` dereferences.

### Return

After a successful read or write, the handler zero-extends the canonical 16-bit token supplied by the resolver and returns it in `D0.L`. Resolver failure returns zero. The overlength write path returns raw status `-8`.

## Callers and xrefs

There is exactly one absolute direct JSR to A1DC in each canonical ROM. The three equivalent call sites construct `read_back=0`, pass a firmware string pointer, zero-extend a file identifier and invoke A1DC. A-line callers may exist and are not excluded by this negative absolute-JSR search.

The observed direct caller is consistent with write mode; read mode remains independently proven by the handler's explicit nonzero branch and reversed `strcpy` argument order.

## Cross-ROM comparison

The public `0x7A`-byte body is structurally equivalent in all three generations. Differences are relocation targets and a diagnostic metadata literal; the `strlen`, compare-6/BLS rule, read/write direction, `strcpy`, return construction and error behavior are unchanged.

## Classification

**CONFIRMED:** three-argument ABI; File API resolver; password field `+0x18`; `read_back==0` write and nonzero read; private helpers mechanically equivalent to `strlen` and `strcpy`; modern acceptance boundary `strlen <= 6`; `strlen > 6 -> -8`; canonical-token success return; resolver-failure zero; lack of NULL guard; cross-ROM equivalence.

**INFERENCIA FUERTE:** conceptual continuity with the historical AlphaSmart file-password subsystem.

**DESCONOCIDO:** original modern System 3 vendor function name and symbolic name of raw status `-8`.

## Refutation

- Refuted the inherited statement that modern passwords are limited to five characters: the actual handler accepts six-character strings plus NUL.
- Refuted `length >= 6 -> -8`: the branch is `BLS`, so exactly six is accepted and only values greater than six fail.
- Refuted setter-only semantics: the nonzero `read_back` branch reverses the copy direction and reads the stored password into caller storage.
- The historical `PasswordSetFilePassword` name is not promoted because its signature and one-way semantics are not ABI-identical to A1DC.

## Regression

Static regression was executed against the canonical ROMs: **96/96 PASS**. It covers ROM hashes, vector boundaries, exact handler size, argument loads, resolver preparation, read/write branch, direct `strlen` helper identity, compare-immediate 6, `BLS` opcode, `-8` construction, both `strcpy` argument orders, direct `strcpy` helper identity, canonical-token return, final result load, one direct caller per ROM and that caller's write-mode argument construction.

Dynamic regression is **SPECIFIED / NOT EXECUTED**. On disposable state it should test write lengths 0 through 6 as accepted, length 7 as `-8` without mutation, get-after-set, resolver failure, and read/write round trips. NULL pointers are outside the supported contract.
