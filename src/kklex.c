#include "kklex.h"
#include "kkerr.h"

#define kklex_throw(code) kk_longjmp(*(c->jbuf), __LINE__)

static void kklexi_buf_clean(kkctx_t *c, kklex_t *lex) {
  lex->buf_top = 0;
  if (lex->buf_size > 64) {
    lex->buf = kk_alloc(c, lex->buf, 16 * sizeof(kkuint16_t));
    if (!lex->buf) goto mem_alloc_error;
    lex->buf_size = 16;
    return;
  } else
    return;
mem_alloc_error:
  lex->buf_size = lex->buf_top = 0;
  kklex_throw(KKERR_NO_MEMORY);
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
  kklex_throw(KKERR_NO_MEMORY);
  return;
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

static inline void kklex_reader_clean(kklex_t *lex) {
  if (lex->cleanup) lex->cleanup(lex->udat);
  lex->cleanup = lex->read = lex->udat = 0;
}

static kkuint16_t kklex_get(kkctx_t *c, kklex_t *lex) {
  kkuint16_t ch = lex->last_ch;
  if (lex->read) {
    if (ch == 0) {
      ch = lex->read(lex->udat);
      if (ch == 0) {
        kklex_reader_clean(lex);
        return 0;
      } else if (kklex_is_line_terminator(ch))
        lex->line++;
      lex->last_ch = ch;
      if (lex->last_ch == 0)  // end of file
        kklex_reader_clean(lex);
    } else {
      lex->last_ch = lex->read(lex->udat);
      if (lex->last_ch == 0)  // end of file
        kklex_reader_clean(lex);
      else if (kklex_is_line_terminator(ch))  // not end of file, but end of line
        lex->line++;
    }
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

static inline kkbool_t kklex_range(kkuint16_t ch, kkuint16_t a, kkuint16_t b) {
  return (a <= ch && ch <= b) ? KK_TRUE : KK_FALSE;
}

static inline kkuint8_t kklex_hex_to_inum(kkuint16_t ch) {
  if (kklex_range(ch, '0', '9'))
    return ch - '0';
  else if (kklex_range(ch, 'a', 'f'))
    return ch - 'a' + 10;
  else if (kklex_range(ch, 'A', 'F'))
    return ch - 'A' + 10;
  else
    return 16;
}

static inline kkbool_t kklex_match(kkctx_t *c, kklex_t *lex, kkuint16_t ch) {
  if (kklex_read(c, lex) == ch) {
    kklex_get(c, lex);
    return KK_TRUE;
  } else
    return KK_FALSE;
}

static inline kkbool_t kklex_range_test(kkctx_t *c, kklex_t *lex, kkuint16_t a, kkuint16_t b) {
  return kklex_range(kklex_read(c, lex), a, b) ? KK_TRUE : KK_FALSE;
}

static inline kkbool_t kklexi_range_push(kkctx_t *c, kklex_t *lex, kkuint16_t a, kkuint16_t b) {
  kkbool_t ret = kklex_range_test(c, lex, a, b);
  if (ret) kklexi_push(c, lex, kklex_get(c, lex));
  return ret;
}

#include "kktok.h"
static const kkuint8_t *kklex_wordstr_tab[] = {
#define KKLEX_WORD_STR(name, str) str,
#define KKLEX_GROUP(n, ch) n(KKLEX_WORD_STR)
    KKTOK_WORD_GROUPS(KKLEX_GROUP)
#undef KKLEX_GROUP
#undef KKLEX_WORD_STR
};

const kkuint8_t *kklex_word_cstr(kkuint8_t tok) {
  enum {
#define KKLEX_WORD_INDEX(name, str) KKLEX_TOK_##name##_INDEX,
#define KKLEX_GROUP(n, ch) n(KKLEX_WORD_INDEX)
    KKTOK_WORD_GROUPS(KKLEX_GROUP)
#undef KKLEX_GROUP
#undef KKLEX_WORD_INDEX
  };
  switch (tok) {
#define KKLEX_WORD_INDEX(name, str) \
  case KKTOK_##name:                \
    return kklex_wordstr_tab[KKLEX_TOK_##name##_INDEX];
#define KKLEX_GROUP(n, ch) n(KKLEX_WORD_INDEX)
    KKTOK_WORD_GROUPS(KKLEX_GROUP)
#undef KKLEX_GROUP
#undef KKLEX_WORD_INDEX
    default:
      return 0;
  }
}

static kkbool_t kklex_compare_string(kklex_t *lex, const kkuint8_t *str) {
  kkuint8_t i = 0;
  for (i = 0; str[i]; i++)
    if (str[i] != lex->buf[i]) return KK_FALSE;
  if (i != lex->buf_top) return KK_FALSE;
  return KK_TRUE;
}

static kkuint8_t kklex_match_keyword(kklex_t *lex) {
  switch (lex->buf[0]) {
#define KKLEX_WORD_INDEX(name, str) \
  if (kklex_compare_string(lex, kklex_word_cstr(KKTOK_##name))) return KKTOK_##name;
#define KKLEX_GROUP(n, ch) \
  case ch:                 \
    n(KKLEX_WORD_INDEX) return 0;
    KKTOK_WORD_GROUPS(KKLEX_GROUP)
#undef KKLEX_GROUP
#undef KKLEX_WORD_INDEX
  }
}

static inline void kklex_clean_lasttype(kkctx_t *c, kklex_t *lex) {
  switch (lex->tok_type) {
    case KKTOK_ID:
    case KKTOK_STRING:
      kkstr_unrefer(c, lex->u.str);
      break;
    case KKTOK_REGEXP:  //!!! regexp module need !!!
      break;
  }
  lex->line_terminator = 0;
  lex->tok_type = 0;
}

void kklex_destory(kkctx_t *c, kklex_t *lex) {
  kklex_clean_lasttype(c, lex);
  if (lex->buf_size) kk_alloc(c, lex->buf, 0);
  if (lex->cleanup) lex->cleanup(lex->udat);
}

#define kk_match(ch) kklex_match(c, lex, ch)
#define kk_get() kklex_get(c, lex)
#define kk_read() kklex_read(c, lex)
#define kk_test(ch) (kklex_read(c, lex) == ch)
#define kk_range kklex_range
#define kk_range_test(a, b) kklex_range_test(c, lex, a, b)
#define kk_range_push(a, b) kklexi_range_push(c, lex, a, b)
#define kk_ret(t) return lex->tok_type = t

static kkf64_t kklexi_get_exponent(kkctx_t *c, kklex_t *lex) {
  kkf64_t fnum = 1;
  kkbool_t neg = 0;
  kkuint32_t i = 0;
  if (kk_match('-'))
    neg = 1;
  else
    kk_match('+');
  if (kk_range_test('0', '9')) {
    while (kk_range_test('0', '9'))
      i = i * 10 + (kk_get() - '0');
  } else
    kklex_throw(KKERR_SYNTAX_ERROR__INVAILD_OR_UNEXPECTED_TOKEN);
  while (i--) fnum *= neg ? 0.1 : 10;
  return fnum;
}

static kkf64_t kklex_get_decimal(kkctx_t *c, kklex_t *lex) {
  kkf64_t f = 0, t = 0.1;
  while (kk_range_test('0', '9')) {
    f += (kk_get() - '0') * t;
    t *= 0.1;
  }
  return f;
}

static kkuint16_t kklexi_parse_string_ch(kkctx_t *c, kklex_t *lex) {
  if (kk_match('\\')) {
    if (kk_match('x')) {
      kkuint16_t ihex = 0;
      for (kkuint8_t i = 0; i < 2; i++) {
        if (kk_range_test('0', '9') || kk_range_test('a', 'f') || kk_range_test('A', 'F'))
          ihex = ihex * 16 + kklex_hex_to_inum(kk_get());
        else
          kklex_throw(KKERR_SYNTAX_ERROR__INVAILD_OR_UNEXPECTED_TOKEN);
      }
      if (kk_range_test('a', 'z') || kk_range_test('A', 'Z') || kk_test('_') || kk_test('$'))
        kklex_throw(KKERR_SYNTAX_ERROR__INVAILD_OR_UNEXPECTED_TOKEN);
      return ihex;
    } else if (kk_match('u')) {
      kkuint16_t ihex = 0;
      for (kkuint8_t i = 0; i < 4; i++) {
        if (kk_range_test('0', '9') || kk_range_test('a', 'f') || kk_range_test('A', 'F'))
          ihex = ihex * 16 + kklex_hex_to_inum(kk_get());
        else
          kklex_throw(KKERR_SYNTAX_ERROR__INVAILD_OR_UNEXPECTED_TOKEN);
      }
      if (kk_range_test('a', 'z') || kk_range_test('A', 'Z') || kk_test('_') || kk_test('$'))
        kklex_throw(KKERR_SYNTAX_ERROR__INVAILD_OR_UNEXPECTED_TOKEN);
      return ihex;
    }
  }
  return kk_get();
}

uint8_t kklexi_next(kkctx_t *c, kklex_t *lex) {
  if (!c->jbuf) return 0;
  kklex_clean_lasttype(c, lex);
  if (lex->buf_top) kklex_throw(KKERR_SYNTAX_ERROR__INVAILD_OR_UNEXPECTED_TOKEN);
check:
  if (kk_match(0)) {
    kk_ret(KKTOK_EOF);
  } else if (kklex_is_line_terminator(kk_read())) {
    lex->line++;
    lex->line_terminator = 1;
    kk_get();
    goto check;
  } else if (kklex_is_white_space(kk_read())) {
    kk_get();
    goto check;
  } else if (kk_range_test('a', 'z') || kk_range_test('A', 'Z') || kk_test('_') || kk_test('$')) {
    // match id
    while (kk_range_test('a', 'z') ||
           kk_range_test('A', 'Z') ||
           kk_range_test('0', '9') ||
           kk_test('_') || kk_test('$'))
      kklexi_push(c, lex, kk_get());
    if (lex->tok_type = kklex_match_keyword(lex)) {
      lex->buf_top = 0;
      return lex->tok_type;
    }
    //!!! return id str !!!
    lex->u.str = kkstr_from_D16(c, lex->buf, lex->buf_top);
    lex->buf_top = 0;
    if (!lex->u.str) kklex_throw(KKERR_NO_MEMORY);
    return lex->tok_type = KKTOK_ID;
  } else if (kk_range_test('0', '9')) {
    if (kk_match('0')) {
      if (kk_match('x') || kk_match('X')) {
        // match hex
        if (kk_range_test('0', '9') || kk_range_test('a', 'f') || kk_range_test('A', 'F')) {
          kkuint64_t inum = 0;
          while (kk_range_test('0', '9') || kk_range_test('a', 'f') || kk_range_test('A', 'F'))
            inum = inum * 16 + kklex_hex_to_inum(kk_get());
          lex->u.inum = inum;
          lex->tok_type = KKTOK_INT;
        } else
          kklex_throw(KKERR_SYNTAX_ERROR__INVAILD_OR_UNEXPECTED_TOKEN);
      } else if (kk_match('b') || kk_match('B')) {
        // match binary
        if (kk_range_test('0', '1')) {
          kkuint64_t inum = 0;
          while (kk_range_test('0', '1'))
            inum = inum * 2 + (kk_get() - '0');
          lex->u.inum = inum;
          lex->tok_type = KKTOK_INT;
        } else
          kklex_throw(KKERR_SYNTAX_ERROR__INVAILD_OR_UNEXPECTED_TOKEN);
      } else if (kk_match('o') || kk_match('O')) {
        // match octal
        if (kk_range_test('0', '7')) {
          kkuint64_t inum = 0;
          while (kk_range_test('0', '7'))
            inum = inum * 8 + (kk_get() - '0');
          lex->u.inum = inum;
          lex->tok_type = KKTOK_INT;
        } else
          kklex_throw(KKERR_SYNTAX_ERROR__INVAILD_OR_UNEXPECTED_TOKEN);
      } else {
        // match number start with '0'
        kkbool_t isoct = 1, isfloat = 0;
        while (kk_range_test('0', '9')) {
          if (kk_range_test('8', '9'))
            isoct = 0;
          kklexi_push(c, lex, kk_get());
        }
        kkf64_t fnum = 0, e = 1;
        if (kk_match('.')) {
          isfloat = 1;
          isoct = 0;
          fnum = kklex_get_decimal(c, lex);
        }
        if (kk_match('e') || kk_match('E')) {
          isfloat = 1;
          isoct = 0;
          e = kklexi_get_exponent(c, lex);
        }
        kkuint64_t inum = 0;
        for (kksize_t i = 0; i < lex->buf_top; i++) {
          if (isoct)
            inum = inum * 8 + (lex->buf[i] - '0');
          else
            inum = inum * 10 + (lex->buf[i] - '0');
        }
        lex->buf_top = 0;
        if (isfloat) {
          lex->u.fnum = (inum + fnum) * e;
          lex->tok_type = KKTOK_FLOAT;
        } else {
          lex->u.inum = inum;
          lex->tok_type = KKTOK_INT;
        }
      }
      if (kk_range_test('a', 'z') || kk_range_test('A', 'Z') || kk_test('_') || kk_test('$'))
        kklex_throw(KKERR_SYNTAX_ERROR__INVAILD_OR_UNEXPECTED_TOKEN);
      return lex->tok_type;
    } else {
      kkuint64_t inum = 0;
      kkbool_t isfloat = 0;
      while (kk_range_test('0', '9'))
        inum = inum * 10 + (kk_get() - '0');
      kkf64_t fnum = 0, e = 1;
      if (kk_match('.')) {
        isfloat = 1;
        fnum = kklex_get_decimal(c, lex);
      }
      if (kk_match('e') || kk_match('E')) {
        isfloat = 1;
        e = kklexi_get_exponent(c, lex);
      }
      if (isfloat) {
        lex->u.fnum = (inum + fnum) * e;
        lex->tok_type = KKTOK_FLOAT;
      } else {
        lex->u.inum = inum;
        lex->tok_type = KKTOK_INT;
      }
      if (kk_range_test('a', 'z') || kk_range_test('A', 'Z') || kk_test('_') || kk_test('$'))
        kklex_throw(KKERR_SYNTAX_ERROR__INVAILD_OR_UNEXPECTED_TOKEN);
      return lex->tok_type;
    }
  }
  switch (kk_get()) {
    case '~':
      kk_ret(KKTOK_BIT_NOT);  //  ~
    case '}':
      kk_ret(KKTOK_BRACE_R);  //  }
    case '|':
      if (kk_match('|')) kk_ret(KKTOK_LOGIC_OR);  //  ||
      if (kk_match('=')) kk_ret(KKTOK_BOR_EQ);    //  |=
      kk_ret(KKTOK_BIT_OR);                       //  |
    case '{':
      kk_ret(KKTOK_BRACE_L);  //  {
    case '^':
      if (kk_match('=')) kk_ret(KKTOK_BXOR_EQ);  //  ^=
      kk_ret(KKTOK_BIT_XOR);                     //  ^
    case ']':
      kk_ret(KKTOK_BRACKET_R);  //  ]
    case '[':
      kk_ret(KKTOK_BRACKET_L);  //  [
    case '?':
      kk_ret(KKTOK_QUESTION);  //  ?
    case '>':
      if (kk_match('>')) {
        if (kk_match('>')) {
          if (kk_match('=')) kk_ret(KKTOK_SHR_EQ);  //  >>>=
          kk_ret(KKTOK_SHR);                        //  >>>
        }
        if (kk_match('=')) kk_ret(KKTOK_SAR_EQ);  //  >>=
        kk_ret(KKTOK_SAR);                        //  >>
      }
      if (kk_match('=')) kk_ret(KKTOK_GE);  //  >=
      kk_ret(KKTOK_GT);                     //  >
    case '=':
      if (kk_match('>')) kk_ret(KKTOK_ARROW);  //  =>
      if (kk_match('=')) {
        if (kk_match('=')) kk_ret(KKTOK_EQ_STRICT);  //  ===
        kk_ret(KKTOK_EQ);                            //  ==
      }
      kk_ret(KKTOK_ASSIGN);  // =
    case '<':
      if (kk_match('=')) kk_ret(KKTOK_LE);  //  <=
      if (kk_match('<')) {
        if (kk_match('=')) kk_ret(KKTOK_SHL_EQ);  //  <<=
        kk_ret(KKTOK_SHL);                        //  <<
      }
      kk_ret(KKTOK_LT);  //  <
    case ';':
      kk_ret(KKTOK_SEM);  //  ;
    case ':':
      kk_ret(KKTOK_COLON);  //  :
    case '/':
      if (kk_match('=')) kk_ret(KKTOK_DIV_EQ);  //  /=
      if (kk_match('/')) {                      // "//"
        while (kk_read() != 0 && (!kklex_is_line_terminator(kk_read())))
          kk_get();
        goto check;
      }
      if (kk_match('*')) {  // "/*"
        while (1) {
          if (kk_match(0)) kklex_throw(KKERR_SYNTAX_ERROR__INVAILD_OR_UNEXPECTED_TOKEN);
          if (kk_match('*') && kk_match('/')) goto check;
          kk_get();
        }
      }
      kk_ret(KKTOK_DIV);  //  /
    case '.':
      if (kk_match('.')) {
        if (kk_match('.')) kk_ret(KKTOK_ELLIPSIS);  //  ...
        kklex_throw(KKERR_SYNTAX_ERROR__INVAILD_OR_UNEXPECTED_TOKEN);
      }
      if (kk_range_test('0', '9')) {
        lex->u.fnum = kklex_get_decimal(c, lex);
        if (kk_range_test('a', 'z') || kk_range_test('A', 'Z') || kk_test('_') || kk_test('$'))
          kklex_throw(KKERR_SYNTAX_ERROR__INVAILD_OR_UNEXPECTED_TOKEN);
        return lex->tok_type = KKTOK_FLOAT;
      }
      kk_ret(KKTOK_DOT);  //  .
    case '-':
      if (kk_match('=')) kk_ret(KKTOK_SUB_EQ);  //  -=
      if (kk_match('-')) kk_ret(KKTOK_DEC);     //  --
      kk_ret(KKTOK_SUB);                        //  -
    case ',':
      kk_ret(KKTOK_COM);  //  ,
    case '+':
      if (kk_match('=')) kk_ret(KKTOK_ADD_EQ);  //  +=
      if (kk_match('+')) kk_ret(KKTOK_INC);     //  ++
      kk_ret(KKTOK_ADD);                        //  +
    case '*':
      if (kk_match('=')) kk_ret(KKTOK_MUL_EQ);  //  *=
      kk_ret(KKTOK_MUL);                        //  *
    case ')':
      kk_ret(KKTOK_PAREN_R);  //  )
    case '(':
      kk_ret(KKTOK_PAREN_L);  //  (
    case '&':
      if (kk_match('=')) kk_ret(KKTOK_BAND_EQ);    //  &=
      if (kk_match('&')) kk_ret(KKTOK_LOGIC_AND);  //  &&
      kk_ret(KKTOK_BIT_AND);                       //  &
    case '%':
      if (kk_match('=')) kk_ret(KKTOK_MOD_EQ);  //  %=
      kk_ret(KKTOK_MOD);                        //  %
    case '!':
      if (kk_match('=')) {
        if (kk_match('=')) kk_ret(KKTOK_NE_STRICT);  //  !==
        kk_ret(KKTOK_NE);                            //  !=
      }
      kk_ret(KKTOK_LOGIC_NOT);  //  !
    case '"':
      while (!kk_test('"')) {
        if (kk_test(0)) kklex_throw(KKERR_SYNTAX_ERROR__INVAILD_OR_UNEXPECTED_TOKEN);
        kklexi_push(c, lex, kklexi_parse_string_ch(c, lex));
      }
      lex->u.str = kkstr_from_D16(c, lex->buf, lex->buf_top);
      if (!lex->u.str) kklex_throw(KKERR_NO_MEMORY);
      return lex->tok_type = KKTOK_STRING;
    case '\'':
      while (!kk_test('\'')) {
        if (kk_test(0)) kklex_throw(KKERR_SYNTAX_ERROR__INVAILD_OR_UNEXPECTED_TOKEN);
        kklexi_push(c, lex, kklexi_parse_string_ch(c, lex));
      }
      lex->u.str = kkstr_from_D16(c, lex->buf, lex->buf_top);
      if (!lex->u.str) kklex_throw(KKERR_NO_MEMORY);
      return lex->tok_type = KKTOK_STRING;
    default:
      kklex_throw(KKERR_SYNTAX_ERROR__INVAILD_OR_UNEXPECTED_TOKEN);
  }
}

#undef kk_ret
#undef kk_range_push
#undef kk_range_test
#undef kk_range
#undef kk_get
#undef kk_read
#undef kk_test
#undef kk_match
