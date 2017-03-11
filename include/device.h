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
 * Rendering callback.
 */
typedef void (*NGXDRAWFUNC)(double dt, void* ptr);

/**
 * Keyboard input callback.
 */
typedef void (*NGXKEYFUNC)(int key, void* ptr);


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
 * Set new draw func.
 */
int ngxDrawFunc(NGXDEVICE dev, NGXDRAWFUNC func);

/**
 * Set key func.
 */
int ngxKeyFunc(NGXDEVICE dev, NGXKEYFUNC func);

/**
 * Set user pointer.
 */
int ngxPointer(NGXDEVICE dev, void* ptr);

/**
 * Time from device start.
 */
double ngxNow(const NGXDEVICE dev);

/**
 * Write formatted message to log.
 */
int ngxLog(const NGXDEVICE dev, const char* format, ...);

/**
 * Is key down.
 */
int ngxIsKeyDown(const NGXDEVICE dev, int key);

int ngxSetPerspective(NGXDEVICE dev, int enable);

#endif // __DEVICE_HEADER__
