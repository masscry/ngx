#include <device.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NGXLOGFNAME ("ngx.log")

struct ngx_device_t {
  uint64_t counter;
};

NGXDEVICE ngxInit(){
  NGXDEVICE dev = 0;
  time_t now = 0;
  struct tm nowtm = {};
  char buffer[256];
  FILE* log = 0;

  log = fopen(NGXLOGFNAME, "w");
  if (log == 0){
    fprintf(stderr, "Failed to open log file %s\n", NGXLOGFNAME);
    return 0;
  }

  now = time(0);
  localtime_r(&now, &nowtm);
  strftime(buffer, 256, "%d %b %Y %H:%M:%S", &nowtm);

  fprintf(log, "=== NGX ===\n");
  fprintf(log, "Time: %s\n", buffer);

  dev = malloc(sizeof(struct ngx_device_t));
  if (dev == 0){
    fprintf(log, "Failed to allocate memory for device\n");
    fclose(log);
    return 0;
  }

  dev->counter = 0;
  fclose(log);
  return dev;
}

void ngxCleanup(NGXDEVICE* pdev){
  if ((pdev != 0)&&( (*pdev) != 0)){
    free(*pdev);
    *pdev = 0;
  }
}

int ngxRun(NGXDEVICE dev){
  ++dev->counter;
  return 1;
}
