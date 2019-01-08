#ifndef KKTOK_H
#define KKTOK_H

enum {
  KKTOK_EOF,        // <EOF>
  KKTOK_STRING,     // <string>
  KKTOK_REGEXP,     // <regexp>
  KKTOK_ID,         // <id>
  KKTOK_INT,        // <int>
  KKTOK_FLOAT,      // <float>

  KKTOK_ARGUMENTS,  // arguments
  KKTOK_ASYNC,      // async
  KKTOK_AWAIT,      // await
  KKTOK_BREAK,      // break
  KKTOK_CASE,       // case
  KKTOK_CATCH,      // catch
  KKTOK_CLASS,      // class
  KKTOK_CONST,      // const
  KKTOK_CONSTRUCTOR,// constructor
  KKTOK_CONTINUE,   // continue
  KKTOK_DEFAULT,    // default
  KKTOK_DELETE,     // delete
  KKTOK_DO,         // do
  KKTOK_ELSE,       // else
  KKTOK_EXPORT,     // export
  KKTOK_EXTENDS,    // extends
  KKTOK_FALSE,      // false
  KKTOK_FINALLY,    // finally
  KKTOK_FOR,        // for
  KKTOK_FUNCTION,   // function
  KKTOK_GET,        // get
  KKTOK_IF,         // if
  KKTOK_IMPORT,     // import
  KKTOK_IN,         // in
  KKTOK_INSTANCEOF, // instanceof
  KKTOK_LET,        // let
  KKTOK_NEW,        // new
  KKTOK_NULL,       // null
  KKTOK_OF,         // of
  KKTOK_RETURN,     // return
  KKTOK_SET,        // set
  KKTOK_STATIC,     // static
  KKTOK_SUPER,      // super
  KKTOK_SWITCH,     // switch
  KKTOK_THIS,       // this
  KKTOK_THROW,      // throw
  KKTOK_TRUE,       // true
  KKTOK_TRY,        // try
  KKTOK_TYPEOF,     // typeof
  KKTOK_VAR,        // var
  KKTOK_VOID,       // void
  KKTOK_WHILE,      // while
  KKTOK_YIELD,      // yield

  KKTOK_BIT_NOT,    // ~
  KKTOK_BRACE_R,    // }
  KKTOK_LOGIC_OR,   // ||
  KKTOK_BOR_EQ,     // |=
  KKTOK_BIT_OR,     // |
  KKTOK_BRACE_L,    // {
  KKTOK_BXOR_EQ,    // ^=
  KKTOK_BIT_XOR,    // ^
  KKTOK_BRACKET_R,  // ]
  KKTOK_BRACKET_L,  // [
  KKTOK_QUESTION,   // ?
  KKTOK_SHR_EQ,     // >>>=
  KKTOK_SHR,        // >>>
  KKTOK_SAR_EQ,     // >>=
  KKTOK_SAR,        // >>
  KKTOK_GE,         // >=
  KKTOK_GT,         // >
  KKTOK_ARROW,      // =>
  KKTOK_EQ_STRICT,  // ===
  KKTOK_EQ,         // ==
  KKTOK_ASSIGN,     // =
  KKTOK_LE,         // <=
  KKTOK_SHL_EQ,     // <<=
  KKTOK_SHL,        // <<
  KKTOK_LT,         // <
  KKTOK_SEM,        // ;
  KKTOK_COLON,      // :
  KKTOK_DIV_EQ,     // /=
  KKTOK_DIV,        // /
  KKTOK_ELLIPSIS,   // ...
  KKTOK_DOT,        // .
  KKTOK_SUB_EQ,     // -=
  KKTOK_DEC,        // --
  KKTOK_SUB,        // -
  KKTOK_COM,        // ,
  KKTOK_ADD_EQ,     // +=
  KKTOK_INC,        // ++
  KKTOK_ADD,        // +
  KKTOK_MUL_EQ,     // *=
  KKTOK_MUL,        // *
  KKTOK_PAREN_R,    // )
  KKTOK_PAREN_L,    // (
  KKTOK_BAND_EQ,    // &=
  KKTOK_LOGIC_AND,  // &&
  KKTOK_BIT_AND,    // &
  KKTOK_MOD_EQ,     // %=
  KKTOK_MOD,        // %
  KKTOK_NE_STRICT,  // !==
  KKTOK_NE,         // !=
  KKTOK_LOGIC_NOT,  // !
};


