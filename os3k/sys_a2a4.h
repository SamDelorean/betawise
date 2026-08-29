#ifndef OS3K_SYS_A2A4_H
#define OS3K_SYS_A2A4_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A2A4.
 *
 * No vendor name has been recovered. The physical ABI uses four 32-bit caller
 * slots. The handler consumes only the low byte of slots 1, 2 and 4; slot 3
 * is a 32-bit pointer.
 *
 * selector is accepted for values 0..4. flags is intentionally uint8_t rather
 * than bool: callers pass both 0 and 0x80, although the handler only tests
 * zero/non-zero and uses non-zero to set bit 7 in a generated header byte.
 *
 * Known local status values include 0x11, 0x16, 0x19, 0x20 and 0x21. Byte-
 * sized errors from the internal helper are propagated. The high bits of D0
 * are not part of the reconstructed contract.
 */
uint8_t SYS_A2A4(uint8_t flags,
                 uint8_t selector,
                 const uint8_t *payload,
                 uint8_t payload_size);

#endif /* OS3K_SYS_A2A4_H */
