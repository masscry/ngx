/**
 * @file device.h
 * @author timur
 *
 * Rendering device.
 *
 */

#ifndef __DEVICE_HEADER__
#define __DEVICE_HEADER__

struct ngx_device_t;

typedef struct ngx_device_t* NGXDEVICE;

NGXDEVICE ngxInit();

void ngxCleanup(NGXDEVICE* pdev);

int ngxRun(NGXDEVICE dev);

#endif // __DEVICE_HEADER__
