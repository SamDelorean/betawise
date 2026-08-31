#ifndef OS3K_SYS_A41C_H
#define OS3K_SYS_A41C_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A41C / index 263.
 *
 * AS3000 (2005) and NEO (2005) do not expose a callable pointer at this
 * table index. NEO (2013) accepts one 32-bit stack slot whose low byte is
 * the effective selector and returns either NULL or a firmware-owned
 * structure pointer in D0.L.
 *
 * The selector's vendor meaning and the concrete structure type have not
 * been recovered independently, so the public contract intentionally uses
 * uint8_t and void * rather than an invented enum or typedef.
 */
void *SYS_A41C(uint8_t selector);

#endif /* OS3K_SYS_A41C_H */
