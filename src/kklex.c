#include "kklex.h"
#include "kkerr.h"

static void kklexi_buf_clean(kkctx_t *c, kklex_t *lex) {
  if (lex->buf_size != 16) {
    lex->buf = kk_alloc(c, lex->buf, 16 * sizeof(kkuint16_t));
    if (!lex->buf) goto mem_alloc_error;
    lex->buf_size = 16;
    return;
  } else
    return;
mem_alloc_error:
  lex->buf_size = lex->buf_top = 0;
  kk_longjmp(*(c->jbuf), KKERR_NO_MEMORY);
  return;
}

static void kklexi_push(kkctx_t *c, kklex_t *lex, kkuint16_t ch) {
  if (lex->buf == 0 || lex->buf_size == 0) {
    lex->buf = kk_alloc(c, lex->buf, 16 * sizeof(kkuint16_t));
    if (!lex->buf) goto mem_alloc_error;
    lex->buf_size = 16;
  }
  kksize_t nsize = lex->buf_size;
  if (lex->buf_size < 64) {
    if (lex->buf_size > lex->buf_top * 4) nsize /= 2;
    if (lex->buf_size <= lex->buf_top) nsize *= 2;
    if (nsize < 16) nsize = 16;
  } else if (lex->buf_size < 2048) {
    if (lex->buf_size > lex->buf_top * 4) nsize /= 2;
    if (lex->buf_size <= lex->buf_top) nsize *= 2;
  } else {
    if (lex->buf_size > lex->buf_top * 4) nsize /= 2;
    if (lex->buf_size <= lex->buf_top) nsize += 1024;
  }
  if (nsize != lex->buf_size) {
    lex->buf = kk_alloc(c, lex->buf, nsize * sizeof(kkuint16_t));
    if (!lex->buf) goto mem_alloc_error;
    lex->buf_size = nsize;
  }
  lex->buf[lex->buf_top++] = ch;
  return;
mem_alloc_error:
  lex->buf_size = lex->buf_top = 0;
  kk_longjmp(*(c->jbuf), KKERR_NO_MEMORY);
  return;
}

static kkuint16_t kklex_get(kkctx_t *c, kklex_t *lex) {
  kkuint16_t ch = lex->last_ch;
  if (lex->read) {
    lex->last_ch = lex->read(lex->udat);
    if (lex->last_ch == 0) {  // end of file
      if (lex->cleanup) lex->cleanup(lex->udat);
      lex->cleanup = lex->read = lex->udat = 0;
    } else if (kklex_is_line_terminator(ch))  // not end of file, but end of line
      lex->line++;
  } else
    lex->last_ch = 0;
  return ch;
}

static kkuint16_t kklex_read(kkctx_t *c, kklex_t *lex) {
  if (lex->last_ch)
    return lex->last_ch;
  else
    return kklex_get(c, lex);
}

static kkbool_t kklex_is_white_space(kkuint16_t ch) {
  switch (ch) {
    case 0x0009:  //	CHARACTER TABULATION	    <TAB>
    case 0x000B:  //	LINE TABULATION	          <VT>
    case 0x000C:  //	FORM FEED (FF)	          <FF>
    case 0x0020:  //	SPACE	                    <SP>
    case 0x00A0:  //	NO-BREAK SPACE	          <NBSP>
    case 0xFEFF:  //	ZERO WIDTH NO-BREAK SPACE	<ZWNBSP>
      return KK_TRUE;
    default:
      return KK_FALSE;
  }
}

static kkbool_t kklex_is_line_terminator(kkuint16_t ch) {
  switch (ch) {
    case 0x000A:  //	LINE FEED (LF)	       <LF>
    case 0x000D:  //	CARRIAGE RETURN (CR)	 <CR>
    case 0x2028:  //	LINE SEPARATOR	       <LS>
    case 0x2029:  //	PARAGRAPH SEPARATOR	   <PS>
      return KK_TRUE;
    default:
      return KK_FALSE;
  }
}

static inline kkbool_t kklex_is_number_char(kkuint16_t ch) {
  if ('0' <= ch && ch <= '9')
    return KK_TRUE;
  else
    return KK_FALSE;
}

static inline kkbool_t kklex_is_octal_char(kkuint16_t ch) {
  if ('0' <= ch && ch <= '7')
    return KK_TRUE;
  else
    return KK_FALSE;
}

static kkbool_t kklex_is_id_char(kkuint16_t ch) {
  if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '$' || ch == '_')
    return KK_TRUE;
  else
    return KK_FALSE;
}

static kkf64_t kklex_match_decimal(kkctx_t *c, kklex_t *lex) {
  kkf64_t f = 0, t = 0.1;
  while (kklex_is_number_char(kklex_read(c, lex))) {
    f += (kklex_get(c, lex) - '0') * t;
    t /= 10;
  }
  return f;
}

static kkuint64_t kklex_match_integer(kkctx_t *c, kklex_t *lex) {}

static inline kkbool_t kklex_match(kkctx_t *c, kklex_t *lex, kkuint16_t ch) {
  if (kklex_read(c, lex) == ch) {
    kklex_get(c, lex);
    return KK_TRUE;
  } else
    return KK_FALSE;
}

#define kk_match(ch) kklex_match(c, lex, ch)
#define kk_get() kklex_get(c, lex)
#define kk_read() kklex_read(c, lex)

uint8_t kklexi_next(kkctx_t *c, kklex_t *lex) {
  if (c->jbuf == 0) return 0;
  lex->line_terminator = 0;
check:
  // end of line
  if (kklex_is_line_terminator(kk_read())) {
    kk_get();
    lex->line_terminator = 1;
    goto check;
  }

  // white space
  if (kklex_is_white_space(kk_read())) {
    kk_get();
    goto check;
  }

  // match number
  if (kklex_is_number_char(kk_read())) {
    if (kk_match('0')) {
      // match octal or decimal
      if (kklex_is_number_char(kk_read())) {  // '0'[0-9]
        while (1) {
          if (kk_match('.')) {
          }
        }
      }
      // match hex
      if (kk_match('x') || kk_match('X')) {  // 0x***
      }
      // match binary
      if (kk_match('b') || kk_match('B')) {  // 0b***
      }
    }
    kkuint64_t inum = kklex_match_integer(c, lex);
    kkf64_t fnum = 0;
    if (kk_match('.')) fnum = kklex_match_decimal(c, lex);
    // match exponent
    if (kk_match('e') || kk_match('E')) {  // 0e
      if (kk_match('-')) {                 // xxxe-***
        kkuint64_t p = kklex_match_integer(c, lex);
      } else {
        kk_match('+');
      }
    }
  }
}

#undef kk_match
#undef kk_get
#undef kk_read
