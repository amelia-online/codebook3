#include "../headers/codebook.h"
#include "../headers/token.h"
#include "../headers/lexer.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define MATCH(STR, PAT) if(!strcmp(STR, PAT))
#define MATCHOP(OP) if (intr == OP)
#define RANGE(START, END) for (int i = START; i < END; i++)

Token *parse(const char *input, size_t *size)
{
  Lexer *lexer = Lexer_New(input);
  Token *tokens = malloc(sizeof(Token)*8);
  *size = 0;
  
  while (Lexer_HasNext(lexer))
  {
    
  }

  
  free(lexer);
  return tokens;
}

/*
Token *parse(const char *input, size_t *size)
{
    Lexer *lexer = Lexer_New(input);
  
    if (!strcmp("", input) || !input)
    {
        *size = 0;
        return NULL;
    }

    Token *tokens = (Token *)malloc(sizeof(Token)*8);
    size_t cap = 8;
    *size = 0;

    size_t linenum = 0;
    size_t col_pos = 0;

    size_t numlines;
    char **lines = splitln(input, &numlines);

    for (int i = 0; i < numlines; i++)
    {
        linenum += 1;
        size_t numTokens;
        char **lineTokens = split(lines[i], &numTokens);

        for (int j = 0; j < numTokens; j++)
        {
            col_pos += 1;
            char *rem = malloc(100);
            if (StrStartsWith(lineTokens[j], "//", &rem)) { free(rem); break;}
            free(rem);
            // Yeah I know this looks bad.

			double fl;
			if (IsFloat(lineTokens[j], &fl))
			{
			  
			}
	    
            long num;
            if (IsHex(lineTokens[j], &num))
            {
                TokenValue val;
                val.integer = num;
                Token t = Token_New(Number, val, linenum, col_pos++);
                tokens[*size] = t;
                *size += 1;
                continue;
            } 
            else if (IsNumber(lineTokens[j], &num))
            {
                TokenValue val;
                val.integer = num;
                Token t = Token_New(Number, val, linenum, col_pos++);
                tokens[*size] = t;
                *size += 1;
                continue;
            }
            
            Intrinsic intr;
            if (MatchIntrinsic(lineTokens[j], &intr))
            {
                TokenValue val;
                val.intr = intr;
                Token t = Token_New(Operator, val, linenum, col_pos++);
                tokens[*size] = t;
                *size += 1;
                continue;
            }

            MATCH (input, "const") 
            {
               // TODO...
            }

        }

        col_pos = 0;

        for (int idx = 0; idx < numTokens; idx++)
            free(lineTokens[idx]);
        free(lineTokens);

        if ((*size) + 1 >= cap) { cap *= 2; tokens = realloc(tokens, sizeof(Token)*cap); }

    }

    for (int i = 0; i < numlines; i++)
        free(lines[i]);
    free(lines);
    free(lexer);
    return tokens;
}

*/

/*
typedef struct
{
    TokenType type;
    TokenValue value;
    unsigned int line;
    unsigned int pos;
} Token;
*/
void interp(Token *tokens, size_t size, Environment *env)
{
    for (size_t i = 0; i < size; i++)
    {
        Token t  = tokens[i];

        if (t.type == Number)
        {
            DS_Push(env->numberStack, t.value.integer, Int);
        }

        if (t.type == Operator)
        {
            DoOp(t.value.intr, env);
            continue;
        }
    }
}

void DoOp(Intrinsic intr, Environment *env)
{
    MATCHOP(Print) // .
    {
        Value v;
        if (DS_Pop(env->numberStack, &v))
        {
            if (v.type)

            printf("%ld", v.val);
        }
        else
        {

        }
    }

    MATCHOP(PrintH) // .h
    {
        Value v;
        if (DS_Pop(env->numberStack, &v))
        {
            printf("0x%X", v.val);
        }
        else
        {

        }
    }

    MATCHOP(PrintC) // .c
    {
        Value v;
        if (DS_Pop(env->numberStack, &v))
        {
            printf("%c", v.val % 256);
        }
        else
        {

        }
    }

    MATCHOP(ReadS) // Ptr Int
    {
        
    }

    // ...

    MATCHOP(Plus) // Int Int -> Int
    {   
    }


}

