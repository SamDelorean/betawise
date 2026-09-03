# SYS_A1D8 file-password reset — source-first closure

Status: mechanically and semantically closed from primary firmware evidence.

```c
uint32_t SYS_A1D8(void);
```

No reliable modern vendor symbol has been recovered, so the public name remains `SYS_A1D8`.

## Source-first correlation

Historical AlphaSmart password sources establish an independent data-model anchor: file passwords are NUL-terminated strings with a five-character limit and the factory file password is `"write"`. Earlier SDK/Ghidra work and official Control Panel callers identify A1D8 as the destructive password-reset path, but those sources are treated only as anchors; the contract below was revalidated from the three canonical ROM handlers.

## Firmware confirmation

A1D8 is vector index 118 in all three canonical generations. The public handler is exactly `0x154` bytes in AS3000 2005, NEO 2005 and NEO 2013, ending immediately before A1DC/index119.

The three handlers preserve the same control-flow structure after relocation differences. Primary evidence confirms:

- the caller ABI has no arguments; there is exactly one absolute internal JSR caller per ROM and it performs no A1D8 argument push;
- the handler initializes its result to zero, so cancel/failure paths return zero;
- the interactive password path passes its local input buffer directly to A24C/index147, the independently reconstructed master-password comparison service;
- a failed A24C comparison branches away from the destructive loop;
- the success loop starts a byte counter at zero and walks descriptors with stride `0x48`;
- each iteration targets descriptor offset `+0x18`, independently established by A1DC as the per-file password field;
- the copy helper called by the loop is mechanically a `strcpy` implementation (copy bytes until the terminating NUL, return destination);
- the copy source resolves to the literal `"write\0"` in all three canonical firmware generations;
- the loop increments one count per descriptor and the success return zero-extends that count into the function result.

Thus the observable operation is:

```text
if interactive master-password authorization is cancelled or fails:
    return 0

for every file descriptor in the current file group:
    strcpy(descriptor->password, "write")

return number_of_passwords_reset
```

The current-group qualification is supported by the existing File API reconstruction and the descriptor iteration/count machinery used by the handler. No evidence supports a device-wide reset across unrelated namespaces.

## Classification

**CONFIRMED:** zero-argument ABI; interactive authorization gate; direct A24C master-password comparison; failure/cancel return zero; descriptor stride `0x48`; password field `+0x18`; `strcpy` copy semantics; factory literal `"write"`; one reset per iterated descriptor; count return; equivalent semantics across AS3000 2005 / NEO 2005 / NEO 2013.

**INFERENCIA FUERTE:** continuity with the historical AlphaSmart factory-password model and description as a current-file-group/namespace reset operation.

**DESCONOCIDO:** original modern System 3 vendor function name and private UI/helper symbols used to build the confirmation/error dialogs.

## Refutation

The firmware refutes several weaker interpretations:

- A1D8 is not a simple global flag setter: it iterates descriptor records and mutates password storage.
- It is not an unauthenticated reset: destructive mutation is reached only after the master-password comparison succeeds.
- It does not return a generic success status: successful execution returns the number of passwords reset.
- It does not use a caller-supplied replacement password: the source is the firmware factory literal `"write"`.
- The historical `PasswordModule` names are not promoted as modern ABI symbols; they establish lineage, not identity.

## Regression

Static regression was executed against newly re-read canonical ROM bytes: **57/57 PASS**. Checks cover canonical SHA-256, vector boundaries, 0x154 handler length, zero-result initialization, cancel path, A24C call, password-field offset, strcpy helper identity, descriptor stride, loop count/return construction, unique no-argument caller, and resolution of the factory source to `"write\0"` in all three generations.

Dynamic/emulator regression remains **SPECIFIED / NOT EXECUTED** because A1D8 deliberately changes all file passwords in the selected group. Any dynamic test must use disposable state, verify both failed and successful authorization, confirm every descriptor becomes `"write"`, check the returned count, and restore or discard the test image afterward. Hardware probing is not appropriate for discovery.
