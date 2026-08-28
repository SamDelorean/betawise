#ifndef _OS3K_CLIPBOARD_SET_H_
#define _OS3K_CLIPBOARD_SET_H_

#include <stdint.h>

int32_t ClipboardSet(const uint8_t *buffer, uint32_t count,
                     uint8_t clipboard_tag);

#endif
