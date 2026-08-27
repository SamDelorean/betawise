#ifndef _OS3K_APPLET_SELECTION_MASK_H_
#define _OS3K_APPLET_SELECTION_MASK_H_

#include <stdint.h>

/*
 * Reconstructed System 3 SmartApplet selection-block mask ABI.
 *
 * These services operate on a 32-entry runtime table of 32-bit masks.
 * Original public/vendor symbols have not been recovered, so the generic
 * A-line names are intentionally retained. See
 * docs/applet-selection-mask-closure.md.
 */

uint32_t SYS_A260(uint32_t applet_index);
void SYS_A264(uint32_t applet_index);
void SYS_A268(uint32_t applet_index);
void SYS_A26C(uint32_t applet_index);
void SYS_A270(uint32_t applet_index);

#endif
