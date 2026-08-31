# SYS_A474 / index 285 — ABI trace

Status: **no callable contract in the canonical corpus (mechanical confidence A)**.

## Vector disposition

| Platform | Raw index-285 value | Result |
|---|---:|---|
| AlphaSmart 3000 (2005) | `0x95088102` | outside the canonical runtime window; not a handler pointer |
| NEO (2005) | `0x95088102` | outside the canonical runtime window; not a handler pointer |
| NEO (2013) | `0x01020304` | outside the canonical runtime window; not a handler pointer |

Because none of the three canonical ROMs supplies a valid handler pointer, no handler bytes, argument list, return value, helper contract, or C prototype is claimed for A474.

## Official SmartApplet corpus

The 16 rehashed EXTENDED applets were checked specifically for A474 in the demonstrated late A-line-table region. None contains an A474 veneer there. The 14 LEGACY applets terminate before this late range and 11 applets have no A-line table, giving a **41/41 negative structural result** for an A474 stub.

Raw aligned `A474` words do occur outside the proven table region in several resource-heavy applets. They are kept as raw-word inventory only and are not promoted to executable calls without a demonstrated code boundary and call target.

## Adversarial disposition

The same raw marker `0x95088102` appearing in both 2005 ROMs and the changed `0x01020304` value in NEO 2013 are preserved as generation-specific slot data, not interpreted as encodings or hidden functions. A literal search in `ioma8/neo-re` also yielded no A474 symbol; this is only secondary corroboration.

Private static regression revalidates all three canonical ROM hashes, the exact index-285 slot values and non-pointer classification, all 16 EXTENDED applet hashes, their late-table prefix, and the absence of an A474 veneer in the demonstrated late window. Result: **57/57 PASS**.

Dynamic regression is not applicable because no callable handler is demonstrated.

## Publication boundary

`SYS_A474` is only a neutral trap/index identifier. Do not create a header or callable prototype for it from the current evidence.
