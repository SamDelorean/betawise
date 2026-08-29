#ifndef OS3K_SYS_A2A8_H
#define OS3K_SYS_A2A8_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A2A8.
 *
 * The physical ABI uses three 32-bit stack slots. `context` is consumed as a
 * full longword and stored without dereference by A2A8. `payload` is a 32-bit
 * pointer and is dereferenced; `payload_size` consumes only the low byte of
 * its physical slot and must be non-zero.
 *
 * A2A8 delegates the payload operation to A2A4 with selector 2, then waits up
 * to ten byte-sized ticks for shared completion state to clear. Local status
 * values include 0x19 and 0x1B; non-zero A2A4 status is propagated and normal
 * completion returns a shared result byte. The high bits of D0 are not part
 * of the reconstructed contract.
 */
uint8_t SYS_A2A8(uint32_t context,
                 const uint8_t *payload,
                 uint8_t payload_size);

#endif /* OS3K_SYS_A2A8_H */
