#include "../headers/token.h"

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

int MatchIntrinsic(char *input, Intrinsic *out)
{
    return 0;
}
