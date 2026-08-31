# SYS_A478 / index 286 — non-callable ABI slot

Status: mechanically closed, confidence A for the canonical ROM set. This entry does **not** expose a callable C contract.

## Canonical vector evidence

The three canonical OS3K ROMs contain non-pointer values at index 286:

- AlphaSmart 3000 (2005): file `+0x02B094` = `0x95017508`, outside the canonical ROM runtime window `0x004C0000..0x00503BEF`.
- NEO (2005): file `+0x0319BE` = `0x95017508`, outside `0x005C0000..0x00600017`.
- NEO (2013): file `+0x03F5D6` = `0x05060708`, outside the mapped firmware runtime window.

Accordingly, no handler address, handler bytes, arguments, return value, globals, structures, or helpers are assigned to `SYS_A478`.

## Call-site evidence

The official 41-SmartApplet corpus was checked independently for this slot. None of the 16 EXTENDED A-line tables contains `A478`; the 14 LEGACY applets terminate before this late range and 11 applets have no A-line table. Result: **0/41 A478 stubs**.

A raw-word adversarial scan found `A478` only in the data payload of Thesaurus Large; no occurrence is an A-line table entry or demonstrated executable callsite. The three canonical firmware images contain zero raw `0xA478` words.

## Regression

A private static regression covering canonical ROM hashes/sizes, index-286 values and runtime rejection, all 16 EXTENDED applet hashes/table windows, the raw-word adversarial inventory, and 41-file structural accounting completed **90/90 PASS**.

## Disposition

`SYS_A478` is retained only as a neutral ABI coordinate/name. For the canonical corpus it is classified as:

`NO_CONTRATO_CALLABLE / HUECO ABI`

No vendor name, prototype, enum, status code, or semantics are inferred from the marker bytes.