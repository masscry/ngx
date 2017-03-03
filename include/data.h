/**
 * @file data.h
 * @author timur
 *
 * Resources archive implementation.
 *
 */

#ifndef __DATA_HEADER__
#define __DATA_HEADER__

typedef struct ngx_resource_t* NGXRES;

typedef struct ngx_archive_t* NGXARC;

/**
 * Create new archive.
 */
NGXARC ngxArcInit(const char* filename);

/**
 * Cleanup archive.
 */
void ngxArcCleanup(NGXARC* oarc);

/**
 * Save archive.
 */
int ngxArcSave(const char* filename);

/**
 * Create new resource.
 */
NGXRES ngxResInit();

/**
 * Cleanup resource.
 */
void ngxResCleanup(NGXRES* ores);

/**
 * Get resource copy with defined name.
 */
NGXRES ngxArcGet(const char* name);

/**
 * Put resource copy to archive with name.
 */
int ngxArcPut(const char* name, const NGXRES res);

/**
 * Get resource data.
 */
void* ngxResData(const NGXRES res);

/**
 * Get resource size.
 */
int ngxResSize(const NGXRES res);

#endif /* __DATA_HEADER__ */
