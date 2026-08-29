#ifndef _OS3K_SYS_A294_H_
#define _OS3K_SYS_A294_H_

#include <stdint.h>

/*
 * Starts the associated subsystem operation with one full-width 32-bit value.
 * The wrapper retries worker status 0x12, propagates other worker errors, then
 * waits for state 6 to resolve. Final state 3 is success; other terminal states
 * map to 0x1e. Only the low byte of D0 is contractual.
 *
 * The original vendor symbol and the semantic meaning of value are not
 * recovered, so the neutral A-line name is intentionally retained.
 * See docs/sys-a294-closure.md.
 */
uint8_t SYS_A294(uint32_t value);

#endif
