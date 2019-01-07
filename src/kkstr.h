#ifndef KKSTR_H
#define KKSTR_H
#include "kkdep.h"

typedef struct kkstr_s {
  kkuint32_t type : 2;
  kkuint32_t length : 30;
  union {
    kkuint16_t *str16;
    kkuint8_t *str8;
  } u;
} kkstr_t;

#endif