#ifndef _OS3K_APPLET_SETTINGS_H_
#define _OS3K_APPLET_SETTINGS_H_

/*
 * Reconstructed System 3 current-SmartApplet settings ABI.
 *
 * A278 returns the settings region for the SmartApplet currently executing,
 * or NULL when the runtime index is invalid or that applet declares no
 * settingsOffset. The original public/vendor symbol has not been recovered,
 * so the generic A-line name is intentionally retained.
 *
 * See docs/applet-current-settings-closure.md.
 */
void *SYS_A278(void);

#endif
