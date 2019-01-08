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

  KKTOK_ADD,        // +
  KKTOK_ADD_EQ,     // +=
  KKTOK_ARROW,      // =>
  KKTOK_BAND_EQ,    // &=
  KKTOK_BIT_AND,    // &
  KKTOK_BIT_NOT,    // ~
  KKTOK_BIT_OR,     // |
  KKTOK_BIT_XOR,    // ^
  KKTOK_BOR_EQ,     // |=
  KKTOK_BRACE_L,    // {
  KKTOK_BRACE_R,    // }
  KKTOK_BRACKET_L,  // [
  KKTOK_BRACKET_R,  // ]
  KKTOK_BXOR_EQ,    // ^=
  KKTOK_COLON,      // :
  KKTOK_COM,        // ,
  KKTOK_DEC,        // --
  KKTOK_DIV,        // /
  KKTOK_DIV_EQ,     // /=
  KKTOK_DOT,        // .
  KKTOK_ELLIPSIS,   // ...
  KKTOK_EQ,         // ==
  KKTOK_EQ_STRICT,  // ===
  KKTOK_GE,         // >=
  KKTOK_GT,         // >
  KKTOK_INC,        // ++
  KKTOK_LE,         // <=
  KKTOK_LOGIC_AND,  // &&
  KKTOK_LOGIC_NOT,  // !
  KKTOK_LOGIC_OR,   // ||
  KKTOK_LT,         // <
  KKTOK_MOD,        // %
  KKTOK_MOD_EQ,     // %=
  KKTOK_MUL,        // *
  KKTOK_MUL_EQ,     // *=
  KKTOK_NE,         // !=
  KKTOK_NE_STRICT,  // !==
  KKTOK_PAREN_L,    // (
  KKTOK_PAREN_R,    // )
  KKTOK_QUESTION,   // ?
  KKTOK_SAR,        // >>
  KKTOK_SAR_EQ,     // >>=
  KKTOK_SEM,        // ;
  KKTOK_SHL,        // <<
  KKTOK_SHL_EQ,     // <<=
  KKTOK_SHR,        // >>>
  KKTOK_SHR_EQ,     // >>>=
  KKTOK_SUB,        // -
  KKTOK_SUB_EQ,     // -=
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
  G(KKTOK_WORD_GROUP_A)           \
  G(KKTOK_WORD_GROUP_B)           \
  G(KKTOK_WORD_GROUP_C)           \
  G(KKTOK_WORD_GROUP_D)           \
  G(KKTOK_WORD_GROUP_E)           \
  G(KKTOK_WORD_GROUP_F)           \
  G(KKTOK_WORD_GROUP_G)           \
  G(KKTOK_WORD_GROUP_I)           \
  G(KKTOK_WORD_GROUP_L)           \
  G(KKTOK_WORD_GROUP_N)           \
  G(KKTOK_WORD_GROUP_O)           \
  G(KKTOK_WORD_GROUP_R)           \
  G(KKTOK_WORD_GROUP_S)           \
  G(KKTOK_WORD_GROUP_T)           \
  G(KKTOK_WORD_GROUP_V)           \
  G(KKTOK_WORD_GROUP_W)           \
  G(KKTOK_WORD_GROUP_Y)           \


#endif
