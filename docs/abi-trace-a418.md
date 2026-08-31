# ABI trace — A418 / index 262

- Disposition: `MECÁNICA_RECONSTRUIDA A / BLOQUEADO` only in contractual return intent.
- AS3000 2005: non-callable/non-pointer table value `0x5A06004E`.
- NEO 2005: non-callable/non-pointer table value `0x7F08005E`.
- NEO 2013: runtime `0x004342D4`, file offset `0x0242D4`.
- Handler: `0x3A` bytes, 19 instructions, one RTS; SHA-256 `69350dbfc4df1cee84a38cde7ae9fbba77912d061e994cf68d394cb0a6f8289b`.
- Physical ABI: one mutable pointer slot.
- Observed input layout: 16-bit count at `+0`, byte state at `+2`; other fields remain untyped.
- Mechanical effect: reset `count` private records at base `0xF43E`, stride `0x56`, through helper `0x004427FC`; then clear count and store state byte `5`. NULL input is a no-op.
- The helper's minimum demonstrated effect is to clear the observed record fields across its `0x56`-byte layout; no semantic record name is assigned.
- Direct firmware xrefs: one JSR at runtime `0x00442F6A` / file `0x032F6A`; zero direct JMP/BSR.
- The demonstrated caller supplies one pointer and does not logically consume D0 after the call.
- D0 is path-dependent/residual and does not establish a scalar return; `void` remains a strong but unconfirmed intent inference.
- Official SmartApplet exposure: structurally absent (`0/41`) because the validated tables end before A418.
- Independent symbol/prototype: not recovered.
- Static regression: `26/26 PASS / OVERALL PASS`; an earlier 25/26 run was invalidated only by a mistyped expected SHA literal and is retained in the private regression record.
- Dynamic/emulator-first: not executed.
- Public callable header: intentionally not published.

Private ROM extraction, disassembly, helper details, caller traces and regression workpapers remain outside the repository.
