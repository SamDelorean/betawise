#ifndef OS3K_SYS_A298_H
#define OS3K_SYS_A298_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Neutral reconstructed contract for System 3 trap A298.
 *
 * The original vendor symbol is unknown. Callers use two 32-bit stack slots,
 * while the compared firmware handlers consume only the low byte of each slot.
 * selector is mechanically constrained to 0..4; its vendor meanings remain
 * unknown. The return contract is the low byte of D0.
 */
uint8_t SYS_A298(uint8_t selector, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif /* OS3K_SYS_A298_H */
