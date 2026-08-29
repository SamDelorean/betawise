#ifndef OS3K_SYS_A2CC_H
#define OS3K_SYS_A2CC_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A2CC.
 *
 * Ten physical 32-bit stack slots are present. `state`, `limit`,
 * `out_bytes`, and `arg10` are consumed as full longwords/pointers. Only the
 * low byte of physical slots 4 through 9 is consumed by the three canonical
 * implementations.
 *
 * `state` is mandatory and mutable. `out_bytes` is nullable; when non-NULL,
 * bytes produced by the state machine are written sequentially. `arg10` is a
 * nullable read-only byte pointer used by comparison/match paths. Original
 * vendor meanings for the six byte arguments remain unknown.
 *
 * D0.L is contractual. Confirmed ROM callers consume it as a full 32-bit
 * processed-byte count (including pointer/count arithmetic). The handler also
 * has explicit 0 and 1 terminal paths; these values are naturally contained
 * in the same uint32_t domain. Do not model the return as void, uint8_t, or
 * bool.
 *
 * AS3000 and NEO share the same external ABI. NEO 2005/2013 add one state
 * field at +0x10 to each of two internal helper calls; this is a real internal
 * generational difference and does not add an external argument.
 */
uint32_t SYS_A2CC(void *state,
                  uint32_t limit,
                  uint8_t *out_bytes,
                  uint8_t arg4,
                  uint8_t arg5,
                  uint8_t arg6,
                  uint8_t arg7,
                  uint8_t arg8,
                  uint8_t arg9,
                  const uint8_t *arg10);

#endif /* OS3K_SYS_A2CC_H */
