#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    char *str;
    size_t cap;
    size_t size;
} Buffer;

void BufferGrow(Buffer *);
Buffer Buffer_New();
void Buffer_Push(Buffer *, const char *);
bool Buffer_Get(const Buffer *, size_t, char *);

typedef struct {
    Buffer buf;
    size_t line;
    size_t col;
} Lexer;

Lexer Lexer_New(char *);
bool Lexer_HasNext(const Lexer *);
bool Lexer_Next(Lexer *, char **);

#endif