# OS3K Manager LIST_APPLETS request

This note documents the externally observable Manager-protocol contract for enumerating installed SmartApplets. It is based on historical AlphaSync/neotools behavior and cross-generation verification against canonical OS3K firmware. No firmware bytes or private disassembly are included here.

## Wire contract

`LIST_APPLETS` uses request selector `0x04`.

- 32-bit request argument: `first`, the ordinal applet offset at which enumeration should begin.
- 16-bit trailing argument: `count`, the maximum number of applet headers requested.
- Response selector: `0x44`.
- 32-bit response argument: payload length in bytes.
- 16-bit response trailing field: additive payload checksum.
- Payload: consecutive SmartApplet headers.

Each returned SmartApplet header is `0x84` bytes.

## Enumeration semantics

The firmware maintains up to 32 physical applet slots. `first` is **not** a raw physical slot number: empty slots do not consume it. Instead, `first` counts present applets in enumeration order. Once `first` reaches zero, present applets are emitted and `count` is decremented for each returned header.

This behavior is invariant in the canonical AlphaSmart 3000 System 3, 2005 NEO System 3, and 2013 NEO System 3 images examined for this project.

For every emitted applet, the firmware copies one complete `0x84`-byte header, adds its bytes into the response checksum, and increases the response payload length by `0x84`. Enumeration stops when the requested count is exhausted or the physical 32-slot inventory has been scanned.

## Host-side page size

Historical AlphaSync deliberately requests at most seven headers per transaction and warns that larger requests can overflow/crash some NEO units. That value is a host-side safety policy, **not** a demonstrated firmware validation rule. Implementations should retain the conservative seven-header page size for compatibility.

## ABI status

This request belongs to the Manager transport protocol. It is not an A-line syscall and does not extend the OS3K A-line ABI frontier.
