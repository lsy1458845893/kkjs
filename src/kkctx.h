#ifndef KKCTX_H
#define KKCTX_H

#include "kkdep.h"

typedef void *(*kkctx_alloc_t)(void *udat, void *space, kksize_t size);
typedef void (*kkctx_cleanup_t)(void *udat);

typedef struct kkctx_s {
  void *udat;
  kkctx_alloc_t alloc;
  kkctx_cleanup_t cleanup;
  kkjbuf_t *jbuf;
} kkctx_t;

static inline void kkctx_init(kkctx_t *c, void *udat, kkctx_alloc_t alloc, kkctx_cleanup_t cleanup) {
  c->udat = udat;
  c->alloc = alloc;
  c->cleanup = cleanup;
}

static inline void kkctx_destory(kkctx_t *c) {
  if (c->cleanup) c->cleanup(c->udat);
}

static inline void *kk_alloc(kkctx_t *c, void *space, kksize_t size) {
  if (c && c->alloc)
    return c->alloc(c->udat, space, size);
  else
    return 0;
}

#endif