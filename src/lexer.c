#include "../headers/lexer.h"
#include <stdlib.h>
#include <string.h>

Buffer Buffer_Make(char *text)
{
  char *buf = malloc(strlen(text)+1);
  strcpy(buf, text);
    // buf[strlen(text)] = '\0';
  return (Buffer)
  {
    .str = buf,
    .cap = (strlen(text)+1) + ((strlen(text)+1) % 2),
    .size = strlen(text),
  };
}

void BufferGrow(Buffer *buf)
{
  if (!buf)
    return;

  buf->cap *= 2;
  buf->str = realloc(buf->str, buf->cap);
}

void Buffer_Clear(Buffer *buf)
{
  if (!buf)
    return;

  free(buf->str);
  buf->cap = 4;
  buf->size = 0;
  buf->str = NULL;
}

void Buffer_Push(Buffer *buf, const char *text)
{
  if (!buf)
    return;

  size_t newsize = strlen(text) + buf->size;
  //buffer->size += strlen(text);
  //buffer->cap = newsize + (newsize % 2);
  while (newsize >= buf->cap)
    BufferGrow(buf);

  strcpy(buf->str+buf->size, text);
}

// Text is assumed to be previously malloc'd/
bool Buffer_GetRange(const Buffer *buf, size_t start, size_t end, char **text)
{
  if (!buf || start < 0 || start >= buf->size || end >= buf->size || start > end
      || end < 0)
    return false;

  strncpy(*text, buf->str+start, end-start);
  
  return true;
}

bool Buffer_Get(const Buffer *buf, size_t index, char *text)
{
  if (!buf || index >= buf->size)
    return false;

  *text = buf->str[index];

  return true;
}

Lexer *Lexer_New(char *text)
{
  Lexer *res = malloc(sizeof(Lexer));
  *res = Lexer_Make(text);
  return res;
}

Lexer Lexer_Make(char *text)
{
  size_t numlines = 0;
  
  Buffer buf = Buffer_Make(text);
  for (size_t i = 0; i < buf.size; i++)
    if (buf.str[i] == '\n')
      numlines++;
  
  return (Lexer)
  {
    .buf = buf,
    .line = 1,
    .col = 1,
    .pos = 0,
    .numlines = numlines,
  };
}

bool Lexer_HasNext(const Lexer *lex)
{
  return lex->pos < lex->buf.size;
}

bool Lexer_HasNextLine(const Lexer *lex)
{
  return lex->line < lex->numlines;
}

char *Lexer_Next(Lexer *lex)
{
  // TODO: ...
}

char *Lexer_NextLine(Lexer *lex)
{

}

char Lexer_ChopLeft(Lexer *lex)
{

}

char Lexer_ChopRight(Lexer *lex)
{

}

void Lexer_SkipLine(Lexer *lex)
{

}


