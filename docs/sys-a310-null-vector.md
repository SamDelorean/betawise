# OS3K A310 — null vector closure

A310 is not exposed as a callable service by the canonical OS3K firmware corpus.

The A-line entry for index 196 is exactly zero in AS3000 2005, NEO 2005, and
NEO 2013. This was verified independently for A310 rather than inherited from
the neighboring null-vector result. There is therefore no handler address,
handler body, CFG, or callable ABI to publish.

The official SmartApplet corpus independently agrees: all 30 applets that
contain the relevant A-line table omit A310 as part of the gap between A308 and
A32C. The remaining 11 applets have no such table. No A310 caller exists in the
41/41 demonstrated A-line corpus.

The demonstrated A-line dispatcher does not test the selected vector for NULL
before its final indirect jump. A forced A310 opcode mechanically resolves the
vector to zero and jumps to address zero. This note deliberately does not assign
vendor semantics such as `reserved`, `unsupported`, `error`, or `crash` to that
mechanical fact.

Status: **mechanically closed / null vector confirmed / no callable contract**.
The original vendor purpose or label for the null slot remains unknown. Private
ROM material and reverse-engineering workpapers remain in Drive.
