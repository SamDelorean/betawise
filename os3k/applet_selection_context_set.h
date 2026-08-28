#ifndef _OS3K_APPLET_SELECTION_CONTEXT_SET_H_
#define _OS3K_APPLET_SELECTION_CONTEXT_SET_H_

#include <stdint.h>

/* Install the selected SmartApplet runtime index when its selection-block mask is zero.
 * Returns 0 on rejection without changing the prior selection; on success returns
 * applet_index. The handler does not bounds-check before indexing the mask table.
 */
uint32_t SYS_A22C(uint32_t applet_index);

#endif
