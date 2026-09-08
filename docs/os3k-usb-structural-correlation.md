# OS3K USB structural correlation

Status: source-first structural note. This document describes recovered layout/behavior; it does not publish ROM images, private disassemblies, or Ghidra artifacts.

## Why this note exists

The A-line dispatcher address window is not a flat list of syscalls. Near the end of the physical lookup area, the nominal `A000 + 4*index` grid overlaps ordinary USB tables and descriptors. A dword at a nominal A-line coordinate must therefore not be promoted to an ABI entry unless it is independently demonstrated to be a valid handler with caller/source evidence.

Canonical images used for the private verification:

- AS3000 2005: `732dca63399d883071ce41cc07ad352fa19c506b7b8c307ace16d5c53e6f8708`
- NEO 2005: `5f550e48ad36892ee5f5a067e8c1b87f6bf504b6cb2c2dfc4098776fa8831a3d`
- NEO 2013: `32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0`

## Dispatcher geometry

- AS3000 2005 and NEO 2005 mask the A-line offset with `0x03FC`: the physical lookup window is 256 dwords (`A000..A3FC`).
- NEO 2013 masks with `0x07FC`: its physical window is 512 dwords (`A000..A7FC`).
- The larger NEO 2013 window does **not** imply 512 callable syscalls. The last independently demonstrated callable NEO 2013 entry is `A470`; `A474` begins a structural marker and is not promoted as a syscall.

## USB control-request tables

Historical `UsbMain.c` provides an independent source-first template for two 16-entry arrays: `DeviceRequest[16]` and `ClassRequest[16]`. Direct firmware verification shows the same functional ordering in all three generations.

Physical starts are deliberately not aligned to the nominal A-line dword grid:

| generation | file start | runtime start | interpretation |
| --- | ---: | ---: | --- |
| AS3000 2005 | `0x02AFE2` | `0x004EAFE2` | 16 DeviceRequest + 16 ClassRequest pointers |
| NEO 2005 | `0x03190C` | `0x005F190C` | same functional table pair |
| NEO 2013 | `0x03F628` | `0x0044F628` | same functional table pair |

This two-byte displacement is important. Several nominal A-line dwords inside the overlap are merely misaligned views across adjacent USB pointers. For example, the former nominal `A500` interpretation in NEO 2013 crosses two `USB_Reserved` entries and must not be treated as a handler.

## Objects immediately following the request tables

The verified layout is:

1. four five-byte sequences consumed by `USB_SetReport`;
2. two four-byte endpoint-index arrays;
3. 63-byte HID keyboard report descriptor;
4. two 18-byte USB device descriptors;
5. configuration/interface/HID/endpoint descriptor objects;
6. USB string descriptors.

### SetReport sequence table

The four rows are byte-identical across the three canonical images:

```text
E0 E1 E2 E3 E4
01 02 04 03 07
F0 F1 F2 F3 F4
07 03 01 04 02
```

`USB_SetReport` is independently identified by historical source and by its ClassRequest slot. The first row is externally validated on physical NEO hardware by `ioma8/neo-re`: it switches the device from HID keyboard product `081e:bd04` to direct-USB product `081e:bd01`.

The concrete effects of the other three rows remain **NO_RESUELTO / EVIDENCIA_INSUFICIENTE**. `neo-re` likewise keeps them as live-test candidates. In particular, the `01 02 04 03 07` values are compatible with the HID keyboard LED output-bit range, but that observation is only a hypothesis and is not promoted here to vendor semantics.

### Endpoint index arrays: resolved correction

The eight bytes after the SetReport rows are **not** an unidentified switch sequence. Historical `UsbRequestResponse.c` defines two four-byte arrays used by `USB_GetStatus`, `USB_ClearFeature`, and `USB_SetFeature`; their roles and ordering match the firmware objects exactly.

AS3000 2005:

```text
EndpointOutInd = 02 05 06 08
EndpointInInd  = 03 04 07 09
```

NEO 2005 / NEO 2013:

```text
OUT-role map = 00 02 03 04
IN-role map  = 01 02 03 04
```

The NEO arrays are functionally homologous to the historical AS3000 arrays, but this note does not claim that the historical C symbol names survived unchanged in NEO binaries.

This resolves a point still described as “needs further investigation” in the current `ioma8/neo-re` direct-USB notes: the eight-byte tail has a source-correlated endpoint-index role. The different AS3000 versus NEO values are retained as a real generational difference rather than normalized away.

## ABI consequence

These USB objects demonstrate why nominal A-line coordinates cannot be audited as independent syscalls once the physical region has crossed into data. Classification must follow object boundaries first. A nominal dword that lands inside a pointer array, descriptor, string, padding, or other data object remains structural data unless an independent callable contract is demonstrated.

## Confidence

- Dispatcher masks/window bounds: **CONFIRMADO**.
- DeviceRequest/ClassRequest object identity and ordering: **CONFIRMADO** by historical source plus firmware correlation.
- SetReport four-row table and first-row direct-USB switch: **CONFIRMADO**; first-row physical effect additionally validated externally.
- Remaining three SetReport row effects: **DESCONOCIDO / NO_RESUELTO**.
- Endpoint index arrays and their IN/OUT roles: **CONFIRMADO** source-first; exact historical symbol names on NEO are **NO_AFIRMADOS**.
