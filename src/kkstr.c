#include "kkstr.h"

kkstr_t *kkstr_from_D16(kkctx_t *c, kkuint16_t *src, kksize_t length) {
  //!!! wait to write !!!
  return 0;
}
kkstr_t *kkstr_from_D8C(kkctx_t *c, kkuint8_t *src) {
  //!!! wait to write !!!
  return 0;
}
kkstr_t *kkstr_from_C8C(kkctx_t *c, kkuint8_t *src) {
  kkstr_t *s = kk_alloc(c, 0, sizeof(kkstr_t));
  if (!s) goto mem_alloc_err;
  s->type = KKSTR_TYPE_C8C;
  s->refer = 1;
  s->length = kk_strlen(src);
  s->u.str8 = src;
mem_alloc_err:
  if (s) kk_alloc(c, s, 0);
  return 0;
}

void kkstr_refer(kkstr_t *s) {
  s->refer++;
}

void kkstr_unrefer(kkctx_t *c, kkstr_t *s) {
  s->refer--;
  if (s->refer == 0) {
    switch (s->type) {
      case KKSTR_TYPE_D8C:
      case KKSTR_TYPE_D8:
        kk_alloc(c, s->u.str8, 0);
        break;
      case KKSTR_TYPE_D16:
        kk_alloc(c, s->u.str16, 0);
        break;
    }
    kk_alloc(c, s, 0);
  }
}
