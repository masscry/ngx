#include <index.h>

#include <stdlib.h>
#include <string.h>

uint32_t sdbm(const void* data, size_t datalen, uint32_t init) {
  uint8_t* cursor = (uint8_t*)data;
  uint32_t result = init;
  int c;

  while (datalen--> 0){
    c = *cursor++;
    result = c + (result << 6) + (result << 16) - result;
  }
  return result;
}

#define NGXINITENTRY (33)
#define NGXINITAPPND (NGXINITENTRY + 1)
#define NGXINITSTLEN (13)

#pragma pack(push, 1)

struct ngx_entry_t {
  uint16_t value; // 2
  char key[NGXINITSTLEN]; // 13
};

struct ngx_findex_t {
  uint16_t size; // 2
  uint16_t cap; // 2
  char reserved[11]; // 11
  struct ngx_entry_t entries[];
};

#pragma pack(pop)

struct ngx_index_t {
  uint16_t size;
  uint16_t cap;
  NGXENTRY entries;
};

NGXINDEX ngxIndexInit(){
  NGXINDEX index = malloc(sizeof(struct ngx_index_t));
  if (index == 0){
    return 0;
  }

  index->size = 0;
  index->cap = NGXINITENTRY;
  index->entries = (NGXENTRY)calloc(NGXINITENTRY, sizeof(struct ngx_entry_t));

  if (index->entries == 0){
    free(index);
    return 0;
  }

  for (int i = 0; i < NGXINITENTRY; ++i){
    index->entries[i].value = 0xFFFF;
  }

  return index;

}

void ngxIndexCleanup(NGXINDEX* pindex){
  if ((pindex != 0) && (*pindex != 0)) {
    free(*pindex);
    *pindex = 0;
  }
}

uint16_t ngxIndexSize(const NGXINDEX index){
  if (index == 0){
    return 0;
  }
  return index->size;
}

uint16_t ngxIndexCap(const NGXINDEX index){
  if (index == 0){
    return 0;
  }
  return index->cap;
}

NGXENTRY ngxIndexFind(const NGXINDEX index, const char* key) {
  uint16_t size = 0;

  if (index == 0){
    return 0;
  }

  if (strlen(key) >= NGXINITSTLEN ){
    return 0;
  }

  size = ngxIndexSize(index);

  for (int i = 0; i < size; ++i){
    if (strcmp(index->entries[i].key, key) == 0){
      return (index->entries + i);
    }
  }
  return 0;
}

NGXENTRY ngxIndexInsert(NGXINDEX index, const char* key, uint16_t value) {
  uint16_t size = 0;
  NGXENTRY result = 0;

  if (index == 0) {
    return 0;
  }

  if (strlen(key) >= NGXINITSTLEN ){ // Need to check, because can't separate cases
    return 0;
  }

  result = ngxIndexFind(index, key);
  if (result != 0){ // Here can be too long str, or not found
    return 0;
  }

  size = ngxIndexSize(index);
  if (size == ngxIndexCap(index)){
    uint16_t ncap = size + NGXINITAPPND;
    NGXENTRY nitems = (NGXENTRY)calloc(ncap, sizeof(struct ngx_entry_t));
    if (nitems == 0){
      return 0;
    }

    memcpy(nitems, index->entries, sizeof(struct ngx_entry_t)*size);

    for(int i = size; i < ncap; ++i){
      nitems[i].value = 0xFFFF;
    }
    free(index->entries);
    index->entries = nitems;
    index->cap = ncap;
  }
  result = index->entries + size;
  result->value = value;
  strcpy(result->key, key);
  index->size += 1;
  return result;
}

int ngxIndexRemove(NGXINDEX index, NGXENTRY entry) {
  int item = 0;

  if (index == 0){
    return -1;
  }

  if (entry == 0){
    return -1;
  }

  item = entry - index->entries;

  if (item >= ngxIndexSize(index)){
    return -1;
  }

  index->size-=1;

  entry->value = index->entries[index->size].value;
  memcpy(entry->key, index->entries[index->size].key, NGXINITSTLEN);

  index->entries[index->size].value = 0xFFFF;
  index->entries[index->size].key[0] = 0;
  return 0;
}

int ngxIndexErase(NGXINDEX index, const char* key) {
  NGXENTRY item = 0;

  if (index == 0){
    return -1;
  }

  if (key == 0){
    return -1;
  }

  item = ngxIndexFind(index, key);
  if (item == 0){
    return -1;
  }

  return ngxIndexRemove(index, item);
}
