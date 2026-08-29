#ifndef OS3K_SYS_A2D8_H
#define OS3K_SYS_A2D8_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A2D8.
 *
 * Two physical 32-bit stack slots are consumed. `state` is a mandatory
 * top-level context pointer. Only the low byte of `post_flag` is consumed.
 * The object reached through state->field0 is mutated; const therefore only
 * describes the top-level container.
 *
 * A2D8 copies state->field4 into pointee+0x24, clamps/refreshes that field,
 * invokes a correlated status helper, optionally invokes a post-success
 * helper when post_flag != 0, and deliberately returns the saved status in
 * D0.L.
 *
 * In AS3000 2005, NEO 2005 and NEO 2013 every reachable exit returns exactly
 * 0. The full 32-bit return handling is still deliberate (explicit zero on
 * one path and D0->D7->D0 propagation on the normal path), so this is not
 * modeled as void.
 */
uint32_t SYS_A2D8(const void *state, uint8_t post_flag);

#endif /* OS3K_SYS_A2D8_H */
