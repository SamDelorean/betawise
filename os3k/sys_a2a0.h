#ifndef OS3K_SYS_A2A0_H
#define OS3K_SYS_A2A0_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A2A0.
 *
 * No vendor name has been recovered. The physical ABI uses one 32-bit caller
 * slot, but the handler consumes only its low byte. The selector is accepted
 * for values 0..4 and is used to address a stride-8 state table.
 *
 * The routine coordinates shared subsystem state, invokes the A2A4 handler,
 * and returns a byte-sized status. Known local status values include 0x15,
 * 0x16, 0x19, 0x20 and 0x1B; non-zero A2A4 status is propagated through D0.B.
 *
 * The high bits of D0 are not part of the reconstructed contract.
 */
uint8_t SYS_A2A0(uint8_t selector);

#endif /* OS3K_SYS_A2A0_H */