#define KKTOK_WORD_GROUP_A(T)     \
  T(ARGUMENTS,     "arguments")   \
  T(ASYNC,         "async")       \
  T(AWAIT,         "await")       \

#define KKTOK_WORD_GROUP_B(T)     \
  T(BREAK,         "break")       \

#define KKTOK_WORD_GROUP_C(T)     \
  T(CASE,          "case")        \
  T(CATCH,         "catch")       \
  T(CLASS,         "class")       \
  T(CONST,         "const")       \
  T(CONSTRUCTOR,   "constructor") \
  T(CONTINUE,      "continue")    \

#define KKTOK_WORD_GROUP_D(T)     \
  T(DEFAULT,       "default")     \
  T(DELETE,        "delete")      \
  T(DO,            "do")          \

#define KKTOK_WORD_GROUP_E(T)     \
  T(ELSE,          "else")        \
  T(EXPORT,        "export")      \
  T(EXTENDS,       "extends")     \

#define KKTOK_WORD_GROUP_F(T)     \
  T(FALSE,         "false")       \
  T(FINALLY,       "finally")     \
  T(FOR,           "for")         \
  T(FUNCTION,      "function")    \

#define KKTOK_WORD_GROUP_G(T)     \
  T(GET,           "get")         \

#define KKTOK_WORD_GROUP_I(T)     \
  T(IF,            "if")          \
  T(IMPORT,        "import")      \
  T(IN,            "in")          \
  T(INSTANCEOF,    "instanceof")  \

#define KKTOK_WORD_GROUP_L(T)     \
  T(LET,           "let")         \

#define KKTOK_WORD_GROUP_N(T)     \
  T(NEW,           "new")         \
  T(NULL,          "null")        \

#define KKTOK_WORD_GROUP_O(T)     \
  T(OF,            "of")          \

#define KKTOK_WORD_GROUP_R(T)     \
  T(RETURN,        "return")      \

#define KKTOK_WORD_GROUP_S(T)     \
  T(SET,           "set")         \
  T(STATIC,        "static")      \
  T(SUPER,         "super")       \
  T(SWITCH,        "switch")      \

#define KKTOK_WORD_GROUP_T(T)     \
  T(THIS,          "this")        \
  T(THROW,         "throw")       \
  T(TRUE,          "true")        \
  T(TRY,           "try")         \
  T(TYPEOF,        "typeof")      \

#define KKTOK_WORD_GROUP_V(T)     \
  T(VAR,           "var")         \
  T(VOID,          "void")        \

#define KKTOK_WORD_GROUP_W(T)     \
  T(WHILE,         "while")       \

#define KKTOK_WORD_GROUP_Y(T)     \
  T(YIELD,         "yield")       \

#define KKTOK_WORD_GROUPS(G)      \
  G(KKTOK_WORD_GROUP_A, 'a')      \
  G(KKTOK_WORD_GROUP_B, 'b')      \
  G(KKTOK_WORD_GROUP_C, 'c')      \
  G(KKTOK_WORD_GROUP_D, 'd')      \
  G(KKTOK_WORD_GROUP_E, 'e')      \
  G(KKTOK_WORD_GROUP_F, 'f')      \
  G(KKTOK_WORD_GROUP_G, 'g')      \
  G(KKTOK_WORD_GROUP_I, 'i')      \
  G(KKTOK_WORD_GROUP_L, 'l')      \
  G(KKTOK_WORD_GROUP_N, 'n')      \
  G(KKTOK_WORD_GROUP_O, 'o')      \
  G(KKTOK_WORD_GROUP_R, 'r')      \
  G(KKTOK_WORD_GROUP_S, 's')      \
  G(KKTOK_WORD_GROUP_T, 't')      \
  G(KKTOK_WORD_GROUP_V, 'v')      \
  G(KKTOK_WORD_GROUP_W, 'w')      \
  G(KKTOK_WORD_GROUP_Y, 'y')      \


#endif
