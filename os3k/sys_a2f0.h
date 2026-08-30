#ifndef OS3K_SYS_A2F0_H
#define OS3K_SYS_A2F0_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A2F0.
 *
 * Official callers construct exactly three physical 32-bit stack slots.
 * The firmware consumes only the low 16 bits of arg1_slot, treats state as a
 * mutable pointer, and consumes only the low 8 bits of arg3_slot.
 *
 * The full slot widths are preserved here instead of narrowing the prototype
 * to uint16_t/uint8_t so the public declaration does not assume historical C
 * integer-promotion or calling-convention details beyond what the binaries
 * demonstrate.
 *
 * D0.L is a contractual raw status. The handler explicitly returns 0 on its
 * success path and propagates nonzero longword statuses on failure. Vendor
 * status names and the semantic meaning of the arguments remain unknown.
 */
uint32_t SYS_A2F0(uint32_t arg1_slot,
                  void *state,
                  uint32_t arg3_slot);

#endif /* OS3K_SYS_A2F0_H */
