# A328 — confirmed null A-line vector

Status: **mechanically closed (A) / confirmed null vector / no callable contract**.

A328 is A-line index 202. The canonical AS3000 2005, NEO 2005, and NEO 2013 syscall tables all contain `0x00000000` for this entry. It is the final member of the cross-ROM null run A30C–A328; A32C / index 203 is non-null in all three ROMs.

Index 202 was checked individually rather than closed by inheritance from adjacent null entries. There is no addressable A328 handler and therefore no callable ABI to publish.

Official SmartApplet corpus: all 30 applets with the relevant A-line table omit A328 within the direct A308→A32C jump; the remaining 11 applets are structural negatives without that table. Executable A328 callers: 0/41.

The reconstructed A-line dispatcher does not guard a null vector before its indirect jump. For a forced A328 opcode the demonstrated mechanical sequence is a zero target followed by the indirect jump. No vendor intent such as "reserved" or "unimplemented" is assigned.

Static cross-check: **OVERALL PASS**. Dynamic regression was not executed.
