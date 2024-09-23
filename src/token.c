#include "../headers/token.h"
#include <string.h>

#define MATCH(IN, STR) if(!strcmp(IN, STR))
#define MATCH_INTR(IN, STR, INTR, OUT) if (!strcmp(IN, STR)) \
{ \
    *OUT = INTR; \
    return 1; \
}

Token Token_New(TokenType type, TokenValue value, unsigned int line, unsigned int pos)
{
    return (Token)
    {
        .type = type,
        .value = value,
        .line = line,
        .pos = pos,
    };
}

/*
    Print,     // .
    PrintS,    // .s
    PrintC,    // .c
    PrintH,    // .h
    Alloc,     // %
    Load,      // @
    Store,     // #
    AddrOf,    // &
    Divmod,    // divmod
    Mult,      // *
    Minus,     // -
    Plus,      // +
    Not,       // not
    And,       // and
    Or,        // or
    Xor,       // xor
    Shl,       // shl
    Shr,       // shr
    Gt,        // >
    Lt,        // <
    Ge,        // >=
    Le,        // <=
    Eq,        // =
    Ne,        // !=
    Drop,      // drop
    Swap,      // swap
    Rot,       // rot
    Over,      // over
    Dup,       // dup
    CastInt,   // ->Int
    CastPtr,   // ->Ptr
    VarAccess, // ?
    StringEq,  // s=
    StringSW,  // ?s
    StringEW,  // s?
    StringAdd, // s+
    StringF,   // s,
*/
int MatchIntrinsic(char *input, Intrinsic *out)
{
    MATCH_INTR(input, ".", Print, out)
    MATCH_INTR(input, ".s", PrintS, out)
    MATCH_INTR(input, ".h", PrintH, out)
    MATCH_INTR(input, ".c", PrintC, out)
    MATCH_INTR(input, "%", Alloc, out)
    MATCH_INTR(input, "@", Load, out)
    MATCH_INTR(input, "#", Store, out)
    MATCH_INTR(input, "&", AddrOf, out)
    MATCH_INTR(input, "divmod", Divmod, out)
    MATCH_INTR(input, "*", Mult, out)
    MATCH_INTR(input, "+", Plus, out)
    MATCH_INTR(input, "-", Minus, out)
    MATCH_INTR(input, "not", Not, out)
    MATCH_INTR(input, "and", And, out)
    MATCH_INTR(input, "or", Or, out)
    MATCH_INTR(input, "xor", Xor, out)
    MATCH_INTR(input, "shl", Shl, out)
    MATCH_INTR(input, "shr", Shr, out)
    MATCH_INTR(input, ">", Gt, out)
    MATCH_INTR(input, "<", Lt, out)
    MATCH_INTR(input, ">=", Ge, out)
    MATCH_INTR(input, "<=", Le, out)
    MATCH_INTR(input, "=", Eq, out)
    MATCH_INTR(input, "!=", Ne, out)
    MATCH_INTR(input, "drop", Drop, out)
    MATCH_INTR(input, "swap", Swap, out)
    MATCH_INTR(input, "rot", Rot, out)
    MATCH_INTR(input, "over", Over, out)
    MATCH_INTR(input, "dup", Dup, out)
    MATCH_INTR(input, "->Int", CastInt, out)
    MATCH_INTR(input, "->Ptr", CastPtr, out)
    MATCH_INTR(input, "?", VarAccess, out)
    MATCH_INTR(input, "s=", StringEq, out)
    MATCH_INTR(input, "?s", StringSW, out)
    MATCH_INTR(input, "s?", StringEW, out)
    MATCH_INTR(input, "s+", StringAdd, out)
    MATCH_INTR(input, "s,", StringF, out)
    return 0;
}
