#ifndef OS3K_SYS_A2E8_H
#define OS3K_SYS_A2E8_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A2E8.
 *
 * Five physical 32-bit stack slots are present in official AlphaWordPlus
 * callers. The firmware handler consumes only the first four; the fifth slot
 * is retained here because removing it would change the physical call shape.
 *
 * state is mutable. selector is a full 32-bit value: its low byte selects a
 * dispatch route, while several routes compare the complete value. arg3 is a
 * caller-supplied pointer/structure used by selected routes. out_value is a
 * mandatory 32-bit output pointer and is cleared before dispatch.
 *
 * All reachable handler exits explicitly construct D0.L as 0 or 1. Official
 * AlphaWordPlus callers from both the 2005 and NEO-era corpora consume only
 * D0.B, so the neutral contract exposes an 8-bit result. This is intentionally
 * not named or typed as a vendor boolean.
 *
 * unused_arg5 is passed as a scratch pointer by the observed callers but is
 * not read by the AS3000 2005, NEO 2005, or NEO 2013 handlers.
 */
uint8_t SYS_A2E8(void *state,
                 uint32_t selector,
                 void *arg3,
                 uint32_t *out_value,
                 void *unused_arg5);

#endif /* OS3K_SYS_A2E8_H */
