#ifndef OS3K_SYS_A2DC_H
#define OS3K_SYS_A2DC_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A2DC.
 *
 * Platform divergence is contractual:
 * - AlphaSmart 3000 implements an unsupported-operation stub and ignores all
 *   stack arguments, returning 0x02000003.
 * - NEO 2005/2013 consume six 32-bit stack slots. `state` is mandatory;
 *   source_4e/source_52/source_56/source_5a are nullable pointers that replace
 *   corresponding pointer fields in the state object when non-null; only the
 *   low byte of post_flag is consumed.
 *
 * NEO returns either 0 or 0x02000005. Across supported platforms the observed
 * return domain is {0, 0x02000003, 0x02000005}.
 *
 * Original vendor names for this operation and these status values are
 * unknown.
 */
uint32_t SYS_A2DC(void *state,
                  const void *source_4e,
                  const void *source_52,
                  const void *source_56,
                  const void *source_5a,
                  uint8_t post_flag);

#endif /* OS3K_SYS_A2DC_H */
