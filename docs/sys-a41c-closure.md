# SYS_A41C / index 263 — closure

Forward-sweep disposition for the OS3K A-line ABI.

## Contract

```c
void *SYS_A41C(uint8_t selector);
```

The name and types are intentionally neutral. No vendor symbol, selector enum, or concrete public structure type has been recovered independently.

## Primary evidence

- AS3000 (2005): table value `0x5A06004E`, non-pointer/unresolved; no callable contract assigned.
- NEO (2005): table value `0x7F08005E`, non-pointer/unresolved; no callable contract assigned.
- NEO (2013): runtime `0x00433B86`, file offset `0x023B86`.
- Handler: `0x92` bytes, 52 decoded instructions, one RTS, SHA-256 `ab0942b2bd308ec22364abbd82bd419f0c5b6bebe884cbb4702e70828d25f255`.
- Physical ABI: one 32-bit stack slot; only the low byte is semantically consumed.
- An internal selector helper also consumes only the low byte and accepts the raw range `5..21`; no signed selector semantics are required by the demonstrated mechanics.
- A private allocator-like helper searches 20 firmware-owned slots of stride `0x5E0` and returns a free slot or NULL.
- On success A41C initializes the returned object, selects a descriptor set from the byte selector, builds up to five records of stride `0x12C` from descriptors of stride `0x0C`, stores the constructed count, and explicitly returns the destination pointer in `D0.L`.
- Failure of either initial helper returns NULL.
- Four direct NEO 2013 firmware JSR callers were found. They pass concrete selectors `7`, `8`, and `10`, then consume `D0.L` as a pointer by storing it, moving it to an address register, dereferencing it, or forwarding it as a pointer.
- Direct JMP/BSR references are absent.
- Official SmartApplet exposure is structurally absent from the validated late-table corpus.
- Static regression: `42/42 PASS / OVERALL PASS`.
- Dynamic/emulator-first regression: not executed.

The mechanical pointer-return contract is therefore confirmed even though the semantic name of the constructed object remains unknown.

Private ROM bytes, disassembly and detailed workpapers remain outside the repository.
