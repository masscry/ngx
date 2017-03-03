#include <data.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

struct ngx_resource_t {
  void* data;
  int length;
};

struct ngx_archive_t {
  FILE* fl;
  uint16_t blkoff;
  uint16_t blksz;
  uint16_t blkcnt;
  uint16_t index;
};

#define NGXMAGIC (0x5677ABCD)
#define NGXVERSION (0x1)
#define NGXBLKSIZE (512)

#pragma pack(push, 1)

struct ngx_block_t {
  uint16_t blkid; /**< Block id */
  uint16_t blknxt; /**< Next block */
  uint8_t blkdata[]; /**< Block data */
};

struct ngx_ientry_t {
  char name[30];
  uint16_t blkid;
};

struct ngx_index_t {
  uint16_t entcnt; /**< Entry count */
  struct ngx_ientry_t ents[];
};

struct ngx_file_t {
  uint32_t magic; /**< archive file magic */
  uint32_t version; /**< archive version */
  uint32_t blkoff; /**< block offset from start */
  uint16_t blksz; /**< block size */
  uint16_t blkcnt; /**< total block count */
  uint16_t index; /**< start index block */
};

#pragma pack(pop)

/*
 * NGX resource archive structure.
 *
 * [0000-0015] ngx_file_t -- archive header information
 * [0016-????] ngx_block_t -- block headers
 *
 */

static FILE* ngxOpen(const char* filename){
  FILE* fl = fopen(filename, "r+");
  if (fl == 0){
    fl = fopen(filename, "w");
    if (fl == 0){ // Can't create new file
      return 0;
    }
    fclose(fl);
    fl = fopen(filename, "r+");
  }
  if (fl == 0){ // Still can't open file
    return 0;
  }
  return fl;
}

static int ngxGetHeader(FILE* fl, struct ngx_file_t* pheader){

  // Seek header
  if (fseek(fl, 0, SEEK_SET) != 0){
    return -1;
  }
  // Try to read header
  if (fread(pheader, sizeof(struct ngx_file_t), 1, fl) == 0 ){
    clearerr(fl);
  }
  // If not present
  if (fseek(fl, 0, SEEK_SET) != 0){
    return -1;
  }

  // Generate new one
  pheader->magic = NGXMAGIC;
  pheader->version = NGXVERSION;
  pheader->blkoff = sizeof(struct ngx_file_t);
  pheader->blksz = NGXBLKSIZE;
  pheader->blkcnt = 0;
  pheader->index = 0;

  // Update file
  if (fwrite(pheader, sizeof(struct ngx_file_t), 1, fl)  != 1){
    return -1;
  }

  fflush(fl);
  return 0;
}

NGXARC ngxArcInit(const char* filename){
  NGXARC result = (NGXARC)malloc(sizeof(struct ngx_archive_t));
  FILE* fl = 0;
  struct ngx_file_t fheader;
  struct ngx_block_t* blkind;

  int offset = 0;

  if (result == 0){
    return 0;
  }

  fl = ngxOpen(filename);
  if (fl == 0){ // Can't open file
    goto FREE_RESULT;
  }

  if (ngxGetHeader(fl, &fheader) != 0){
    goto FREE_RESULT;
  }

  if (fheader.magic != NGXMAGIC){
    goto FREE_RESULT;
  }

  if (fheader.version != NGXVERSION){
    goto FREE_RESULT;
  }

  result->fl = fl;
  result->blkoff = fheader.blkoff;
  result->blksz = fheader.blksz;
  result->blkcnt = fheader.blkcnt;
  result->index = fheader.index;
  return result;

FREE_RESULT:
  free(result);
  return 0;
}

void ngxArcCleanup(NGXARC* oarc){
  if ((oarc != 0)&&(*oarc != 0)){
    fclose((*oarc)->fl);
    free(*oarc);
    *oarc = 0;
  }
}
