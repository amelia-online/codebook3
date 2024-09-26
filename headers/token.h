#ifndef TOKEN_H
#define TOKEN_H

typedef enum
{
    Number = 0,
    Pointer,
    String,
    Ident,
    None,
    Operator,
    FnDef,
    WhileLoop,
    IfStatement,
    TimesLoop,
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
    ReadS,     // ,s
    ReadI,     // ,i
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

int MatchIntrinsic(char *, Intrinsic *);

typedef enum 
{
    End = 0,
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
    Int = 0,
    Ptr,
} DataType;

typedef union
{
    long integer;
    Intrinsic intr;
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
