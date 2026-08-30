# A31C — confirmed null A-line vector

Status: **mechanically closed (A) / confirmed null vector / no callable contract**.

A31C is A-line index 199. The canonical AS3000 2005, NEO 2005, and NEO 2013 syscall tables all contain `0x00000000` for this entry. There is therefore no addressable handler to describe and no callable ABI to publish.

This index is one member of the cross-ROM null run A30C–A328. The entry was nevertheless checked individually rather than closed by inheritance from neighboring null entries. A308 and A32C are non-null boundary entries.

The official SmartApplet corpus is also structurally negative: all 30 applets that materialize the relevant A-line table jump directly from A308 to A32C and omit A31C; the remaining 11 applets do not contain such a table. Executable A31C callers: 0/41.

The reconstructed A-line dispatcher does not test the selected vector for null before its indirect jump. For a forced A31C opcode, the demonstrated mechanical sequence is therefore a zero target followed by the indirect jump. This document deliberately does not assign vendor intent such as "reserved" or "unimplemented" to the null entry.

Static cross-check: **OVERALL PASS**. Dynamic regression was not executed and is not required to establish the null table entry.
