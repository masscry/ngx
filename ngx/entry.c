#include <device.h>
#include <model.h>
#include <stdio.h>
#include <stdlib.h>
#include <data.h>

NGXMODEL mdl = 0;

void Render(double dt) {
  ngxModelDraw(mdl);
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

  while (ngxUpdate(dev) != 0){
    ;
  }

  free(mdl);
  ngxCleanup(&dev);
  return 0;
}
