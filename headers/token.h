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
    Print = 0,
    PrintS,
    PrintC,
    Alloc,
    Load,
    Store,
    Divmod,
    Mult,
    Minus,
    Plus,
    Not,
    And,
    Or,
    Xor,
    Shl,
    Shr,
    Gt,
    Lt,
    Ge,
    Le,
    Eq,
    Ne,
    Drop,
    Swap,
    Rot,
    Over,
    Dup,
    CastInt,
    CastPtr,
    CastChar,
    VarAccess,
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
    Chr,
} DataType;

typedef union
{
     // todo
} TokenValue;

typedef struct
{
    TokenType type;
    TokenValue value;
    unsigned int line;
    unsigned int pos;
} Token;

Token Token_New(TokenType, TokenValue, unsigned int, unsigned int);


#endif
