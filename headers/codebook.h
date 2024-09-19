#ifndef CODEBOOK_H
#define CODEBOOK_H

#include "token.h"
#include <stddef.h>

void interp(Token *);
Token *parse(const char *, size_t *);
char **split(const char *, size_t *);

int IsNumber(char *, long *);

typedef struct 
{
    long val;
    DataType type;
} Value;

Value NewValue(DataType, long);

typedef struct
{
    Value *array;
    int top;
    size_t cap;
    size_t size;
} DataStack;

DataStack DS_New();
void DS_Free(DataStack *);
void DS_Resize(DataStack *);
void DS_Push(DataStack *, long, DataType);
int DS_Pop(DataStack *, Value *);
int DS_Peek(DataStack *, Value *);
void DS_Clear(DataStack *);

typedef struct
{
    const char *key;
    int isConst;
    int free;
    void *value;
} KVPair;

KVPair KVP_New(const char *, void *, int);
KVPair KVP_Default();
void KVP_Free(KVPair *);
int KVP_Equals(KVPair, KVPair);

typedef struct
{
    KVPair *table;
    size_t cap;
    size_t size;
} VariableTableCB;

VariableTableCB VT_New();
unsigned long hash(const char *);
size_t clamp(unsigned long, size_t);
void VT_Free(VariableTableCB *);
void VT_Resize(VariableTableCB *);
void VT_Put(VariableTableCB *, const char *, void *);
void *VT_Get(VariableTableCB *, const char *);
void VT_Remove(VariableTableCB *, const char *);
int VT_ContainsKey(VariableTableCB *, const char *);
char **VT_Keys(const VariableTableCB *, size_t *);

typedef struct
{
    DataStack *numberStack;
    VariableTableCB *variables;
} EnvironmentCB;

EnvironmentCB Env_New();
void Env_Free(EnvironmentCB *);
void Env_DeclVar(const char *, void *);


#endif // CODEBOOK_H
