#ifndef KKLEX_H
#define KKLEX_H

#include "kkdep.h"
#include "kkstr.h"

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
  union {
    kkuint64_t inum;
    kkf64_t fnum;
    kkstr_t *str;
  }u;
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
void kklex_destory(kkctx_t *c, kklex_t *lex);

const kkuint8_t *kklex_word_cstr(kkuint8_t tok);
uint8_t kklexi_next(kkctx_t *c,kklex_t *lex);

#endif