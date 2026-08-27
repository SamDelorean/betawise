#ifndef _OS3K_FILE_CLIPBOARD_STATE_H_
#define _OS3K_FILE_CLIPBOARD_STATE_H_

#include <stdint.h>

/*
 * Reconstructed System 3 clipboard state/capacity ABI.
 *
 * Mechanical contracts are closed across AS3000 2005, NEO 2005 and NEO 2013,
 * but reliable original public names have not been recovered. Generic A-line
 * names are intentionally retained. See docs/clipboard-state-capacity-closure.md.
 */

/* Returns the low byte of the shared clipboard metadata/tag field.
 * Upper D0 bits are not explicitly cleared by the firmware handler, so only the
 * uint8_t result is contractual. Known tag values include 1 and an internal
 * comparison against 4; symbolic enum names remain unknown.
 */
uint8_t SYS_A218(void);

/* Returns clipboard logical length for the selected read representation.
 * read_variant == 3 calculates the control-code-aware/filtered output length;
 * every other value returns raw clipboard current_size.
 */
uint32_t SYS_A21C(uint8_t read_variant);

/* Returns the clipboard descriptor's storage/base pointer (+0x00). */
uint8_t* SYS_A220(void);

/* Returns the clipboard descriptor's maximum allocated capacity (+0x0C). */
uint32_t SYS_A224(void);

/* Adjusts clipboard allocation for requested_size, clamps to resulting max_size,
 * writes the effective value to current and recoverable size, and returns that
 * effective value. Does not copy/initialize data, change tag, or move cursor.
 */
uint32_t SYS_A228(uint32_t requested_size);

#endif
