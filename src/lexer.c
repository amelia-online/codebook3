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
  return (Lexer)
  {
    .buf = Buffer_Make(text),
    .line = 1,
    .col = 1
  };
}
