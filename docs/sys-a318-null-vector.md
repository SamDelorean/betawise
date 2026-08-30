# OS3K A318 — null A-line vector

Status: **mechanically closed / confirmed null vector / no callable contract**.

A318 is A-line table index 198. In the canonical AS3000 2005, NEO 2005, and NEO 2013 firmware images, the A318 table entry is `0x00000000`. Therefore there is no addressable A318 handler to assign a C prototype to.

This result is intentionally neutral: it does **not** label A318 as reserved, unimplemented, unsupported, or an error service, because those vendor semantics are not demonstrated by the available primary evidence.

The adjacent run A30C..A328 (indices 195..202) is also null in all three canonical ROMs, while A308 (194) and A32C (203) are non-null. A318 was nevertheless checked as its own index rather than closed merely by inheritance from that run.

The reconstructed A-line dispatcher performs no null check before the indirect jump. That mechanical observation is documented only as dispatcher behavior and is not promoted to a vendor-level contract for A318.

No callable declaration is published for `SYS_A318`.
