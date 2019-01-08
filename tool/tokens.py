

tok_complexs = [
    "EOF",
    "string",
    "regexp",
    "id",
    "int",
    "float",
]

tok_words = [
    "class",
    "super",
    "this",
    "extends",
    "static",
    "constructor",
    "get",
    "set",
    "new",
    "delete",
    "if",
    "else",
    "for",
    "in",
    "of",
    "do",
    "while",
    "break",
    "continue",
    "switch",
    "case",
    "default",
    "return",
    "try",
    "catch",
    "finally",
    "throw",
    "function",
    "arguments",
    "async",
    "await",
    "instanceof",
    "typeof",
    "void",
    "yield",
    "var",
    "let",
    "const",
    "import",
    "export",
    "true",
    "false",
    "null",
]
tok_symbols = [
    ("assign",     "="),
    ("add",        "+"),
    ("sub",        "-"),
    ("div",        "/"),
    ("mul",        "*"),
    ("mod",        "%"),
    ("shl",        "<<"),
    ("sar",        ">>"),
    ("shr",        ">>>"),
    ("bit_and",    "&"),
    ("bit_or",     "|"),
    ("bit_xor",    "^"),
    ("bit_not",    "~"),
    ("add_eq",     "+="),
    ("sub_eq",     "-="),
    ("div_eq",     "/="),
    ("mul_eq",     "*="),
    ("mod_eq",     "%="),
    ("shl_eq",     "<<="),
    ("sar_eq",     ">>="),
    ("shr_eq",     ">>>="),
    ("band_eq",    "&="),
    ("bor_eq",     "|="),
    ("bxor_eq",    "^="),
    ("logic_and",  "&&"),
    ("logic_or",   "||"),
    ("logic_not",  "!"),
    ("lt",         "<"),
    ("le",         "<="),
    ("gt",         ">"),
    ("ge",         ">="),
    ("eq",         "=="),
    ("eq_strict",  "==="),
    ("ne",         "!="),
    ("ne_strict",  "!=="),
    ("inc",        "++"),
    ("dec",        "--"),
    ("arrow",      "=>"),
    ("paren_l",    "("),
    ("paren_r",    ")"),
    ("brace_l",    "{"),
    ("brace_r",    "}"),
    ("bracket_l",  "["),
    ("bracket_r",  "]"),
    ("dot",        "."),
    ("com",        ","),
    ("sem",        ";"),
    ("colon",      ":"),
    ("question",   "?"),
    ("ellipsis",   "..."),
]


def padding_end(s, n):
    return s + (' ' * (n - len(s)))

# gen token type enum


def expand_item(name, params, padding):
    src = ['  ', name, '(']
    for i in range(len(params)-1):
        src.append(padding_end("%s," % params[i], padding[i]))
    src.append(padding_end("%s)" % params[-1], padding[-1]))
    return "".join(src) + '\\'


print("#ifndef KKTOK_H")
print("#define KKTOK_H\n")

print("enum {")
for i in tok_complexs:
    s = ("  kktok_" + i + ",").upper()
    print(padding_end(s, 20)+("// <%s>" % i))

print("")
tok_words.sort()
for i in tok_words:
    s = ("  kktok_" + i + ",").upper()
    print(padding_end(s, 20)+("// %s" % i))


def symbol_compare(a, b):
    for i in range(min(len(a), len(b))):
        if a[i] == b[i]:
            continue
        elif a[i] < b[i]:
            return 1
        else:
            return -1
    if len(a) < len(b):
        return 1
    else:
        return -1


print("")
tok_symbols.sort(lambda x, y: symbol_compare(x[1], y[1]))
for i in tok_symbols:
    s = ("  kktok_" + i[0] + ",").upper()
    print(padding_end(s, 20)+("// %s" % i[1]))

print("};\n")

ch = 0
first = []
for w in tok_words:
    if w[0] != ch:
        ch = w[0]
        first.append(ch)
        print("\n#define KKTOK_WORD_GROUP_%s(T)     \\" % ch.upper())
    print(expand_item('T', ["%s" % w.upper(), '"%s"' % w], [15, 15]))

print("\n#define KKTOK_WORD_GROUPS(G)      \\")
for w in first:
    print(expand_item('G', ['KKTOK_WORD_GROUP_%s' %
                            w.upper(), "'%s'" % w[0]], [20, 10]))


print("\n\n#endif")
