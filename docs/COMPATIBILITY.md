# Platform compatibility

BetaWise targets the AlphaSmart System 3 / OS3K family rather than one single hardware implementation.

## AlphaSmart 3000

The AS3000 uses a Motorola MC68EZ328 DragonBall EZ and a 40x4 character LCD driven through external character-controller hardware. Historical production material documents 256 KiB SRAM and 1 MiB Flash. Its screen and low-level hardware differ substantially from the NEO.

## AlphaSmart NEO / NEO2

NEO-family devices use a Motorola 68VZ328-class DragonBall platform and a graphical LCD architecture. BetaWise contains code paths that use OS3K font and LCD information when available.

## Shared OS3K ABI

The same `.OS3KApp` format and many A-line calls are usable across AS3000 and NEO-family System 3 implementations. This does **not** imply identical firmware internals, ROM addresses, display hardware, memory maps, or peripheral implementations.

Use `CallSysInt` queries such as `SYS_INT_GET_HW_LEVEL`, `SYS_INT_GET_LCD_WIDTH`, `SYS_INT_GET_LCD_HEIGHT`, `SYS_INT_GET_ROW_COUNT`, and `SYS_INT_IS_NEO_2` when behavior must adapt at runtime.

## Compatibility policy

A syscall documented as common must have evidence that its contract is stable across the relevant System 3 targets. Hardware-specific internals discovered during emulator work should not be exposed as portable SDK APIs unless a compatibility abstraction is provided.

The emulator project is treated as a feedback source for BetaWise: firmware execution can reveal ABI details, while BetaWise probes provide regression cases for the emulator. Hardware testing remains the final arbiter when emulator and firmware analysis disagree.
