# A324 — confirmed null A-line vector

Status: **mechanically closed (A) / confirmed null vector / no callable contract**.

A324 is A-line index 201. The canonical AS3000 2005, NEO 2005, and NEO 2013 syscall tables all contain `0x00000000` for this entry. There is therefore no addressable handler and no callable ABI to publish.

Index 201 was checked individually even though it belongs to the cross-ROM null run A30C–A328. A308 and A32C are non-null boundary entries.

Official SmartApplet corpus: 30 applets with the relevant A-line table omit A324 inside the direct A308→A32C jump; 11 applets are structural negatives without that table. Executable A324 callers: 0/41.

The reconstructed A-line dispatcher does not guard a null table entry before the indirect jump. For a forced A324 opcode the demonstrated mechanical sequence is a zero target followed by the indirect jump. No vendor intent such as "reserved" or "unimplemented" is assigned.

Static cross-check: **OVERALL PASS**. Dynamic regression was not executed.
