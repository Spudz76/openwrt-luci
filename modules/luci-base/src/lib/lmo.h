/*
 * lmo - Lua Machine Objects - General header
 *
 *   Copyright (C) 2009-2012 Jo-Philipp Wich <jow@openwrt.org>
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef _TEMPLATE_LMO_H_
#define _TEMPLATE_LMO_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fnmatch.h>
#include <dirent.h>
#include <ctype.h>
#include <limits.h>

#if (defined(__GNUC__) && defined(__i386__))
#define sfh_get16(d) (*((const uint16_t *) (d)))
#else
#define sfh_get16(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
					   +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

#ifndef __hidden
#define __hidden __attribute__((visibility("hidden")))
#endif


struct lmo_entry {
	uint32_t key_id;
	uint32_t val_id;
	uint32_t offset;
	uint32_t length;
} __attribute__((packed));

typedef struct lmo_entry lmo_entry_t;


struct lmo_archive {
	int         fd;
	int	        length;
	uint32_t    size;
	lmo_entry_t *index;
	char        *mmap;
	char		*end;
	struct lmo_archive *next;
};

typedef struct lmo_archive lmo_archive_t;


struct lmo_catalog {
	char lang[6];
	struct lmo_archive *archives;
	struct lmo_catalog *next;
};

typedef struct lmo_catalog lmo_catalog_t;

typedef void (*lmo_iterate_cb_t)(uint32_t, const char *, int, void *);

__hidden uint32_t sfh_hash(const char *data, size_t len, uint32_t init);
__hidden uint32_t lmo_canon_hash(const char *data, int len,
                                 const char *ctx, int ctxlen, int plural);

__hidden lmo_archive_t * lmo_open(const char *file);
__hidden void lmo_close(lmo_archive_t *ar);


__hidden extern lmo_catalog_t *_lmo_catalogs;
__hidden extern lmo_catalog_t *_lmo_active_catalog;

__hidden int lmo_load_catalog(const char *lang, const char *dir);
__hidden int lmo_change_catalog(const char *lang);
__hidden int lmo_translate(const char *key, int keylen, char **out, int *outlen);
__hidden int lmo_translate_ctxt(const char *key, int keylen,
                                const char *ctx, int ctxlen, char **out, int *outlen);
__hidden int lmo_translate_plural(int n, const char *skey, int skeylen,
                                  const char *pkey, int pkeylen,
                                  char **out, int *outlen);
__hidden int lmo_translate_plural_ctxt(int n, const char *skey, int skeylen,
                                       const char *pkey, int pkeylen,
                                       const char *ctx, int ctxlen,
                                     char **out, int *outlen);
__hidden void lmo_iterate(lmo_iterate_cb_t cb, void *priv);
__hidden void lmo_close_catalog(const char *lang);

#endif
