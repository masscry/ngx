/**
 * @file device.h
 * @author timur
 *
 * Rendering device.
 *
 */

#ifndef __DEVICE_HEADER__
#define __DEVICE_HEADER__

enum ngxKeys {
    NGX_ESC = 65307,
    NGX_LEFT = 65361,
    NGX_RIGHT = 65363,
    NGX_UP = 65362,
    NGX_DOWN = 65364,
    NGX_SHIFT = 65505,
    NGX_CTRL = 65507
};

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

typedef void (*NGXLOADRESOURCEFUNC)(int load, void* ptr);

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
 * Shutdown engine.
 * 
 * @param dev devive
 * @return return non-zero on errors
 */
int ngxShutdown(NGXDEVICE dev);

/**
 * Set new draw func.
 */
int ngxDrawFunc(NGXDEVICE dev, NGXDRAWFUNC func);

/**
 * Set key func.
 */
int ngxKeyFunc(NGXDEVICE dev, NGXKEYFUNC func);

/**
 * Set load resources func.
 * 
 * @param dev device
 * @param func function
 * @return non zero on errors.
 */
int ngxLoadResourcesFunc(NGXDEVICE dev, NGXLOADRESOURCEFUNC func);

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

/**
 * Set perspective mode.
 * 
 * @param dev device
 * @param enable 0 - flat, 1 - 3D, 2 - 2D
 * @return non-zero on error
 */
int ngxSetPerspective(NGXDEVICE dev, int enable);

/**
 * Set initial resolution.
 * 
 * @param dev device
 * @param width initial width
 * @param height initial height
 * @return non-zero on error
 */
int ngxSetResolution(NGXDEVICE dev, int width, int height);

#endif // __DEVICE_HEADER__
