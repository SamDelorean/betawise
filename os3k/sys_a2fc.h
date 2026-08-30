#ifndef OS3K_SYS_A2FC_H
#define OS3K_SYS_A2FC_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A2FC.
 *
 * Official AlphaWordPlus callers pass five physical 32-bit stack slots.
 * The firmware handler consumes only the first four; the fifth slot remains
 * in the declaration because removing it would alter the physical ABI.
 *
 * state is a mutable firmware-owned structure. selector is a full 32-bit
 * value: its low byte selects one of thirteen dispatch routes, while selected
 * paths compare the complete value. arg3 is a mutable caller-supplied
 * structure/pointer used by selected routes. out_value is a mandatory 32-bit
 * output pointer and is cleared before dispatch.
 *
 * Every reachable exit constructs D0.L as 0 or 1. The two independently
 * observed AlphaWordPlus callers copy D0 and subsequently test only its low
 * byte, so the neutral public contract exposes an 8-bit result. This is not
 * assigned a vendor boolean or semantic name.
 *
 * unused_arg5 is physically passed by callers but is not read or written by
 * the AS3000 2005, NEO 2005, or NEO 2013 handlers.
 */
uint8_t SYS_A2FC(void *state,
                 uint32_t selector,
                 void *arg3,
                 uint32_t *out_value,
                 void *unused_arg5);

#endif /* OS3K_SYS_A2FC_H */
