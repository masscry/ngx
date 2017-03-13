/**
 * @file conftree.h
 * @author timur
 *
 * Simple multilevel key-value storage.
 *
 */

#ifndef __CONFIG_TREE_HEADER__
#define __CONFIG_TREE_HEADER__

enum conftype_t {
  CT_UND = 0,
  CT_NUM = 1,
  CT_STR = 2,
  CT_BLK = 3,
  CT_TOT
};

typedef struct config_tree_t* NGXCTREE;

typedef struct config_pair_t* NGXCTPAIR;

/**
 * Create new configuration tree.
 */
NGXCTREE ngxCTreeInit();

/**
 * Delete configuration tree.
 */
void ngxCTreeCleanup(NGXCTREE* ctree);

/**
 * Create new config tree kv-pair as number.
 */
NGXCTPAIR ngxCTPairNumber(const char* key, double value);

/**
 * Create new config tree kv-pair as string.
 */
NGXCTPAIR ngxCTPairString(const char* key, const char* value);

/**
 * Create new config tree kv-pair as block.
 */
NGXCTPAIR ngxCTPairBlock(const char* key);

/**
 * Remove kv-pair.
 */
void ngxCTPairCleanup(NGXCTPAIR* pair);

/**
 * Get number from pair.
 */
double ngxCTPairAsNumber(const NGXCTPAIR pair);



/**
 * Add kv-pair to the tree.
 */
int ngxCTreeAppend(NGXCTREE ctree, NGXCTPAIR pair);



#endif /* __CONFIG_TREE_HEADER__ */
