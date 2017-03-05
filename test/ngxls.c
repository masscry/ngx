#include <data.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

int main(int argc, char* argv[]){

  if (argc != 2){
    fprintf(stderr, "Usage: %s FILENAME\n", argv[0]);
    return -1;
  }

  NGXARC arc = ngxArcInit(argv[1], 1);
  if (arc == 0){
    fprintf(stderr, "Can't open %s\n", argv[1]);
    return -1;
  }

  uint16_t bsz = ngxArcBlockSize(arc);
  uint16_t bcn = ngxArcBlockCount(arc);

  printf("Block size: %u\n", bsz);
  printf("Block count: %u\n", bcn);

  ngxArcCleanup(&arc);
  return 0;
}

