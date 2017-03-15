#include <data.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __DATA_DEBUG__
#define DLOG(fmt, ...) fprintf(stderr, fmt "\n",  ##__VA_ARGS__)
#else
#define DLOG(...)
#endif

struct ngx_archive_t {
  FILE* fl;
  uint16_t blkoff;
  uint16_t blksz;
  uint16_t blkcnt;
  int ronly;
};

struct ngx_ablock_t {
  uint16_t blkid;
  uint16_t blknxt;
  uint8_t* blkdata;
};

#define NGXMAGIC (0x5677ABCD)
#define NGXVERSION (0x1)
#define NGXBLKSIZE (512)

#pragma pack(push, 1)

struct ngx_block_t {
  uint16_t blknxt; /**< Next block */
  uint8_t blkdata[]; /**< Block data */
};

struct ngx_file_t {
  uint32_t magic; /**< archive file magic */
  uint32_t version; /**< archive version */
  uint32_t blkoff; /**< block offset from start */
  uint16_t blksz; /**< block size */
  uint16_t blkcnt; /**< total block count */
};

#pragma pack(pop)

/*
 * NGX resource archive structure.
 *
 * [0000-0015] ngx_file_t -- archive header information
 * [0016-????] ngx_block_t -- block headers
 *
 */

static FILE* ngxOpen(const char* filename, int readonly){
  if (readonly == 0){
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

  // readonly
  FILE* fl = fopen(filename, "r");
  if (fl == 0){
    return 0;
  }
  return fl;
}

static int ngxGetHeader(FILE* fl, struct ngx_file_t* pheader, int readonly){

  // Seek header
  if (fseek(fl, 0, SEEK_SET) != 0){
    return -1;
  }
  // Try to read header
  if (fread(pheader, sizeof(struct ngx_file_t), 1, fl) != 1 ){

    if (readonly != 0){
      return -1;
    }

    clearerr(fl);
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

    // Update file
    if (fwrite(pheader, sizeof(struct ngx_file_t), 1, fl)  != 1){
      return -1;
    }

    fflush(fl);
  }
  return 0;
}

uint16_t ngxArcBlockSize(const NGXARC arc){
  if (arc != 0) {
    return arc->blksz;
  }
  return 0;
}

uint16_t ngxArcBlockCount(const NGXARC arc){
  if (arc != 0) {
    return arc->blkcnt;
  }
  return 0;
}

int ngxUpdateHeader(NGXARC arc, int readonly){
  if (readonly == 0){
    struct ngx_file_t fheader;
    fheader.magic = NGXMAGIC;
    fheader.version = NGXVERSION;
    fheader.blkoff = arc->blkoff;
    fheader.blksz = arc->blksz;
    fheader.blkcnt = arc->blkcnt;

    // Seek header
    if (fseek(arc->fl, 0, SEEK_SET) != 0){
      return -1;
    }

    // Update file
    if (fwrite(&fheader, sizeof(struct ngx_file_t), 1, arc->fl)  != 1){
      return -1;
    }

    fflush(arc->fl);
    return 0;
  }
  return -1;
}

#define MAX(A, B) ((A>B)?(A):(B))

NGXARC ngxArcInit(const char* filename, int readonly){
  NGXARC result = (NGXARC)malloc(sizeof(struct ngx_archive_t));
  FILE* fl = 0;
  struct ngx_file_t fheader;

  if (result == 0){
    return 0;
  }

  fl = ngxOpen(filename, readonly);
  if (fl == 0){ // Can't open file
    goto FREE_RESULT;
  }

  if (ngxGetHeader(fl, &fheader, readonly) != 0){
    goto FREE_RESULT;
  }

  if (fheader.magic != NGXMAGIC){
    goto FREE_RESULT;
  }

  if (fheader.version != NGXVERSION){
    goto FREE_RESULT;
  }

  if (fheader.blksz != NGXBLKSIZE ) {
    goto FREE_RESULT;
  }

  result->fl = fl;
  result->blkoff = fheader.blkoff;
  result->blksz = fheader.blksz;
  result->blkcnt = fheader.blkcnt;
  result->ronly = readonly;
  return result;

FREE_RESULT:
  free(result);
  if (fl != 0){
    fclose(fl);
  }
  return 0;
}

void ngxArcCleanup(NGXARC* oarc){
  if ((oarc != 0)&&(*oarc != 0)){
    fclose((*oarc)->fl);
    free(*oarc);
    *oarc = 0;
  }
}

int ngxGetBlock(NGXARC arc, uint16_t blkid, NGXBLK blk, int readonly){
  int offset = arc->blkoff + arc->blksz*blkid;
  struct ngx_block_t* fblk = 0;

  fblk = (struct ngx_block_t*) malloc(arc->blksz);
  if (fblk == 0){
    return -1;
  }

  // Seek block
  if (fseek(arc->fl, offset, SEEK_SET) != 0){
    free(fblk);
    return -1;
  }

  // Try to read block
  if (fread(fblk, arc->blksz, 1, arc->fl) != 1 ){
    if (readonly != 0){
      free(fblk);
      return -1;
    }

    clearerr(arc->fl);

    // If not present
    if (fseek(arc->fl, offset, SEEK_SET) != 0){
      free(fblk);
      return -1;
    }

    // Generate new one
    fblk->blknxt = 0xFFFF;
    memset(fblk->blkdata, 0, arc->blksz - sizeof(struct ngx_block_t));

    // Update file
    if (fwrite(fblk, arc->blksz, 1, arc->fl)  != 1){
      free(fblk);
      return -1;
    }

    fflush(arc->fl);
  }

  blk->blkdata = (uint8_t*) malloc(arc->blksz - sizeof(struct ngx_block_t));
  if (blk->blkdata == 0){
    free(fblk);
    return -1;
  }

  blk->blkid = blkid;
  blk->blknxt = fblk->blknxt;
  memcpy(blk->blkdata, fblk->blkdata, arc->blksz - sizeof(struct ngx_block_t));

  free(fblk);
  return 0;
}

NGXBLK ngxArcBlock(NGXARC arc, uint16_t blkid){
  NGXBLK blk = 0;
  int offset = 0;

  if (blkid == 0xFFFF){
    return 0;
  }

  blk = (NGXBLK)malloc(sizeof(struct ngx_ablock_t));
  if (blk == 0){
    return 0;
  }
  blk->blkdata = 0;

  if (ngxGetBlock(arc, blkid, blk, arc->ronly) != 0){
    goto FREE_BLOCK;
  }

  if (blk->blkid != blkid){
    goto FREE_BLOCK;
  }

  if (blk->blkid >= arc->blkcnt){
    arc->blkcnt = blk->blkid + 1;
    if (ngxUpdateHeader(arc, arc->ronly) != 0){
      goto FREE_BLOCK;
    }
  }

  return blk;

FREE_BLOCK:
  free(blk->blkdata);
  free(blk);
  return 0;
}

uint16_t ngxBlockID(const NGXBLK blk){
  if (blk == 0){
    return 0;
  }
  return blk->blkid;
}

uint16_t ngxBlockNextID(const NGXBLK blk){
  if (blk == 0){
    return 0;
  }
  return blk->blknxt;
}

void ngxBlockCleanup(NGXBLK* oblk){
  if ((oblk != 0) && (*oblk != 0)){
    free((*oblk)->blkdata);
    free(*oblk);
    *oblk = 0;
  }
}

int ngxBlockSetNextID(NGXBLK blk, uint16_t nid){
  if (blk == 0){
    return -1;
  }
  blk->blknxt = nid;
  return 0;
}

int ngxArcUpdateBlock(NGXARC arc, const NGXBLK blk){
  int offset = 0;
  if ((arc == 0) || (blk == 0)) {
    return -1;
  }

  if (arc->ronly != 0){
    return -1;
  }

  offset = arc->blkoff + arc->blksz*blk->blkid;

  clearerr(arc->fl);
  fflush(arc->fl);
  // Seek block
  if (fseek(arc->fl, offset, SEEK_SET) != 0){
    return -1;
  }

  // Write block header
  if (fwrite(&(blk->blknxt), sizeof(uint16_t), 1, arc->fl) != 1){
    return -1;
  }

  // Update data
  if (fwrite(blk->blkdata, arc->blksz - sizeof(uint16_t), 1, arc->fl)  != 1){
    return -1;
  }

  fflush(arc->fl);
  return 0;

}

uint16_t ngxArcDataPut(NGXARC arc, const void* data, uint32_t datalen, uint16_t blkid){
  NGXBLK pos = 0;
  uint8_t* cursor = (uint8_t*) data;
  uint16_t result = 0xFFFF;
  uint8_t* bcursor = 0;
  uint32_t ln = 0;

  if (arc->ronly != 0) {
    return 0xFFFF;
  }

  if (blkid == 0xFFFF){
    blkid = arc->blkcnt;
  }

  pos = ngxArcBlock(arc, blkid);
  if (pos == 0){
    return 0xFFFF;
  }
  result = ngxBlockID(pos);

  memcpy(pos->blkdata, &datalen, sizeof(uint32_t));
  bcursor = pos->blkdata + sizeof(uint32_t);
  ln = (datalen > (arc->blksz-sizeof(struct ngx_block_t)-sizeof(uint32_t)))?
                  (arc->blksz-sizeof(struct ngx_block_t)-sizeof(uint32_t)):datalen;
  while (datalen > 0){
    memcpy(bcursor, cursor, ln);
    datalen -= ln;
    cursor += ln;
    if (datalen > 0){
      NGXBLK next = 0;
      if (ngxBlockNextID(pos) == 0xFFFF){
        next = ngxArcBlock(arc, arc->blkcnt);
      } else {
        next = ngxArcBlock(arc, ngxBlockNextID(pos));
      }
      if (next == 0){
        ngxBlockCleanup(&pos);
        return 0xFFFF;
      }
      ngxBlockSetNextID(pos, ngxBlockID(next));
      if (ngxArcUpdateBlock(arc, pos) != 0){
        ngxBlockCleanup(&pos);
        ngxBlockCleanup(&next);
        return 0xFFFF;
      }
      ngxBlockCleanup(&pos);
      pos = next;
      bcursor = pos->blkdata;
    }
    ln = (datalen > (arc->blksz-sizeof(struct ngx_block_t)))?
                    (arc->blksz-sizeof(struct ngx_block_t)):datalen;
  }
  if (ngxArcUpdateBlock(arc, pos) != 0){
    ngxBlockCleanup(&pos);
    return 0xFFFF;
  }
  ngxBlockCleanup(&pos);
  return result;
}

void* ngxArcDataGet(NGXARC arc, uint16_t blkid, uint32_t* datalen){
  NGXBLK root = ngxArcBlock(arc, blkid);
  uint32_t totlen = 0;
  uint32_t left = 0;
  uint8_t* result = 0;
  uint8_t* cursor = 0;
  uint32_t ln = 0;

  if (root == 0){
    return 0;
  }

  memcpy(&totlen, root->blkdata, sizeof(uint32_t));
  result = (uint8_t*)malloc(totlen);
  if (result == 0) {
    ngxBlockCleanup(&root);
    return 0;
  }

  cursor = result;
  left = totlen;

  ln = (left > (arc->blksz - sizeof(struct ngx_block_t) - sizeof(uint32_t)))?
               (arc->blksz - sizeof(struct ngx_block_t) - sizeof(uint32_t)) : left;

  memcpy(cursor, root->blkdata + sizeof(uint32_t), ln);
  cursor += ln;
  left -= ln;
  while(left > 0){
    NGXBLK next = ngxArcBlock(arc, ngxBlockNextID(root));
    if (next == 0){
      ngxBlockCleanup(&root);
      free(result);
      return 0;
    }
    ngxBlockCleanup(&root);
    root = next;

    ln = (left > (arc->blksz - sizeof(struct ngx_block_t)))?(arc->blksz - sizeof(struct ngx_block_t)):left;
    memcpy(cursor, root->blkdata, ln);
    cursor += ln;
    left -= ln;
  }

  if (datalen != 0){
    *datalen = totlen;
  }

  ngxBlockCleanup(&root);
  return result;
}

void* ngxDataGet(FILE* file, uint32_t* datalen) {
  void* result = 0;
  int32_t dlen = 0;

  if (fseek(file, 0, SEEK_END) != 0){
    DLOG("%s", "Seek failed");
    return 0;
  }

  dlen = ftell(file);
  if (dlen < 0) {
    DLOG("%s", "Tell failed");
    return 0;
  }

  result = malloc(dlen);
  if (result == 0){
    DLOG("%s", "Malloc failed");
    return 0;
  }

  if (fseek(file, 0, SEEK_SET) != 0){
    DLOG("%s", "Seek-2 failed");
    free(result);
    return 0;
  }

  if (fread(result, dlen, 1, file) != 1){
    DLOG("%s", "Read failed");
    free(result);
    return 0;
  }

  if (datalen != 0){
    *datalen = dlen;
  }
  return result;
}
