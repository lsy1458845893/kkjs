#ifndef KKLEX_H
#define KKLEX_H

#include "kkdep.h"

typedef kkuint16_t (*kklex_read_t)(void *udat);  // read unicode
typedef void (*kklex_cleanup_t)(void *udat);

typedef struct kklex_s {
  void *udat;
  kklex_read_t read;
  kklex_cleanup_t cleanup;
  kkuint8_t line_terminator : 1;
  kkuint8_t tok_type : 7;
  kkuint16_t last_ch;
  kksize_t buf_size;
  kksize_t buf_top;
  kkuint16_t *buf;
  kksize_t line;
} kklex_t;

#include "kkctx.h"

static inline void kklex_init(kklex_t *lex, void *udat, kklex_read_t read, kklex_cleanup_t cleanup) {
  lex->udat = udat;
  lex->read = read;
  lex->cleanup = cleanup;
  lex->line_terminator = 0;
  lex->tok_type = 0;
  lex->last_ch = 0;
  lex->buf_size = 0;
  lex->buf_top = 0;
  lex->buf = 0;
  lex->line = 1;
}
static inline void kklex_destory(kkctx_t *c, kklex_t *lex) {
  if (lex->buf_size) kk_alloc(c, lex->buf, 0);
  if (lex->cleanup) lex->cleanup(lex->udat);
}

uint8_t kklexi_next(kkctx_t *c,kklex_t *lex);

#endif