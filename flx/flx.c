#include <device.h>
#include <model.h>
#include <stdio.h>
#include <stdlib.h>
#include <data.h>
#include <string.h>

#include "cell.h"
#include "font.h"
#include "junks.h"

NGXDEVICE dev = 0;

typedef struct object_t {
    float x;
    float y;
    int f;
    float timeout;
} object_t;

object_t player;

ngx_sprite_t spr;
ngx_sprite_t fontSpr;

unsigned int grass = 0;
unsigned int ptex = 0;
unsigned int font = 0;

const char* text = "FLX v.0.0.1 by masscry";

void Render(double dt, void* ptr) {

    ngxSetupTexture(grass);
    for (int y = 0; y < 60; ++y) {
        for (int x = 0; x < 80; ++x) {
            ngxQuad(x * 8.0, y * 8.0, 8.0, 8.0);
        }
    }

    ngxSetupTexture(font);
    int tlen = strlen(text);
    for (int i = 0; i < tlen; ++i) {
        ngxSprite(i * 8.0, 0.0, 8.0, 8.0, &fontSpr, text[i]);
    }

    int loop = ngxIsKeyDown(dev, NGX_RIGHT) || ngxIsKeyDown(dev, NGX_LEFT)
            || ngxIsKeyDown(dev, NGX_DOWN) || ngxIsKeyDown(dev, NGX_UP);

    int croch = ngxIsKeyDown(dev, NGX_CTRL);

    int run = ngxIsKeyDown(dev, NGX_SHIFT);

    ngxSetupTexture(ptex);
    ngxSprite(player.x, player.y, 32, 32, &spr, player.f);

    player.timeout += dt;

    if (croch) {
        player.f = 3;
    } else {

        player.x += (ngxIsKeyDown(dev, NGX_RIGHT) - ngxIsKeyDown(dev, NGX_LEFT)) * dt * (100.0 + 100.0 * run);
        player.y += (ngxIsKeyDown(dev, NGX_DOWN) - ngxIsKeyDown(dev, NGX_UP)) * dt * (100.0 + 100.0 * run);

        if (player.x > 640 - 32) {
            player.x = 640 - 32;
        }

        if (player.x < 0) {
            player.x = 0;
        }

        if (player.y > 480 - 32) {
            player.y = 480 - 32;
        }

        if (player.y < 0) {
            player.y = 0;
        }

        if (loop) {
            if (player.timeout > 0.1 - 0.05 * run) {
                player.f = !player.f;
                player.timeout = 0.0;
            }
        } else {
            player.f = 2;
        }
    }
}

void Keys(int key, void* ptr) {
    NGXDEVICE dev = (NGXDEVICE) ptr;
    switch (key) {
        case NGX_ESC:
            ngxShutdown(dev);
            break;
            //        default:
            //            printf("key: %d\n", key);
    }
}

void Load(int load, void* ptr) {
    if (load != 0) {
        spr.sx = 4;
        spr.sy = 4;

        fontSpr.sx = 16;
        fontSpr.sy = 16;

        player.x = 320 - 16;
        player.y = 240 - 16;

        ptex = ngxTexture(flJunksWidth(), flJunksHeight(), flJunksPixels());
        grass = ngxTexture(flCellWidth(), flCellHeight(), flCellPixels());
        font = ngxTexture(flFontWidth(), flFontHeight(), flFontPixels());
    } else {
        ngxDeleteTexture(font);
        ngxDeleteTexture(ptex);
        ngxDeleteTexture(grass);
    }
}

int main(int argc, char* argv[]) {
    dev = ngxInit();

    if (dev == 0) {
        return -1;
    }

    ngxDrawFunc(dev, Render);
    ngxKeyFunc(dev, Keys);
    ngxLoadResourcesFunc(dev, Load);
    ngxPointer(dev, dev);
    ngxSetPerspective(dev, 2);
    ngxSetResolution(dev, 1280, 960);

    while (ngxUpdate(dev) != 0) {
        ;
    }

    ngxCleanup(&dev);
    return 0;
}