// mallocs list and list items, splits by spaces.
char **split(const char *input, size_t *size)
{
    *size = 0;
    if (!input) return NULL;

    char current[100] = "";
    char **list = malloc(sizeof(char *)*8);
    size_t cap = 8;
    
    size_t len = strlen(input);

    for (int i = 0; i < len; i++)
    {
        char ch[2];
        ch[1] = '\0';
        ch[0] = input[i];
        
        if (!strcmp(" ", ch))
        {
            if ((*size) + 1 >= cap) { list = realloc(list, sizeof(char *)*cap * 2); cap *= 2; }

            list[*size] = malloc(sizeof(char)*strlen(current)+1);
            strncpy(list[*size], current, strlen(current)+1);
            *size += 1;
            strcpy(current, "");
            
        } else strcat(current, ch);
            
    }

    if (strcmp("", current))
    {
        list[*size] = malloc(sizeof(char)*strlen(current)+1);
        strncpy(list[*size], current, strlen(current)+1);
        *size += 1;
    }

    for (size_t i = *size; i < cap; i++)
        list[i] = NULL;
    
    return list;
}

char **splitln(char *input, size_t *size)
{
    *size = 0;
    if (!input) return NULL;

    char current[100] = "";
    char **list = malloc(sizeof(char *)*8);
    size_t cap = 8;
    
    size_t len = strlen(input);

    for (int i = 0; i < len; i++)
    {
        char ch[2];
        ch[1] = '\0';
        ch[0] = input[i];
        
        if (!strcmp("\n", ch))
        {
            if ((*size) + 1 >= cap) { list = realloc(list, sizeof(char *)*cap * 2); cap *= 2; }

            list[*size] = malloc(sizeof(char)*strlen(current)+1);
            strncpy(list[*size], current, strlen(current)+1);
            *size += 1;
            strcpy(current, "");
            
        } else strcat(current, ch);
            
    }

    if (strcmp("", current))
    {
        list[*size] = malloc(sizeof(char)*strlen(current)+1);
        strncpy(list[*size], current, strlen(current)+1);
        *size += 1;
    }

    for (size_t i = *size; i < cap; i++)
        list[i] = NULL;
    
    return list;
}

char **splitBy(const char *input, char *item, size_t *size)
{
    *size = 0;
    if (!input) return NULL;

    char current[100] = "";
    char **list = malloc(sizeof(char *)*8);
    size_t cap = 8;
    
    size_t len = strlen(input);

    for (int i = 0; i < len; i++)
    {
        char ch[2];
        ch[1] = '\0';
        ch[0] = input[i];
        
        if (!strcmp(item, ch))
        {
            if ((*size) + 1 >= cap) { list = realloc(list, sizeof(char *)*cap * 2); cap *= 2; }

            list[*size] = malloc(sizeof(char)*strlen(current)+1);
            strncpy(list[*size], current, strlen(current)+1);
            *size += 1;
            strcpy(current, "");
            
        } else strcat(current, ch);
            
    }

    if (strcmp("", current))
    {
        list[*size] = malloc(sizeof(char)*strlen(current)+1);
        strncpy(list[*size], current, strlen(current)+1);
        *size += 1;
    }

    for (size_t i = *size; i < cap; i++)
        list[i] = NULL;
    
    return list;
}

int StrStartsWith(char *str, char *pat, char **rem) // ?s
{
    size_t strLen = strlen(str);
    size_t patLen = strlen(pat);

    if (patLen > strLen)
        return 0;

    int offset = 0;
    for (; offset < patLen; offset++)
        if (str[offset] != pat[offset])
            return 0;
    if (rem)
      *rem = Substr(str, offset, strLen);
    
    return 1;
}

int StrEndsWith(char *str, char *pat, char **rem) // s?
{
    size_t strLen = strlen(str);
    size_t patLen = strlen(pat);

    if (patLen > strLen)
        return 0;

    int offset = strLen - patLen;
    int idx = 0;
    for (; offset < patLen; offset++)
        if (str[offset] != pat[idx++])
            return 0;
    if (rem)
      strncpy(*rem, str, (strLen-patLen)+1);
    return 1;
}

