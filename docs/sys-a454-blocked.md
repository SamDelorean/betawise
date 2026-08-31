# SYS_A454 — blocked source-level contract

`SYS_A454` (A-line index 277) is mechanically reconstructed for NEO 2013 but remains a neutral symbol.

Confirmed mechanics:

- not callable from the AS3000 2005 or NEO 2005 canonical vectors;
- four 32-bit physical stack slots on NEO 2013;
- selector low byte constrained to 1..8 and converted to a zero-based record index;
- slot 2 stored as a full longword;
- only the low bytes of slots 3 and 4 affect the selected record;
- updates one of eight fixed-size records;
- three official SmartApplet callers plus one firmware caller recovered;
- all recovered callers discard `D0`;
- unlike A43C–A450, A454 does not use the `A1` six-byte aggregate-result channel.

Still blocked:

- vendor/source-level function name;
- semantic identity of the eight-record table and its fields;
- scalar return intent, width, and signedness.

For those reasons no callable header is added to the SDK. Public code should continue to refer to this service as `SYS_A454` only when discussing the mechanically reconstructed trap.
