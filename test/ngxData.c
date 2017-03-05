#include <data.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define EXPECT(cond)\
  if (!(cond)){\
    fprintf(stderr, "Error: %s at %s:%d\n", #cond, __FILE__, __LINE__);\
    exit(-1);\
  }

void test000(const char* fname){
  NGXARC arc = 0;
  NGXBLK block = 0;
  NGXBLK nblk = 0;

  arc = ngxArcInit(fname, 0);

  EXPECT(arc != 0);

  EXPECT(ngxArcBlock(arc, 0xFFFF) == 0); // Invalid block id

  block = ngxArcBlock(arc, 0);
  EXPECT(block != 0);

  EXPECT(ngxBlockID(0) == 0x0); // no segfault
  EXPECT(ngxBlockNextID(0) == 0x0); // no segfault

  EXPECT(ngxBlockID(block) == 0);

  nblk = ngxArcBlock(arc, 1);
  EXPECT(nblk != 0);

  EXPECT(ngxBlockID(nblk) == 1);
  EXPECT(ngxBlockNextID(nblk) == 0xFFFF);

  EXPECT(ngxBlockSetNextID(block, 1) == 0);
  EXPECT(ngxBlockNextID(block) == 1);

  EXPECT(ngxArcUpdateBlock(arc, block) == 0);

  ngxBlockCleanup(&nblk);
  ngxBlockCleanup(&block);
  EXPECT(nblk == 0);
  EXPECT(block == 0);

  ngxArcCleanup(&arc);
  EXPECT(arc == 0);
}

#define DLEN (300)

void test001(const char* fname){
  NGXARC arc = 0;
  uint16_t blkid = 0xFFFF;
  int* buf = 0;
  uint32_t len = 0;
  int* ibuf = malloc(sizeof(int)*DLEN);

  for (int i = 0; i < DLEN; ++i){
    ibuf[i] = rand();
  }

  arc = ngxArcInit(fname, 0);
  blkid = ngxArcDataPut(arc, ibuf, DLEN*sizeof(int));
  EXPECT(blkid != 0xFFFF);

  buf = (int*) ngxArcDataGet(arc, blkid, &len);
  EXPECT(buf != 0);

  EXPECT(len == DLEN*sizeof(int));
  EXPECT(memcmp(buf, ibuf, len) == 0);

  free(buf);
  free(ibuf);
  ngxArcCleanup(&arc);
}

int main(int argc, char* argv[]){

  if (argc != 2){
    fprintf(stderr, "Usage: %s FILENAME\n", argv[0]);
    return -1;
  }

  srand(time(0));
  test000(argv[1]);
  test001(argv[1]);
}

