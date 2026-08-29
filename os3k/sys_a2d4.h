#ifndef OS3K_SYS_A2D4_H
#define OS3K_SYS_A2D4_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A2D4.
 *
 * Four physical stack slots are consumed. `state` is a mandatory pointer to
 * a two-longword-or-larger context. `source` is consumed as a full 32-bit
 * pointer and dereferenced by the internal worker. `value` is consumed as a
 * full 32-bit scalar. Only the low byte of `flags` is used.
 *
 * The handler first validates state->field0. It can return 0x02000005 or
 * 0x02000006 directly. Otherwise it invokes an internal six-argument worker
 * with [state->field0, state->field4, source, value, flags, &result] and
 * returns that output longword.
 *
 * Across AS3000 2005, NEO 2005 and NEO 2013 the exact contractual D0.L
 * domain is:
 *   0,
 *   0x01000001,
 *   0x01000002,
 *   0x02000005,
 *   0x02000006.
 *
 * Original vendor names for the operation, fields, flags and status values
 * remain unknown.
 */
uint32_t SYS_A2D4(const void *state,
                  const uint8_t *source,
                  uint32_t value,
                  uint8_t flags);

#endif /* OS3K_SYS_A2D4_H */