// Includes start, does not include end.
char *Substr(char *str, size_t start, size_t end) // s_
{
    size_t len = end - start;
    if (len <= 0 || len > strlen(str))
        return NULL;

    char *res = malloc(len+1);
    strncpy(res, str+start, len);
    res[len] = '\0';
    return res;
}

int StrIncludes(char *str, char *pat) // s_?
{
    size_t patlen = strlen(pat);
    size_t strLen = strlen(str);

    if (patlen > strLen)
        return 0;
    else if (patlen == strLen)
    {
        if (!strcmp(str, pat))
            return 1;
        return 0;
    }

    int found = 0;
    for (int i = 0; i + patlen < strLen; i++)
    {
        char *slice = Substr(str, i, i + patlen);
        if (!strcmp(slice, pat))
            found = 1;
        free(slice);
    }

    return found;
}

int StrCount(char *str, char *pat)
{
    size_t len = strlen(str);
    size_t plen = strlen(pat);
    int count = 0;
    for (int i = 0; i + plen < len; i++)
    {
        char *slice = Substr(str, i, i + plen);
        if (!strcmp(slice, pat))
            count += 1;
        free(slice);
    }
    return count;
}

int IsZero(char *input)
{
    size_t len = strlen(input);

    for (int i = 0; i < len; i++)
        if (input[i] != '0')
            return 0;
    return 1;
}

int IsFloat(char *input, double *num)
{
    if (!StrIncludes(input, ".") || StrCount(input, ".") > 1)
        return 0;

    size_t len;
    char **parts = splitBy(input, ".", &len);

    int isZero = 1;

    RANGE(0, len)
    {
      if (!IsZero(parts[i]))
	isZero = 0;
      free(parts[i]);
    }
    free(parts);

    if (isZero)
    {
      *num = 0.0;
      return 1;
    }

    errno = 0;
    char *end;

    double res = strtod(input, &end);

    if (!(errno == ERANGE || !res))
    {
        *num = res;
        return 1;
    }

    return 0;
}

int IsNumber(char *input, long *num)
{
    if (IsZero(input))
    {
        *num = 0;
        return 1;
    }

    errno = 0;
    char *end;

    long res = strtol(input, &end, 10);

    if (!(errno == ERANGE || !res))
    {
        *num = res;
        return 1;
    }

    return 0;
}

int IsHex(char *input, long *num)
{
    int inlen = strlen(input);
    int remlen = inlen - 2;
    if (remlen <= 0)
        return 0;
    char *rem = malloc(remlen+1);
    rem[remlen] = '\0';
    if (!(StrStartsWith(input, "0x", &rem)))
    {
        free(rem);
        return 0;
    }
    else if (IsZero(rem))
    {
        free(rem);
        *num = 0;
        return 1;
    }
            
    errno = 0;
    char *end;

    long res = strtol(input, &end, 16);

    if (errno == ERANGE || !res)
    {
        free(rem);
        return 0;
    }
    
    *num = res;
    free(rem);
    return 1;
}

Value NewValue(DataType type, long val)
{
    return (Value)
    {
        .type = type,
        .val = val
    };
}

DataStack DS_New()
{
    return (DataStack)
    {
      .array = malloc(sizeof(Value)*64),
      .size = 0,
      .cap = 64,
      .top = -1,
    };
}

void DS_Free(DataStack *ds)
{
    free(ds->array);
}

void DS_Resize(DataStack *ds)
{
    ds->cap *= 2;
    ds->array = realloc(ds->array, sizeof(Value)*ds->cap);
}

void DS_Push(DataStack *ds, long d, DataType type)
{
    if (ds->size+1 >= ds->cap)
        DS_Resize(ds);

    ds->top++;
    ds->array[ds->top] = NewValue(type, d);
    ds->size++;
}

int DS_Pop(DataStack *ds, Value *d)
{
    if (ds->top == -1)
        return 0;

    *d = ds->array[ds->top];
    ds->top--;
    ds->size--;
    return 1;
}

int DS_Peek(DataStack *ds, Value *d)
{
    if (ds->top == -1)
        return 0;

    *d = ds->array[ds->top];
    return 1;
}

