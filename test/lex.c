#include <stdio.h>
#include <stdlib.h>
#include "kkctx.h"
#include "kklex.h"
#include "kktok.h"

const char *get_token_string(kkuint8_t tok) {
  static const char *tab[] = {
      "<EOF>",
      "<string>",
      "<regexp>",
      "<id>",
      "<int>",
      "<float>",
      "arguments",
      "async",
      "await",
      "break",
      "case",
      "catch",
      "class",
      "const",
      "constructor",
      "continue",
      "default",
      "delete",
      "do",
      "else",
      "export",
      "extends",
      "false",
      "finally",
      "for",
      "function",
      "get",
      "if",
      "import",
      "in",
      "instanceof",
      "let",
      "new",
      "null",
      "of",
      "return",
      "set",
      "static",
      "super",
      "switch",
      "this",
      "throw",
      "true",
      "try",
      "typeof",
      "var",
      "void",
      "while",
      "yield",
      "~",
      "}",
      "||",
      "|=",
      "|",
      "{",
      "^=",
      "^",
      "]",
      "[",
      "?",
      ">>>=",
      ">>>",
      ">>=",
      ">>",
      ">=",
      ">",
      "=>",
      "===",
      "==",
      "=",
      "<=",
      "<<=",
      "<<",
      "<",
      ";",
      ":",
      "/=",
      "/",
      "...",
      ".",
      "-=",
      "--",
      "-",
      ",",
      "+=",
      "++",
      "+",
      "*=",
      "*",
      ")",
      "(",
      "&=",
      "&&",
      "&",
      "%=",
      "%",
      "!==",
      "!=",
      "!",
  };
  return tab[tok];
}

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

static const *src =
    "()=>{ 'test' + 1; }\n"
    "function test(...args){ return args; } // \n"
    "function test(...args){ return args; } /* \n"
    "function test(...args){ return args; } */ \n"
    "function test(...args){ return args; } // \n"
    ;

int main(int argc, char const *argv[]) {
  kkctx_t ctx, *c = &ctx;
  kkctx_init(c, 0, user_alloc, 0);
  kklex_t _lex, *lex = &_lex;
  string_reader_t reader = {.src = src};
  kklex_init(lex, &reader, string_reader_read, 0);
  kkjbuf_t jbuf;
  c->jbuf = &jbuf;
  kkret_t code;
  if (!(code = kk_setjmp(jbuf))) {
    kklexi_next(c, lex);
    while (lex->tok_type) {
      switch (lex->tok_type) {
        case KKTOK_INT:
          printf("<%d:%d> ", lex->u.inum, lex->line);
          break;
        case KKTOK_FLOAT:
          printf("<%f:%d> ", lex->u.fnum, lex->line);
          break;
        case KKTOK_ID:
          if (lex->u.str->type == KKSTR_TYPE_D8C)
            printf("<id:%s:%d> ", lex->u.str->u.str8, lex->line);
          else
            printf("<error type id string> ");
          break;
        case KKTOK_STRING:
          if (lex->u.str->type == KKSTR_TYPE_D8C)
            printf("<str:%s:%d> ", lex->u.str->u.str8, lex->line);
          else
            printf("<error type id string> ");
          break;
        default:
          printf("<%s:%d> ", get_token_string(lex->tok_type), lex->line);
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
