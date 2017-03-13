#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <conftree.h>

#define EXPECT(cond) \
  if (!(cond)){\
    fprintf(stderr, "ERROR: %s at %s:%d\n", #cond, __FILE__, __LINE__);\
    exit(-1);\
  } else {\
    printf("EXPECTED: %s at %s:%d\n", #cond, __FILE__, __LINE__);\
  }

int main(int argc, char* argv[]){
  NGXCTREE tree = ngxCTreeInit();
  EXPECT(tree != 0);
  ngxCTreeCleanup(&tree);
  EXPECT(tree == 0);

  return 0;
}
