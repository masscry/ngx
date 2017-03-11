#include <device.h>
#include <model.h>
#include <stdio.h>
#include <stdlib.h>
#include <data.h>

NGXMODEL mdl = 0;

void Render(double dt, void* ptr) {
  ngxModelDraw(mdl);
}

int prs = 0;
void Keys(int key, void* ptr) {
  NGXDEVICE dev = (NGXDEVICE) ptr;
  ngxSetPerspective(dev, (prs = !prs));
}

int main(int argc, char* argv[]){
  NGXDEVICE dev = ngxInit();

  if (dev == 0){
    return -1;
  }

  if (argc == 2) {
    FILE* input = fopen(argv[1], "r");
    if (input == 0){
      ngxLog(dev, "Can't open model %s", argv[1]);
      ngxCleanup(&dev);
      return -1;
    }

    mdl = (NGXMODEL) ngxDataGet(input, 0);
    if (mdl == 0){
      ngxLog(dev, "Can't load model");
      ngxCleanup(&dev);
      return -1;
    }
    ngxDrawFunc(dev, Render);
  }
  ngxKeyFunc(dev, Keys);
  ngxPointer(dev, dev);

  while (ngxUpdate(dev) != 0){
    ;
  }

  free(mdl);
  ngxCleanup(&dev);
  return 0;
}
