# OS3K A314 — null vector closure

A314 is not exposed as a callable service by the canonical OS3K firmware corpus.

Its A-line entry (index 197) is exactly zero in AS3000 2005, NEO 2005, and NEO
2013. This index was checked independently rather than inferred from neighboring
null entries. No handler address, handler body, CFG, or callable ABI therefore
exists to publish.

All 30 official SmartApplets that contain the relevant A-line table omit A314 in
the demonstrated gap between A308 and A32C. The remaining 11 official applets
lack that table. The 41/41 official corpus therefore contains no A314 A-line
caller path.

The demonstrated A-line dispatcher does not guard a zero vector before its final
indirect jump. A forced A314 opcode mechanically resolves to address zero. No
vendor semantic label is assigned to that fact.

Status: **mechanically closed / null vector confirmed / no callable contract**.
The original vendor purpose or label remains unknown. ROM material and private
reverse-engineering workpapers remain outside this repository.
