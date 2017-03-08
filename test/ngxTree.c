#include <data.h>
#include <index.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>


#define EXPECT(cond)\
  if (!(cond)){\
    fprintf(stderr, "Error: %s at %s:%d\n", #cond, __FILE__, __LINE__);\
    exit(-1);\
  } else {\
    fprintf(stderr, "EXPECTED: %s at %s:%d\n", #cond, __FILE__, __LINE__);\
  }

void test000(){
  NGXINDEX index = ngxIndexInit();
  EXPECT(index != 0);

  EXPECT(ngxIndexSize(0) == 0);
  EXPECT(ngxIndexCap(0) == 0);

  EXPECT(ngxIndexSize(index) == 0);
  EXPECT(ngxIndexCap(index) == 33);

  EXPECT(ngxIndexFind(0, "abc") == 0);
  EXPECT(ngxIndexFind(index, "abc") == 0);
  EXPECT(ngxIndexInsert(index, "abc", 0) != 0);
  EXPECT(ngxIndexFind(0, "abc") == 0);
  EXPECT(ngxIndexSize(index) == 1);

  EXPECT(ngxIndexInsert(index, "abc", 1) == 0);
  EXPECT(ngxIndexSize(index) == 1);

  EXPECT(ngxIndexFind(index, "abc") != 0);

  EXPECT(ngxIndexErase(index, "abc") == 0);
  EXPECT(ngxIndexSize(index) == 0);

  EXPECT(ngxIndexFind(index, "abc") == 0);

  EXPECT(ngxIndexRemove(index, "abc") != 0);

  ngxIndexCleanup(&index);
  EXPECT(index == 0);
}

void test001(const char* fname){
  NGXARC arc = ngxArcInit(fname, 0);
  NGXINDEX index = ngxIndexInit();
  NGXINDEX nindex = 0;
  uint16_t head = 0xFFFF;

  ngxIndexInsert(index, "abc", 1);
  ngxIndexInsert(index, "bac", 2);
  ngxIndexInsert(index, "bca", 3);

  head = ngxIndexSave(arc, index);

  EXPECT(head != 0xFFFF);

  nindex = ngxIndexLoad(arc, head);

  EXPECT(nindex != 0);

}

int main(int argc, char* argv[]){

  if (argc != 2){
    fprintf(stderr, "Usage: %s FILENAME\n", argv[0]);
    return -1;
  }

  test000();
  test001(argv[1]);
}

