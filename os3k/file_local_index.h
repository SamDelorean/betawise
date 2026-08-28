#ifndef _OS3K_FILE_LOCAL_INDEX_H_
#define _OS3K_FILE_LOCAL_INDEX_H_

#include <stdint.h>

/* Get/set the local file index stored at descriptor +0x46.
 * index_out == NULL sets index_value. Non-NULL index_out reads the field and
 * ignores index_value. Success returns 0; resolution failure returns status.
 */
int32_t SYS_A1F4(uint16_t file_id, uint16_t index_value, uint16_t *index_out);

#endif
