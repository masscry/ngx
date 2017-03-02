/**
 * @file device.h
 * @author timur
 *
 * Rendering device.
 *
 */

#ifndef __DEVICE_HEADER__
#define __DEVICE_HEADER__

/**
 * Rendering device guts.
 */
struct ngx_device_t;

/**
 * Public interface for rendering device.
 */
typedef struct ngx_device_t* NGXDEVICE;

/**
 * Initialize rendering device.
 */
NGXDEVICE ngxInit();

/**
 * Cleanup rendering device.
 */
void ngxCleanup(NGXDEVICE* pdev);

/**
 * Update rendering device state.
 */
int ngxUpdate(NGXDEVICE dev);

/**
 * Time from device start.
 */
double ngxNow(const NGXDEVICE dev);

/**
 * Write formatted message to log.
 */
int ngxLog(const NGXDEVICE dev, const char* format, ...);

#endif // __DEVICE_HEADER__
