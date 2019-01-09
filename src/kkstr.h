#ifndef KKSTR_H
#define KKSTR_H
#include "kkctx.h"
#include "kkdep.h"

enum {
  KKSTR_TYPE_D8C,
  KKSTR_TYPE_C8C,
  KKSTR_TYPE_D8,
  KKSTR_TYPE_C8,
  KKSTR_TYPE_D16,
  KKSTR_TYPE_C16,
  KKSTR_TYPE_C16,
};

typedef struct kkstr_s {
  kkuint32_t type : 3;
  kkuint32_t refer : 29;
  kksize_t length;
  union {
    kkuint16_t *str16;
    kkuint8_t *str8;
  } u;
} kkstr_t;

kkstr_t *kkstr_from_D16(kkctx_t *c, kkuint16_t *src, kksize_t length);
kkstr_t *kkstr_from_D8C(kkctx_t *c, kkuint8_t *src);
kkstr_t *kkstr_from_D8(kkctx_t *c, kkuint8_t *src, kksize_t length);
kkstr_t *kkstr_from_C8C(kkctx_t *c, kkuint8_t *src);

void kkstr_refer(kkstr_t *s);
void kkstr_unrefer(kkctx_t *c, kkstr_t *s);

#endif