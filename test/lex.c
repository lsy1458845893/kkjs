#include <stdio.h>
#include <stdlib.h>
#include "kkctx.h"
#include "kklex.h"
#include "kktok.h"

void *user_alloc(void *udat, void *space, kksize_t size) {
  return realloc(space, size);
}

typedef struct string_reader_s {
  uint32_t cur;
  const char *src;
} string_reader_t;

kkuint16_t string_reader_read(string_reader_t *r) {
  if (r->src[r->cur] != 0)
    return r->src[r->cur++];
  return 0;
}

int main(int argc, char const *argv[]) {
  kkctx_t ctx, *c = &ctx;
  kkctx_init(c, 0, user_alloc, 0);
  kklex_t _lex, *lex = &_lex;
  string_reader_t reader = {.src = "234.12e-2 0x34 name void test function"};
  kklex_init(lex, &reader, string_reader_read, 0);
  kkjbuf_t jbuf;
  c->jbuf = &jbuf;
  kkret_t code;
  if (!(code = kk_setjmp(jbuf))) {
    kklexi_next(c, lex);
    while (lex->tok_type) {
      switch (lex->tok_type) {
        case KKTOK_INT:
          printf("<%d> ", lex->u.inum);
          break;
        case KKTOK_FLOAT:
          printf("<%f> ", lex->u.fnum);
          break;
        case KKTOK_ID:
          if (lex->u.str->type == KKSTR_TYPE_D8C)
            printf("<id:%s> ", lex->u.str->u.str8);
          else
            printf("<error type id string> ");
          break;
        default:
          if (kklex_word_cstr(lex->tok_type))
            printf("<%s> ", kklex_word_cstr(lex->tok_type));
      }
      kklexi_next(c, lex);
    }
  } else
    printf("error %d\n", code);
  printf("\n");

  kklex_destory(c, lex);
  kkctx_destory(c);
  return 0;
}
