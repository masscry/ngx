/**
 * @file data.h
 * @author timur
 *
 * Resources archive implementation.
 *
 */

#include <stdint.h>
#include <stddef.h>

#ifndef __DATA_HEADER__
#define __DATA_HEADER__

typedef struct ngx_ablock_t* NGXBLK;

typedef struct ngx_archive_t* NGXARC;

typedef struct ngx_index_t NGXINDEX;

/**
 * Create new archive.
 */
NGXARC ngxArcInit(const char* filename, int readonly);

/**
 * Cleanup archive.
 */
void ngxArcCleanup(NGXARC* oarc);

/**
 * Get archive block size.
 */
uint16_t ngxArcBlockSize(const NGXARC arc);

/**
 * Get archive block count.
 */
uint16_t ngxArcBlockCount(const NGXARC arc);

/**
 * Get block from archive.
 */
NGXBLK ngxArcBlock(NGXARC arc, uint16_t blkid);

/**
 * Update block in archive.
 */
int ngxArcUpdateBlock(NGXARC arc, const NGXBLK blk);

/**
 * Get block ID.
 */
uint16_t ngxBlockID(const NGXBLK blk);

/**
 * Get next block ID.
 */
uint16_t ngxBlockNextID(const NGXBLK blk);

/**
 * Set next block ID.
 */
int ngxBlockSetNextID(NGXBLK blk, uint16_t nid);

/**
 * Cleanup block data copy.
 */
void ngxBlockCleanup(NGXBLK* blk);

/**
 * Put data to archive.
 */
uint16_t ngxArcDataPut(NGXARC arc, const void* data, uint32_t datalen, uint16_t blkid);

/**
 * Get data from archive.
 */
void* ngxArcDataGet(NGXARC arc, uint16_t blkid, uint32_t* datalen);

/**
 * Create new index.
 */
NGXINDEX ngxIndexInit();

/**
 * Cleanup index.
 */
void ngxIndexCleanup(NGXINDEX* index);

/**
 * Insert new item into index.
 */
int ngxIndexInsert(NGXINDEX index, const char* fname, uint16_t blkid);

/**
 * Total index size.
 */
int ngxIndexCount(const NGXINDEX index);

/**
 * Total index length in bytes.
 */
uint32_t ngxIndexSize(const NGXINDEX index);

#endif /* __DATA_HEADER__ */
