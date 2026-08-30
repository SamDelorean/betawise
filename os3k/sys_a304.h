#ifndef OS3K_SYS_A304_H
#define OS3K_SYS_A304_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A304.
 *
 * The handler receives three physical 32-bit stack slots. state is a mutable
 * firmware-owned structure. value is consumed only through its low 16 bits;
 * the upper half of that physical slot has no observable effect in the three
 * canonical firmware generations. string is a required NUL-terminated byte
 * string and is read, not modified, by the handler.
 *
 * On the observed success path the handler stores value into a 16-bit field
 * of state. Internally it compares strings through the firmware's ASCII
 * case-normalization helper. The subsystem and vendor field names remain
 * unknown, so this header intentionally keeps neutral names.
 *
 * All reachable exits define D0.B as 0 or 1. Official AlphaWordPlus callers
 * test only D0.B; upper D0 bits are not contractual. The public result is
 * therefore exposed as uint8_t without assigning a vendor boolean meaning.
 */
uint8_t SYS_A304(void *state, uint16_t value, const char *string);

#endif /* OS3K_SYS_A304_H */
