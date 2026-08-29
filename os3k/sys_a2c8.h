#ifndef OS3K_SYS_A2C8_H
#define OS3K_SYS_A2C8_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A2C8.
 *
 * Three physical 32-bit stack slots are present. `source` and `out_state`
 * are pointers; only the low byte of the third slot is consumed as selector.
 * `out_state` is mandatory in all observed implementations.
 *
 * Common effects before selector dispatch:
 *   - clear 18 bytes at out_state + 0x14,
 *   - clear byte out_state + 0x2A,
 *   - store source at out_state + 0x00.
 *
 * Selectors 0, 1 and 2 are recognized. Any other selector returns
 * 0x02000002 after the common mutations above. Valid selectors return 0.
 * The full 32-bit D0 value is therefore contractual; do not model this as
 * a byte or boolean.
 *
 * AS3000 and NEO differ materially: NEO 2005/2013 additionally derive the
 * longword at out_state + 0x10 from one of source +0x4E/+0x52/+0x56/+0x5A,
 * while AS3000 leaves out_state + 0x10 untouched.
 *
 * Original vendor names for this routine, selectors, structures and status
 * values remain unknown.
 */
uint32_t SYS_A2C8(const void *source,
                  void *out_state,
                  uint8_t selector);

#endif /* OS3K_SYS_A2C8_H */
