#ifndef TOKEN_H
#define TOKEN_H

typedef enum
{
    Number = 0,
    String,
    Ident,
    None,
    Operator,
    FnDef,
    WhileLoop,
    IfStatement,
    TimesLoop,
    VarDefn,
    ConstDefn,
    Pop,
    Use,
} TokenType;

typedef enum 
{
    Print = 0, // .
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
} Intrinsic;

int MatchIntrinisc(char *, Intrinsic *);

typedef enum 
{
    Var = 0,
    End,
    Const,
    While,
    If,
    Endif,
    Elseif,
    Else,
    Do,
    Times,
} Keywords;

typedef enum 
{
    Str = 0,
    Int,
    Ptr,
} DataType;

typedef union
{
    long integer;
    void *pointer;
} TokenValue;

typedef struct
{
    TokenType type;
    TokenValue value;
    unsigned int line;
    unsigned int pos;
} Token;

typedef struct
{
    char *symbol;
    Token *body;
} Expr_VarDefn;

typedef struct
{
    char *name;
    Token *body;
} Expr_FnDef;


Token Token_New(TokenType, TokenValue, unsigned int, unsigned int);


#endif
