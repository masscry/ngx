#include <device.h>
#include <model.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>

#pragma pack(push, 1)

struct ngx_vertex_t {
    float v[3];
    float t[2];
    float n[3];
};

struct ngx_model_t {
    uint16_t id;
    uint16_t vcnt;
    struct ngx_vertex_t vdt[];
};

#pragma pack(pop)

void ngxModelDraw(const NGXMODEL mdl) {
    if (mdl == 0) {
        return;
    }

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < mdl->vcnt; ++i) {
        glColor3f(fabsf(mdl->vdt[i].n[0]), fabsf(mdl->vdt[i].n[1]), fabsf(mdl->vdt[i].n[2]));
        //    glColor3f(0.5f, 0.5f, 0.5f);
        glVertex3fv(mdl->vdt[i].v);
    }
    glEnd();
}

void ngxQuad(float x, float y, float w, float h) {
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x, y, 0.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x + w, y, 0.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x + w, y + h, 0.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x + w, y + h, 0.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x, y + h, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x, y, 0.0f);
    glEnd();
}

void ngxSprite(float x, float y, float w, float h, const ngx_sprite_t* spr, int frame) {
    int px = 0;
    int py = 0;

    if (spr == 0){
        return;
    }
    
    ngx_sprite_t temp;
    temp.sx = (spr->sx == 0)?1:spr->sx;
    temp.sy = (spr->sy == 0)?1:spr->sy;
    
    px = (frame%(temp.sx*temp.sy))%temp.sx;
    py = (frame%(temp.sx*temp.sy))/temp.sx;
    
    double lx = 1.0/temp.sx;
    double ly = 1.0/temp.sy;
        
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(lx*px, ly*py);
    glVertex3f(x, y, 0.0f);

    glTexCoord2f(lx*(px+1), ly*py);
    glVertex3f(x + w, y, 0.0f);

    glTexCoord2f(lx*(px+1), ly*(py+1));
    glVertex3f(x + w, y + h, 0.0f);

    glTexCoord2f(lx*(px+1), ly*(py+1));
    glVertex3f(x + w, y + h, 0.0f);

    glTexCoord2f(lx*px, ly*(py+1));
    glVertex3f(x, y + h, 0.0f);

    glTexCoord2f(lx*px, ly*py);
    glVertex3f(x, y, 0.0f);
    glEnd();
}

void ngxLookAt(double eyeX, double eyeY, double eyeZ,
        double centerX, double centerY, double centerZ,
        double upX, double upY, double upZ) {
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
}

void ngxRotate(float angle, float x, float y, float z) {
    glRotatef(angle, x, y, z);
}

void ngxTranslate(float x, float y, float z) {
    glTranslatef(x, y, z);
}

unsigned int ngxTexture(int width, int height, const unsigned char* pixels) {
    unsigned int result = 0;
    glGenTextures(1, &result);
    glBindTexture(GL_TEXTURE_2D, result);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    return result;
}

void ngxDeleteTexture(unsigned int tex) {
    glDeleteTextures(1, &tex);
}

void ngxSetupTexture(unsigned int tex) {
    glBindTexture(GL_TEXTURE_2D, tex);
}

