#ifndef OS3K_SYS_A2BC_H
#define OS3K_SYS_A2BC_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A2BC.
 *
 * All eight arguments occupy 32-bit physical stack slots. `state` and
 * `value` are consumed as full longwords; the remaining six arguments are
 * consumed from the low word of their slots.
 *
 * The four coordinate/extent arguments form two position+extent pairs.
 * AS3000 accepts the legacy 40 x 4 domain. NEO generations accept a larger
 * 264 x 66 domain and convert legacy-scale horizontal values by 6 and
 * vertical values by 16 when applicable. NEO 2013 also materializes extra
 * flag bits that older generations do not.
 *
 * The return value is a full signed 32-bit status. Zero is the normal success
 * value. Observed explicit non-zero values include 0x01000001, 0x02000001,
 * and 0x02000004, and complete helper results can also be propagated.
 * Original vendor names for the routine, status values and structure remain
 * unknown.
 */
int32_t SYS_A2BC(void *state,
                 uint16_t selector,
                 uint16_t flags,
                 uint32_t value,
                 uint16_t coord_x,
                 uint16_t coord_y,
                 uint16_t extent_x,
                 uint16_t extent_y);

#endif /* OS3K_SYS_A2BC_H */