void DS_Clear(DataStack *ds)
{
    DS_Free(ds);
    *ds = DS_New();
}

// Assume value has previously been malloced.
KVPair KVP_New(const char *key, void *value)
{
    return (KVPair)
    {
        .free = 0,
        .key = key,
        .value = value,
    };
}

KVPair KVP_Default()
{
    return (KVPair)
    {
        .free = 1,
        .key = "",
        .value = NULL,
    };
}

void KVP_Free(KVPair *kvp)
{
    free(kvp->value);
}

int KVP_Equals(KVPair kvp1, KVPair kvp2)
{
    return kvp1.free == kvp2.free && kvp1.key == kvp2.key && kvp1.value == kvp2.value;
}

unsigned long hash(const char *key)
{
    unsigned long result;
    char c;
    int multiplier = 1;

    while ((c = *key++))
    {
        result += ((int)c * multiplier++) << 5;
    }

    return result;
}

size_t clamp(unsigned long hash, size_t cap)
{
    return hash % cap;
}

VariableTableCB VT_New()
{
    VariableTableCB vt = (VariableTableCB)
    {
      .table = (KVPair *)malloc(sizeof(KVPair)*32),
      .cap = 32,
      .size = 0,
    };

    for (int i = 0; i < 32; i++)
      vt.table[i] = KVP_Default();

    return vt;
}

void VT_Free(VariableTableCB *vt)
{
    free(vt->table);
}

void VT_Resize(VariableTableCB *vt)
{
    size_t oldcap = vt->cap;
    vt->cap *= 2;
    vt->table = realloc(vt->table, vt->cap);
    for (size_t i = oldcap; i < vt->cap; i++)
        vt->table[i] = KVP_Default();
}

// Assume value has been previously malloc'ed.
void VT_Put(VariableTableCB *vt, const char *key, void *value)
{
    if (vt->size + 1 > vt->cap)
        VT_Resize(vt);

    size_t index = clamp(hash(key), vt->cap);

    while (!vt->table[index].free)
    {
        index++;
        if (index > vt->cap-1)
            index = 0;
    }

    KVPair pair = KVP_New(key, value);
    pair.free = 0;
    vt->table[index] = pair;
    vt->size++;
}

void *VT_Get(VariableTableCB *vt, const char *key)
{
    if (!VT_ContainsKey(vt, key))
        return NULL;

    size_t index = clamp(hash(key), vt->cap);

    while (strcmp(vt->table[index].key, key))
    {
        index++;
        if (index >= vt->cap)
            index = 0;
    }

    return vt->table[index].value;
}

// Returns NULL if there are no keys.
char **VT_Keys(const VariableTableCB *vt, size_t *size)
{
    if (vt->size == 0) { *size = 0; return NULL; }

    char **keys = malloc(sizeof(char *)*vt->size);
    size_t idx = 0;
    size_t found = 0;


    for (size_t i = 0; i < vt->cap; i++)
        if (!vt->table[i].free)
        {
            keys[idx++] = vt->table[i].key;
            found++;
        }

    *size = found;
    return keys;
}

int VT_ContainsKey(VariableTableCB *vt, const char *key)
{
    size_t index = clamp(hash(key), vt->cap);
    size_t start = index;

    while (strcmp(key, vt->table[index++].key))
    {
        if (index == start) return 0;
        if (index >= vt->cap) index = 0;
    }

    return 1;
}

void VT_Remove(VariableTableCB *vt, const char *key)
{
    if (!VT_ContainsKey(vt, key)) return;

    size_t index = clamp(hash(key), vt->cap);

    while (strcmp(vt->table[index].key, key))
    {
        index++;
        if (index >= vt->cap)
            index = 0;
    }

    vt->table[index].free = 1;
    vt->size--;
}

Environment Env_New()
{
    Environment env =  (Environment)
    {
        .numberStack = malloc(sizeof(DataStack)),
        .variables = malloc(sizeof(VariableTableCB)),
    };
    *(env.numberStack) = DS_New();
    *(env.variables)   = VT_New();
    return env;
}

void Env_Free(Environment *env)
{
    free(env->numberStack);
    free( env->variables );
}
