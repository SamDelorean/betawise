# SYS_A450 — blocked source-level contract

`SYS_A450` (A-line index 276) is mechanically reconstructed for NEO 2013 but intentionally remains a neutral symbol.

Confirmed mechanics:

- NEO 2013 only among the three canonical ROM generations examined;
- five 32-bit incoming stack slots;
- a separate exact 6-byte output aggregate through `A1`;
- first-stage structural selection via `SYS_A448` with capacity 2;
- second-stage bounded entity/reference decoding via `SYS_A444` of descriptor 0;
- the fourth stack slot contributes only a signed 16-bit effective bound;
- three official Accelerated Reader callers use selector literals `/q`, `/q1`, `/q2` with bound 1024;
- the recovered callers do not consume `D0` as a return value.

Still blocked:

- vendor/source-level function name;
- exact source-level type and calling-convention meaning of the 6-byte `A1` aggregate;
- a stable scalar `D0` return contract;
- vendor names for aggregate states, descriptor type, or selector grammar.

For those reasons no speculative prototype is added to the SDK. The public reconstruction should continue to refer to this service as `SYS_A450` until independent evidence resolves those source-level details.
