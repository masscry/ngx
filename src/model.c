#include <device.h>
#include <model.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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
  if (mdl == 0){
    return;
  }

  glBegin(GL_TRIANGLES);
  for (int i = 0; i < mdl->vcnt; ++i){
    glColor3fv(mdl->vdt[i].n);
    glVertex3fv(mdl->vdt[i].v);
  }
  glEnd();
}
