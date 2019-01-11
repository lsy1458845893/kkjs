#ifndef KKPARSE_H
#define KKPARSE_H

enum {
  /**
   * function_parameter_declare:
   *  <(> [<...> <id>] <)>
   *  <(> init_pair [<,> init_pair ] * [<,> <...> <id>] <)>
   */
  /**
   * function_declare:
   *  function
   *  arrow
   *  async_function
   *  async_arrow
   *  generator_function
   */
  /**
   * express_statment:
   *  expr
   */
  KKAST_EXPRESS_STATMENT,
  /**
   * for_statment:(4)
   *  <for> <(> <;> [expr] <;> [expr] <)> statment
   *  <for> <(> express_statment <;> [expr] <;> [expr] <)> statment
   *  <for> <(> var_statment <;> [expr] <;> [expr] <)> statment
   *  <for> <(> let_statment <;> [expr] <;> [expr] <)> statment
   *  <for> <(> const_statment <;> [expr] <;> [expr] <)> statment
   * for_in_statment:(3)
   *  <for> <(> <id> <in> expr <)> statment
   *  <for> <(> var_statment <in> expr <)> statment
   *  <for> <(> let_statment <in> expr <)> statment
   * for_of_statment:(3)
   *  <for> <(> <id> <of> expr <)> statment
   *  <for> <(> var_statment <of> expr <)> statment
   *  <for> <(> let_statment <of> expr <)> statment
   */
  KKAST_FOR_STATMENT,
  KKAST_FOR_IN_STATMENT,
  KKAST_FOR_OF_STATMENT,
  /**
   * if_statment:(3)
   *  <if> <(> expr <)> statment [<else> statment]
   */
  KKAST_IF_STATMENT,
  /**
   * while_statment:(2) 
   *  <while> <(> expr <)> statment
   */
  KKAST_WHILE_STATMENT,
  /**
   * do_while_statment:(2) 
   *  <do> block <while> <(> expr <)> <;>
   */
  KKAST_DO_WHILE_STATMENT,
  /**
   * init_pair:(2)
   *  <id> [<=> expr]
   */
  KKAST_INIT_PAIR,
  /**
   * var_statment:(n)
   *  var init_pair [<,> init_pair]*
   * let_statment:
   *  let init_pair [<,> init_pair]*
   * const_statment:
   *  const init_pair [<,> init_pair]*
   */
  KKAST_VAR_STATMENT,
  KKAST_LET_STATMENT,
  KKAST_CONST_STATMENT,
  /**
   * block:(n)
   *  <{> statment* <}>
   */
  KKAST_BLOCK,
  /**
   * statment:(1)
   *  if_statment
   *  while_statment
   *  do_while_statment
   *  var_statment <;>
   *  let_statment <;>
   *  const_statment <;>
   *  block
   *  <;>
   */
  KKAST_STATMENT,
};

#include "kkdep.h"
#include "kkstr.h"

typedef struct kkast_head_s {
  kkuint8_t isconst : 1;
  kkuint8_t istok : 1;
  kkuint8_t type : 6;
} kkast_head_t;

typedef struct kkast_data_s {
  kkast_head_t head;
  union {
    kkuint64_t inum;
    kkf64_t fnum;
    kkstr_t *str;
  } u;
} kkast_data_t;

typedef struct kkast_common_s kkast_common_t;

struct kkast_common_s {
  kkast_head_t head;
  kksize_t line;
  kksize_t length;
  kkast_common_t *children[0];
};

#include "kklex.h"

typedef struct kkparse_s {
  kklex_t lex;
  kksize_t top, size;
  kkast_common_t *stack;
} kkparse_t;



#endif