#ifndef TOKEN_H
#define TOKEN_H

typedef enum
{
    Number = 0,
    String,
    Ident,
    None,
    Operator,
} TokenType;

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
