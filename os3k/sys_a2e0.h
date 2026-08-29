#ifndef OS3K_SYS_A2E0_H
#define OS3K_SYS_A2E0_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A2E0.
 *
 * One physical 32-bit stack slot is consumed: a mandatory mutable state
 * pointer. The compared handlers deliberately manage a full 32-bit status.
 *
 * In AS3000 2005 and NEO 2005 the handler bodies are identical modulo two
 * relocated helper addresses. NEO 2013 adds two preparation calls before the
 * common core when the state guard is clear.
 *
 * The first correlated helper always exits with D0.L = 0. The second helper
 * clamps its result to a non-negative range before returning. A2E0 contains a
 * defensive 0x02000004 path for a negative second-helper result, but that path
 * is not reachable with any of the three canonical helper implementations.
 * Consequently every reachable exit in the compared ROMs returns D0.L = 0.
 *
 * The function is not modeled as void because the firmware intentionally
 * constructs, tests and normalizes a 32-bit status value.
 */
uint32_t SYS_A2E0(void *state);

#endif /* OS3K_SYS_A2E0_H */
