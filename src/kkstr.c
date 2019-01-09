#include "kkstr.h"

static kkstr_t *kkstr_create_buffer(kkctx_t *c, kksize_t size) {
  kkstr_t *s = kk_alloc(c, 0, sizeof(kkstr_t));
  if (!s) goto mem_alloc_err;
  s->u.str16 = kk_alloc(c, 0, size);
  if (!s->u.str16) goto mem_alloc_err;
  s->refer = 1;
  return s;
mem_alloc_err:
  if (s) kk_alloc(c, s, 0);
  return 0;
}

static kkstr_t *kkstr_create_D16(kkctx_t *c, kkuint16_t *src, kksize_t length) {
  kkstr_t *s = kkstr_create_buffer(c, length * sizeof(kkuint16_t));
  if (!s) return 0;
  kk_memcpy(s->u.str16, src, length * sizeof(kkuint16_t));
  s->type = KKSTR_TYPE_D16;
  s->length = length;
  return s;
}

static kkstr_t *kkstr_create_D8(kkctx_t *c, kkuint8_t *src, kksize_t length) {
  kkstr_t *s = kkstr_create_buffer(c, length * sizeof(kkuint8_t));
  if (!s) return 0;
  kk_memcpy(s->u.str8, src, length * sizeof(kkuint8_t));
  s->type = KKSTR_TYPE_D16;
  s->length = length;
  return s;
}

kkstr_t *kkstr_from_D16(kkctx_t *c, kkuint16_t *src, kksize_t length) {
  kkbool_t iscstr = 1;
  for (kksize_t i = 0; i < length; i++) {
    if (src[i] > 0xff) return kkstr_create_D16(c, src, length);
    if (src[i] == 0) iscstr = 0;
  }
  if (iscstr) {
    kkstr_t *s = kkstr_create_buffer(c, length + 1);
    for (kksize_t i = 0; i < length; i++)
      s->u.str8[i] = src[i];
    s->u.str8[length] = 0;
    s->length = length;
    s->type = KKSTR_TYPE_D8C;
    return s;
  } else {
    kkstr_t *s = kkstr_create_buffer(c, length);
    for (kksize_t i = 0; i < length; i++)
      s->u.str8[i] = src[i];
    s->length = length;
    s->type = KKSTR_TYPE_D8;
    return s;
  }
}

kkstr_t *kkstr_from_D8(kkctx_t *c, kkuint8_t *src, kksize_t length) {
  kkbool_t iscstr = 1;
  for (kksize_t i = 0; i < length; i++) {
    if (src[i] == 0) {
      iscstr = 0;
      break;
    }
  }
  if (iscstr) {
    kkstr_t *s = kkstr_create_buffer(c, length + 1);
    if (!s) return 0;
    kk_memcpy(s->u.str8, src, length);
    s->u.str8[length] = 0;
    s->length = length;
    s->type = KKSTR_TYPE_D8C;
    return s;
  } else
    return kkstr_create_D8(c, src, length);
}

kkstr_t *kkstr_from_D8C(kkctx_t *c, kkuint8_t *src) {
  kksize_t length = strlen(src);
  kkstr_t *s = kkstr_create_buffer(c, length + 1);
  if (!s) return 0;
  kk_memcpy(s->u.str8, src, length + 1);
  s->type = KKSTR_TYPE_D8C;
  s->length = length;
  return s;
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
