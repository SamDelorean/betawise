#ifndef _OS3K_CLIPBOARD_GET_H_
#define _OS3K_CLIPBOARD_GET_H_

#include <stdint.h>

int32_t ClipboardGet(uint8_t *buffer, uint32_t count,
                     uint8_t read_variant);

#endif
