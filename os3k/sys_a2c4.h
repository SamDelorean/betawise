#ifndef OS3K_SYS_A2C4_H
#define OS3K_SYS_A2C4_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A2C4.
 *
 * The syscall has one physical argument: a mandatory mutable state pointer.
 * The handler reads state fields and, when its validation/gating path permits,
 * forwards a derived eight-argument call to SYS_A2BC.
 *
 * The return is a full signed 32-bit status. Observed paths return zero,
 * propagate a negative helper status, return the explicit value 0x01000002,
 * or propagate the complete SYS_A2BC result. It must not be modeled as a
 * byte, word, boolean, or void result.
 *
 * AS3000 2005 and NEO 2005 are mechanically equivalent modulo relocations.
 * NEO 2013 adds preparatory state handling while preserving the same external
 * ABI. Original vendor names for the routine, state structure, helpers, and
 * status values remain unknown.
 */
int32_t SYS_A2C4(void *state);

#endif /* OS3K_SYS_A2C4_H */
