#ifndef _OS3K_FILE_DYNAMIC_H_
#define _OS3K_FILE_DYNAMIC_H_

#include <stdint.h>

/*
 * Reconstructed System 3 dynamic-file identity ABI.
 *
 * These interfaces retain their A-line names because the behavior is closed but
 * reliable original System 3 public symbols have not been recovered.  See
 * docs/file-identity-dynamic-closure.md.
 */

/* Create a dynamic descriptor/file.
 * effective maximum allocation = max(requested_max_size, min_size, 0x200).
 * Success returns the canonical 16-bit token. Known raw errors:
 * -9 descriptor table full, -10 invalid/unavailable token group,
 * -12 allocation failure, -13 invalid name, -15 duplicate name.
 */
int32_t SYS_A1E8(uint8_t token_group, const char *name,
                 uint32_t min_size, uint32_t requested_max_size);

/* Remove a descriptor and reclaim/compact its allocation/table entry.
 * Success returns 0; resolver failure returns the resolver status.
 * Destructive: this removes the file descriptor itself, not just its contents.
 */
int32_t SYS_A1EC(uint16_t file_id);

/* Get or set descriptor filename (+0x34, maximum 15 chars plus NUL).
 * set_name == 0: optional copy to name_buffer and return internal name pointer.
 * set_name != 0: validate/normalize/duplicate-check new name; return internal
 * pointer on success or NULL on failure. Resolver failure also returns NULL.
 */
char *SYS_A1F0(uint16_t file_id, char *name_buffer, uint8_t set_name);

/* Get/set local file index stored at descriptor +0x46.
 * index_out == NULL sets index_value. Non-NULL index_out reads the field and
 * ignores index_value. Success returns 0; resolution failure returns status.
 */
int32_t SYS_A1F4(uint16_t file_id, uint16_t index_value, uint16_t *index_out);

/* Reverse local-index lookup in the current group.
 * Returns canonical token when found or raw -11 when no match exists.
 */
int32_t SYS_A1F8(uint16_t local_index);

/* Build a 16-bit token from its low/high bytes and validate it through the file
 * resolver. Returns the token on success or the resolver status on failure.
 */
int32_t SYS_A1FC(uint8_t token_low, uint8_t token_high);

#endif
