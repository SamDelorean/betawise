# OS3K A30C — null vector closure

A30C is not exposed as a callable service by the canonical OS3K firmware corpus.

Primary firmware evidence shows that A30C's A-line vector entry is exactly zero in
AS3000 2005, NEO 2005, and NEO 2013. The surrounding run A30C through A328 is
also zero in all three images, while A308 immediately before the run and A32C
immediately after it are non-zero. Therefore there is no handler address, handler
body, CFG, or callable ABI to publish for A30C.

The official SmartApplet corpus independently agrees with that firmware result.
All 30 applets that contain the relevant A-line stub table transition directly
from A308 to A32C; A30C through A328 are omitted from the table. The remaining
11 applets have no such table. Thus the 41/41 official corpus contains no A30C
caller path through the demonstrated A-line mechanism.

The A-line dispatcher itself does not test the resolved vector for NULL before
its final indirect jump. For a forced A30C opcode the mechanical destination is
address zero. This document deliberately does not label that behavior as a
vendor-defined error, crash, reserved slot, unsupported operation, or any other
semantic category that the evidence does not establish.

Status: **mechanically closed / null vector confirmed**. No callable prototype is
published. The original vendor purpose or label for the null slot remains
unknown. ROM bytes and private reverse-engineering workpapers are intentionally
not included here.
