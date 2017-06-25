/**
 * @file model.h
 * @author timur
 *
 * Defines NMD format.
 *
 */

#ifndef __MODEL_HEADER__
#define __MODEL_HEADER__

typedef struct ngx_model_t* NGXMODEL;

void ngxLookAt(double eyeX, double eyeY, double eyeZ,
        double centerX, double centerY, double centerZ,
        double upX, double upY, double upZ);

void ngxModelDraw(const NGXMODEL mdl);

void ngxQuad(float x, float y, float w, float h);

void ngxRotate(float angle, float x, float y, float z);

void ngxTranslate(float x, float y, float z);

unsigned int ngxTexture(int width, int height, const unsigned char* pixels);

void ngxDeleteTexture(unsigned int tex);

void ngxSetupTexture(unsigned int tex);

typedef struct ngx_sprite_t {
    int sx;
    int sy;
} ngx_sprite_t;

void ngxSprite(float x, float y, float w, float h, const ngx_sprite_t* spr, int frame);

#endif /* __MODEL_HEADER__ */
