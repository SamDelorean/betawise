# A320 — confirmed null A-line vector

Status: **mechanically closed (A) / confirmed null vector / no callable contract**.

A320 is A-line index 200. The canonical AS3000 2005, NEO 2005, and NEO 2013 syscall tables all contain `0x00000000` for this entry. There is therefore no addressable handler and no callable ABI to publish.

This entry belongs to the cross-ROM null run A30C–A328, but index 200 was checked individually. A308 and A32C are non-null boundary entries.

Official SmartApplet corpus: all 30 applets with the relevant A-line table omit A320 inside the direct A308→A32C jump; the remaining 11 applets are structural negatives without that table. Executable A320 callers: 0/41.

The reconstructed A-line dispatcher does not guard a null table entry before the indirect jump. For a forced A320 opcode the demonstrated mechanical sequence is therefore a zero target followed by the indirect jump. No vendor intent such as "reserved" or "unimplemented" is assigned.

Static cross-check: **OVERALL PASS**. Dynamic regression was not executed.
