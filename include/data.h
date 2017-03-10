/**
 * @file data.h
 * @author timur
 *
 * Resources archive implementation.
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#ifndef __DATA_HEADER__
#define __DATA_HEADER__

typedef struct ngx_ablock_t* NGXBLK;

typedef struct ngx_archive_t* NGXARC;

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
 * Get data from file.
 */
void* ngxDataGet(FILE* file, uint32_t* datalen);

#endif /* __DATA_HEADER__ */
