#ifndef OS3K_SYS_A350_H
#define OS3K_SYS_A350_H

#include "os3k.h"

/*
 * Raw OS3K A350 character-output service.
 *
 * The firmware consumes two 32-bit physical slots: the character value and
 * an opaque FILE/stream handle. The full D0 longword is contractual: success
 * returns the input character value; unsupported/failing paths return -1.
 *
 * BetaWise's public fputc() implementation is a separate wrapper layer.
 */
int _OS3K_fputc(int c, FILE *stream);

#endif /* OS3K_SYS_A350_H */
