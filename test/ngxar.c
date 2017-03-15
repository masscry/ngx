#include <data.h>
#include <index.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

void* load(const char* fname, uint32_t* plen){
  FILE* input = fopen(fname, "r");
  int32_t len = 0;
  void* result = 0;

  if (input == 0){
    return 0;
  }

  if (fseek(input, 0, SEEK_END) != 0) {
    fclose(input);
    return 0;
  }

  len = ftell(input);

  if (len < 0){
    fclose(input);
    return 0;
  }

  result = malloc(len);
  if (result == 0){
    fclose(input);
    return 0;
  }

  if (fseek(input, 0, SEEK_SET) != 0) {
    fclose(input);
    free(result);
    return 0;
  }

  if (fread(result, 1, len, input) != len) {
    fclose(input);
    free(result);
    return 0;
  }

  fclose(input);
  *plen = len;
  return result;
}

int main(int argc, char* argv[]){

  if (argc <= 2){
    goto USAGE;
  }

  NGXARC arc = ngxArcInit(argv[1], 0);
  if (arc == 0){
    goto USAGE;
  }

  NGXINDEX index = ngxIndexInit();
  if (index == 0){
    ngxArcCleanup(&arc);
    goto USAGE;
  }

  uint16_t iblk = ngxIndexSave(arc, index, 0);

  for (int i = 2; i < argc; ++i){
    fprintf(stderr, "+ %s ", argv[i]);
    void* data = 0;
    uint32_t len = 0;
    data = load(argv[i], &len);
    if (data != 0) {
      uint16_t dblk = ngxArcDataPut(arc, data, len, 0xFFFF);
      if (dblk == 0xFFFF){
        fprintf(stderr, "ERR\n");
        free(data);
        continue;
      }
      ngxIndexInsert(index, argv[i], dblk);
      fprintf(stderr, "OK\n");
    } else {
      fprintf(stderr, "ERR\n");
    }
    free(data);
  }
  ngxIndexSave(arc, index, 0);
  return 0;

USAGE:
  printf("Usage: %s ARCHIVE FILES...", argv[0]);
  return -1;
}
