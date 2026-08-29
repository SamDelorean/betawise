#ifndef OS3K_SYS_A29C_H
#define OS3K_SYS_A29C_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A29C.
 *
 * No vendor name has been recovered. The handler takes no arguments and
 * returns an 8-bit status value. Known mechanics:
 *   - retries an internal helper while it returns 0x14;
 *   - propagates other non-zero helper results;
 *   - after helper success, waits while the shared state is 7 or 4;
 *   - shared state 1 returns 0, any other final state returns 0x1F.
 *
 * The high bits of D0 are not part of the reconstructed contract.
 */
uint8_t SYS_A29C(void);

#endif /* OS3K_SYS_A29C_H */
