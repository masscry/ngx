#include <device.h>

int main(int argc, char* argv[]){
  NGXDEVICE dev = ngxInit();
  if (dev == 0){
    return -1;
  }

  while (ngxUpdate(dev) != 0){
    ;
  }

  ngxCleanup(&dev);
  return 0;
}
