# A044 / index17 — null A-line vector

Status: **CONFIRMED** for the canonical AS3000 2005, NEO 2005, and NEO 2013 firmware images.

Source-first correlation did not recover a vendor name, wrapper, or prototype for A044. Direct verification of the A-line dispatch tables in all three canonical ROMs shows that index 17 (`A044`) contains `0x00000000`.

The adjacent audited entries preserve table alignment: A03C/index15 is non-null, A040/index16 is null, A044/index17 is null, and A048/index18 is non-null in all three generations.

Accordingly, there is no handler body, callable ABI, argument list, return contract, or function-specific side effect to document for A044 in this corpus. The historical/vendor purpose of the slot remains **UNKNOWN**. This document intentionally does not define a `SYS_A044()` prototype.

Private firmware bytes, hashes, offsets, and regression workpapers remain in the project Drive.
