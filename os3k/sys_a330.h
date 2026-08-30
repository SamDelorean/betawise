#ifndef OS3K_SYS_A330_H
#define OS3K_SYS_A330_H

#include <stdint.h>

/*
 * Raw OS3K A330 keyboard-character service.
 *
 * The firmware consumes no caller arguments and returns a byte-valued
 * character zero-extended to the full D0 longword. Zero is used internally
 * to continue waiting and is not returned by the reconstructed handlers;
 * no EOF/negative path is present in the compared firmware generations.
 *
 * BetaWise's public getchar() implementation is a separate wrapper layer.
 */
int32_t _OS3K_getchar(void);

#endif /* OS3K_SYS_A330_H */
