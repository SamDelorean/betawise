#ifndef _OS3K_APPLET_SELECTION_H_
#define _OS3K_APPLET_SELECTION_H_

#include <stdint.h>

/*
 * Reconstructed System 3 selected-SmartApplet context ABI.
 *
 * Mechanical behavior is closed across AS3000 2005, NEO 2005 and NEO 2013,
 * but reliable original public names have not been recovered. Generic A-line
 * names are intentionally retained. See docs/applet-selection-closure.md.
 */

/* Installs a runtime SmartApplet index only when its per-applet selection-block
 * mask is zero. On rejection returns 0 and preserves the prior selection; on
 * success stores and returns applet_index. The handler itself does not bounds-
 * check before indexing the mask table: callers must supply a valid runtime
 * index (normal System 3 range 0..31). A successful index 0 is indistinguishable
 * from rejection by return value alone.
 */
uint32_t SYS_A22C(uint32_t applet_index);

/* Clears the selected-applet context to sentinel -1 and deterministically
 * returns -1 in D0.
 */
int32_t SYS_A230(void);

/* Returns the selected applet header's 16-bit id field (+0x14), or 0 when the
 * selected-index sentinel is -1. Only the low word of D0 is contractual.
 */
uint16_t SYS_A234(void);

#endif
