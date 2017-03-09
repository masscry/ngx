#include <stdint.h>
#include <stddef.h>
#include <data.h>

#ifndef __INDEX_HEADER__
#define __INDEX_HEADER__

typedef struct ngx_index_t* NGXINDEX;

typedef struct ngx_entry_t* NGXENTRY;

NGXINDEX ngxIndexInit();

void ngxIndexCleanup(NGXINDEX* pindex);

NGXENTRY ngxIndexInsert(NGXINDEX index, const char* key, uint16_t value);

uint16_t ngxIndexSize(const NGXINDEX index);

uint16_t ngxIndexCap(const NGXINDEX index);

int ngxIndexRemove(NGXINDEX index, NGXENTRY entry);

int ngxIndexErase(NGXINDEX index, const char* key);

NGXENTRY ngxIndexFind(const NGXINDEX index, const char* key);

const char* ngxEntryKey(const NGXENTRY entry);

uint16_t ngxEntryValue(const NGXENTRY entry);

uint16_t ngxIndexSave(NGXARC arc, const NGXINDEX index, uint16_t loc);

NGXINDEX ngxIndexLoad(NGXARC arc, uint16_t head);

#endif /* __INDEX_HEADER__  */
