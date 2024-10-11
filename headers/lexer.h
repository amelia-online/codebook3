#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    char *str;
    size_t cap;
    size_t size; // not including null terminator.
} Buffer;

void BufferGrow(Buffer *);
Buffer Buffer_Make(char *);
void Buffer_Clear(Buffer *);
void Buffer_Push(Buffer *, const char *);
bool Buffer_Get(const Buffer *, size_t, char *);
bool Buffer_GetRange(const Buffer *, size_t, size_t, char **);

typedef struct {
    Buffer buf;
    size_t line;
    size_t col;
} Lexer;

Lexer *Lexer_New(char *);
Lexer Lexer_Make(char *);
bool Lexer_HasNext(const Lexer *);
bool Lexer_HasNextLine(const Lexer *);
bool Lexer_HasNextChar(const Lexer *);
char *Lexer_Next(Lexer *);
char Lexer_ChopLeft(Lexer *);
char Lexer_ChopRight(Lexer *);
void Lexer_SkipLine(Lexer *);
char *Lexer_NextLine(Lexer *);


#endif // LEXER_H
