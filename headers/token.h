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

} TokenType;

typedef enum 
{
    Str = 0,
    Int,
    Ptr,
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
