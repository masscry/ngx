#include <conftree.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct config_pair_t config_pair_t;

typedef struct conf_number_t {
  int type;
  double val;
} conf_number_t;

typedef struct conf_string_t {
  int type;
  char* val;
} conf_string_t;

typedef struct conf_block_t {
  int type;
  config_pair_t* val;
} conf_block_t;

typedef union config_value_t {
  int type;
  conf_number_t num;
  conf_string_t str;
  conf_block_t blk;
} config_value_t;

struct config_pair_t {
  char* key;
  config_value_t val;
  config_pair_t* l;
  config_pair_t* r;
  int c;
};

struct config_tree_t {
  config_pair_t root;
};

void ngxCTreePairInit(config_pair_t* pr) {
  pr->key = 0;
  pr->val.type = CT_UND;
  pr->val.num.val = 0.0;
  pr->l = 0;
  pr->r = 0;
  pr->c = 0;
}

NGXCTREE ngxCTreeInit() {
  NGXCTREE result = malloc(sizeof(struct config_tree_t));
  if (result == 0){
    return 0;
  }
  ngxCTreePairInit(&(result->root));
  return result;
}

void ngxCTreeCleanup(NGXCTREE* ctree) {
  if ((ctree != 0) && (*ctree != 0)) {
    free(*ctree);
    *ctree = 0;
  }
}
