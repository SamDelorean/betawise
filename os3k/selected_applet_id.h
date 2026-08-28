#ifndef _OS3K_SELECTED_APPLET_ID_H_
#define _OS3K_SELECTED_APPLET_ID_H_

#include <stdint.h>

/* Return the selected applet header's 16-bit id, or 0 when selection is -1.
 * Only the low word of D0 is contractual.
 */
uint16_t SYS_A234(void);

#endif
