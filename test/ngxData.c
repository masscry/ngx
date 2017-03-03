#include <data.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define EXPECT(cond)\
  if (!(cond)){\
    fprintf(stderr, "Error: %s at %s:%d", #cond, __FILE__, __LINE__);\
    exit(-1);\
  }

int main(int argc, char* argv[]){
  NGXARC arc = ngxArcInit("test.ngx");
  EXPECT(arc != 0);
  ngxArcCleanup(&arc);
  return 0;
}
