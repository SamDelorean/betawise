#ifndef OS3K_SYS_A2D0_H
#define OS3K_SYS_A2D0_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A2D0.
 *
 * Five consumed physical stack slots are confirmed. `state` is a mandatory
 * pointer to a 44-byte-or-larger context copied into a local snapshot.
 * `limit` is consumed as a full 32-bit value. Only the low byte of physical
 * slots 3 through 5 is consumed.
 *
 * A2D0 runs SYS_A2CC against the local snapshot with no output buffer, then
 * invokes an internal eight-argument helper using both original and snapshot
 * state. The top-level context is only read directly, but the helper mutates
 * an object reached through its first pointer field, so callers must not infer
 * absence of transitive side effects from the const qualifier.
 *
 * D0.L is contractual. The mechanically closed return domain in all three
 * canonical ROMs is exactly 0 or 0x02000005. The vendor meaning of the
 * non-zero status is unknown and intentionally unnamed.
 */
uint32_t SYS_A2D0(const void *state,
                  uint32_t limit,
                  uint8_t arg3,
                  uint8_t arg4,
                  uint8_t arg5);

#endif /* OS3K_SYS_A2D0_H */
