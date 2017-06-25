#include <device.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>


#define NGXLOGFNAME ("ngx.log")

enum ngx_state_t {
    NGX_INIT = 0,
    NGX_LOOP = 1,
    NGX_CLOS = 2
};

struct ngx_xdata_t {
    Display* dpy;
    Window win;
    GLXContext glc;
    Atom WM_DELETE_WINDOW;
};

struct ngx_device_t {
    struct ngx_xdata_t dsp;
    uint64_t counter;
    int state;
    struct timespec start;
    NGXDRAWFUNC drawfunc;
    NGXKEYFUNC keyfunc;
    NGXLOADRESOURCEFUNC loadfunc;
    void* ptr;
    uint8_t keys[USHRT_MAX];
    int persp;
    int width;
    int height;
};

static void ngxDefaultKey(int key, void* ptr) {
    ;
}

static void ngxDefaultLoad(int load, void* ptr) {
    ;
}

static double angle = 0.0;

static void ngxDefaultDraw(double dt, void* ptr) {
    angle += dt * 10.0;

    gluLookAt(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    glBegin(GL_TRIANGLES);

    glColor3f(1.0f, 0.0f, 0.0f); // Красный
    glVertex3f(0.0f, 1.0f, 0.0f); // Верх треугольника (Передняя)
    glColor3f(0.0f, 1.0f, 0.0f); // Зеленный
    glVertex3f(-1.0f, -1.0f, 1.0f); // Левая точка
    glColor3f(0.0f, 0.0f, 1.0f); // Синий
    glVertex3f(1.0f, -1.0f, 1.0f); // Правая точка

    glColor3f(1.0f, 0.0f, 0.0f); // Красная
    glVertex3f(0.0f, 1.0f, 0.0f); // Верх треугольника (Правая)
    glColor3f(0.0f, 0.0f, 1.0f); // Синия
    glVertex3f(1.0f, -1.0f, 1.0f); // Лево треугольника (Правая)
    glColor3f(0.0f, 1.0f, 0.0f); // Зеленная
    glVertex3f(1.0f, -1.0f, -1.0f); // Право треугольника (Правая)

    glColor3f(1.0f, 0.0f, 0.0f); // Красный
    glVertex3f(0.0f, 1.0f, 0.0f); // Низ треугольника (Сзади)
    glColor3f(0.0f, 1.0f, 0.0f); // Зеленный
    glVertex3f(1.0f, -1.0f, -1.0f); // Лево треугольника (Сзади)
    glColor3f(0.0f, 0.0f, 1.0f); // Синий
    glVertex3f(-1.0f, -1.0f, -1.0f); // Право треугольника (Сзади)

    glColor3f(1.0f, 0.0f, 0.0f); // Красный
    glVertex3f(0.0f, 1.0f, 0.0f); // Верх треугольника (Лево)
    glColor3f(0.0f, 0.0f, 1.0f); // Синий
    glVertex3f(-1.0f, -1.0f, -1.0f); // Лево треугольника (Лево)
    glColor3f(0.0f, 1.0f, 0.0f); // Зеленный
    glVertex3f(-1.0f, -1.0f, 1.0f); // Право треугольника (Лево)

    glEnd();
}

NGXDEVICE ngxInit() {
    NGXDEVICE dev = 0;
    time_t now = 0;
    struct tm nowtm;
    char buffer[256];
    FILE* log = 0;

    log = fopen(NGXLOGFNAME, "w");
    if (log == 0) {
        fprintf(stderr, "Failed to open log file %s\n", NGXLOGFNAME);
        return 0;
    }

    now = time(0);
    localtime_r(&now, &nowtm);
    strftime(buffer, 256, "%d %b %Y %H:%M:%S", &nowtm);

    fprintf(log, "=== NGX ===\n");
    fprintf(log, "Current time: %s\n", buffer);

    dev = malloc(sizeof (struct ngx_device_t));
    if (dev == 0) {
        fprintf(log, "Failed to allocate memory for device\n");
        fclose(log);
        return 0;
    }

    memset(&(dev->dsp), 0, sizeof (struct ngx_xdata_t));
    dev->counter = 0;
    dev->state = NGX_INIT;
    clock_gettime(CLOCK_MONOTONIC, &dev->start);
    dev->drawfunc = ngxDefaultDraw;
    dev->keyfunc = ngxDefaultKey;
    dev->loadfunc = ngxDefaultLoad;
    memset(dev->keys, 0, USHRT_MAX);
    dev->persp = 0;
    dev->width = 800;
    dev->height = 600;
    fclose(log);
    return dev;
}

double ngxNow(const NGXDEVICE dev) {
    struct timespec ts;
    struct timespec start;

    if (dev == 0) {
        return -1.0;
    }
    start = dev->start;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    if (start.tv_nsec < ts.tv_nsec) {
        return (ts.tv_sec - start.tv_sec) + (ts.tv_nsec - start.tv_nsec)*1.0e-9;
    }
    return (ts.tv_sec - start.tv_sec - 1) + (ts.tv_nsec - start.tv_nsec + 1e9)*1.0e-9;
}

int ngxLog(const NGXDEVICE dev, const char* format, ...) {
    FILE* log = fopen(NGXLOGFNAME, "a");
    va_list vl;

    if (log == 0) {
        return -1;
    }

    fprintf(log, "[%15.5f] ", ngxNow(dev));
    va_start(vl, format);
    vfprintf(log, format, vl);
    va_end(vl);
    fprintf(log, "\n");
    fclose(log);
    return 0;
}

void ngxCleanup(NGXDEVICE* pdev) {
    if ((pdev != 0)&&((*pdev) != 0)) {
        free(*pdev);
        *pdev = 0;
    }
}

int ngxOpenWindow(NGXDEVICE dev) {
    struct ngx_xdata_t tdsp;
    Window root;
    XVisualInfo* vi;
    GLint attr[] = {
        GLX_RGBA,
        GLX_DEPTH_SIZE, 24,
        GLX_DOUBLEBUFFER,
        None
    };
    Colormap cmap;
    XSetWindowAttributes swa;

    tdsp.dpy = XOpenDisplay(0);
    if (tdsp.dpy == 0) {
        ngxLog(dev, "Can't open display");
        return -1;
    }

    root = DefaultRootWindow(tdsp.dpy);

    vi = glXChooseVisual(tdsp.dpy, 0, attr);

    if (vi == 0) {
        ngxLog(dev, "No visuals found");
        XCloseDisplay(tdsp.dpy);
        return -1;
    }

    ngxLog(dev, "Current visuals: %p", (void*) vi->visualid);

    cmap = XCreateColormap(tdsp.dpy, root, vi->visual, AllocNone);

    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask;

    tdsp.win = XCreateWindow(
            tdsp.dpy,
            root,
            0, 0,
            dev->width, dev->height,
            0,
            vi->depth,
            InputOutput,
            vi->visual,
            CWColormap | CWEventMask, &swa
            );

    XMapWindow(tdsp.dpy, tdsp.win);
    XStoreName(tdsp.dpy, tdsp.win, "NGXDevice");

    tdsp.WM_DELETE_WINDOW = XInternAtom(tdsp.dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(tdsp.dpy, tdsp.win, &tdsp.WM_DELETE_WINDOW, 1);

    tdsp.glc = glXCreateContext(tdsp.dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(tdsp.dpy, tdsp.win, tdsp.glc);

    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    memcpy(&dev->dsp, &tdsp, sizeof (struct ngx_xdata_t));
    return 0;
}

void ngxCloseWindow(NGXDEVICE dev) {
    ngxLog(dev, "=== CLEANUP ===");
    glXMakeCurrent(dev->dsp.dpy, None, 0);
    glXDestroyContext(dev->dsp.dpy, dev->dsp.glc);
    XDestroyWindow(dev->dsp.dpy, dev->dsp.win);
    XCloseDisplay(dev->dsp.dpy);
}

void SetPerspective(int enable, double fovY, double aspect, double zNear, double zFar) {
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;
    fH = tan(fovY / 360 * pi) * zNear;
    fW = fH * aspect;

    switch (enable) {
        case 0:
            fW *= 180.0;
            fH *= 180.0;
            glOrtho(-fW, fW, -fH, fH, zNear, zFar);
            break;
        case 1:
            glFrustum(-fW, fW, -fH, fH, zNear, zFar);
            break;
        case 2:
        {
            int iViewport[4];
            glGetIntegerv(GL_VIEWPORT, iViewport);
            glOrtho(iViewport[0], iViewport[0] + iViewport[2] / 2,
                    iViewport[1] + iViewport[3] / 2, iViewport[1], -1, 1);
            break;
        }
    }
}

void Scene(NGXDEVICE dev, double delta) {

    glViewport(0, 0, dev->width, dev->height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    SetPerspective(dev->persp, 45.0, dev->width / ((double) dev->height), 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    dev->drawfunc(delta, dev->ptr);

}

int ngxShutdown(NGXDEVICE dev) {
    if (dev != 0) {
        dev->state = NGX_CLOS;
        return 0;
    }
    return -1;
}

int ngxIsKeyDown(const NGXDEVICE dev, int key) {
    return (dev->keys[key % USHRT_MAX] != 0);
}

double timed = 0.0;

int ngxUpdate(NGXDEVICE dev) {
    int result = 0;
    double now = 0.0;

    switch (dev->state) {
        case NGX_INIT:
            ngxLog(dev, "=== INIT ===");
            result = ngxOpenWindow(dev);
            if (result == 0) {
                dev->loadfunc(1, dev->ptr);
                dev->state = NGX_LOOP;
                timed = ngxNow(dev);
                return 1;
            } else {
                ngxLog(dev, "Failed to open display with code %d", result);
                return 0;
            }
        case NGX_LOOP:
            ++dev->counter;

            result = XPending(dev->dsp.dpy);
            while (result-- > 0) {
                XEvent evt;
                XNextEvent(dev->dsp.dpy, &evt);
                switch (evt.type) {
                    case Expose:
                        goto RENDER;
                    case ClientMessage:
                        dev->state = NGX_CLOS;
                        break;
                    case KeyPress:
                    {
                        int key = XLookupKeysym(&evt.xkey, ((evt.xkey.state & ShiftMask) != 0));
                        if (key < USHRT_MAX) {
                            dev->keys[key] = 1;
                            dev->keyfunc(key, dev->ptr);
                        } else {
                            ngxLog(dev, "Bad code: %X", key);
                        }
                        break;
                    }
                    case KeyRelease:
                    {
                        int key = XLookupKeysym(&evt.xkey, ((evt.xkey.state & ShiftMask) != 0));
                        if (key < USHRT_MAX) {
                            dev->keys[key] = 0;
                        } else {
                            ngxLog(dev, "Bad code: %d", key);
                        }
                        break;
                    }
                    case ConfigureNotify:
                    {
                        if ((evt.xconfigure.width != dev->width) && (evt.xconfigure.height != dev->height)) {
                            dev->width = evt.xconfigure.width;
                            dev->height = evt.xconfigure.height;
                            goto RENDER;
                        }
                        break;
                    }
                    default:
                        ngxLog(dev, "Event%X", evt.type);
                }
            }

RENDER:

            now = ngxNow(dev);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Scene(dev, now - timed);
            glXSwapBuffers(dev->dsp.dpy, dev->dsp.win);
            timed = now;

            return 1;

        case NGX_CLOS:
            dev->loadfunc(0, dev->ptr);
            ngxCloseWindow(dev);
            return 0;
        default:
            return 0;
    }

}

int ngxDrawFunc(NGXDEVICE dev, NGXDRAWFUNC func) {
    if (dev == 0) {
        return -1;
    }

    if (func == 0) {
        dev->drawfunc = ngxDefaultDraw;
        return 0;
    }
    dev->drawfunc = func;
    return 0;
}

int ngxKeyFunc(NGXDEVICE dev, NGXKEYFUNC func) {
    if (dev == 0) {
        return -1;
    }

    if (func == 0) {
        dev->keyfunc = ngxDefaultKey;
        return 0;
    }

    dev->keyfunc = func;
    return 0;
}

int ngxLoadResourcesFunc(NGXDEVICE dev, NGXLOADRESOURCEFUNC func) {
    if (dev == 0) {
        return -1;
    }

    if (func == 0) {
        dev->loadfunc = ngxDefaultLoad;
        return 0;
    }

    dev->loadfunc = func;
    return 0;
}

int ngxPointer(NGXDEVICE dev, void* ptr) {
    if (dev == 0) {
        return -1;
    }

    dev->ptr = ptr;
    return 0;
}

int ngxSetPerspective(NGXDEVICE dev, int enable) {
    if (dev != 0) {
        dev->persp = enable;
        return 0;
    }
    return -1;
}

int ngxSetResolution(NGXDEVICE dev, int width, int height) {
    if (dev != 0) {
        dev->width = width;
        dev->height = height;
        return 0;
    }
    return -1;
}