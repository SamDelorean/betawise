#ifndef _OS3K_CLIPBOARD_COPY_H_
#define _OS3K_CLIPBOARD_COPY_H_

#include <stdint.h>

/* Copy from active file at cursor into the clipboard. */
int32_t SYS_A200(uint32_t count, uint8_t clipboard_tag);

#endif
