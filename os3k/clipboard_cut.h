#ifndef _OS3K_CLIPBOARD_CUT_H_
#define _OS3K_CLIPBOARD_CUT_H_

#include <stdint.h>

/* Copy from active file into clipboard, then delete the copied bytes. */
int32_t SYS_A204(uint32_t count, uint8_t clipboard_tag);

#endif
