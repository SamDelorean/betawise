#ifndef OS3K_SYS_A334_H
#define OS3K_SYS_A334_H

#include <stdint.h>

/*
 * Raw OS3K A334 blocking character input with echo.
 *
 * The handler consumes no caller arguments. It obtains one character through
 * A330, writes that character to stdout through A350/_OS3K_fputc, ignores the
 * fputc result, and returns the original byte sign-extended to D0.L.
 *
 * No independent vendor symbol name beyond SYS_A334 has been recovered.
 */
int32_t SYS_A334(void);

#endif /* OS3K_SYS_A334_H */
