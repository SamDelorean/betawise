#ifndef _OS3K_CLIPBOARD_PASTE_H_
#define _OS3K_CLIPBOARD_PASTE_H_

#include <stdint.h>

/* Paste clipboard contents using the selected write/read variants. */
int32_t SYS_A208(uint8_t write_mode, uint8_t read_variant);

#endif
