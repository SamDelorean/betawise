#ifndef OS3K_SYS_A32C_H
#define OS3K_SYS_A32C_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A32C.
 *
 * A32C consumes no stack arguments. Its complete handler is a single
 * 32-bit load from a firmware global into D0 followed by RTS, so the full
 * longword result is mechanically contractual.
 *
 * Writers of the source global strongly correlate it with C-library error
 * state (including values matching ERANGE and ENOMEM), but no independent
 * vendor/public symbol has been recovered. Keep SYS_A32C and the return type
 * neutral rather than publishing an inferred errno name or signedness.
 */
uint32_t SYS_A32C(void);

#endif /* OS3K_SYS_A32C_H */
