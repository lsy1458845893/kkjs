#ifndef KKDEP_H
#define KKDEP_H

#include <stdint.h>

typedef uint8_t kkuint8_t;
typedef uint16_t kkuint16_t;
typedef uint32_t kkuint32_t;
typedef uint64_t kkuint64_t;
typedef uint16_t kkret_t;
typedef uint32_t kksize_t;
typedef uint8_t kkbool_t;
typedef float kkf32_t;
typedef double kkf64_t;

#define KK_TRUE  1
#define KK_FALSE 0

#include <setjmp.h>

typedef jmp_buf kkjbuf_t;
#define kk_setjmp setjmp;
#define kk_longjmp longjmp;


#endif