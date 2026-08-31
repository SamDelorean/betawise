#ifndef OS3K_SYS_A404_H
#define OS3K_SYS_A404_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A404 / index 257.
 *
 * AS3000 (2005) and NEO (2005) do not expose a callable pointer at this
 * table index. NEO (2013) implements a zero-argument byte getter.
 *
 * The handler reads one byte from firmware-owned state and writes only D0.B.
 * A direct NEO 2013 firmware caller consumes exactly D0.B immediately after
 * the call, establishing an 8-bit contractual return. The field's semantic
 * meaning and vendor symbol are not recovered, so the neutral SYS_A404 name
 * is retained and no boolean/enum meaning is assigned.
 */
uint8_t SYS_A404(void);

#endif /* OS3K_SYS_A404_H */
