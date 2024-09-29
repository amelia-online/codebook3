#ifndef CODEBOOK_H
#define CODEBOOK_H

#include "token.h"
#include <stddef.h>

Token *parse(const char *, size_t *);
char **split(const char *, size_t *);
char **splitln(char *, size_t *);
char **splitBy(const char *, char *, size_t *);

int StrStartsWith(char *str, char *pat, char **rem);
int StrEndsWith(char *, char *, char **);

int IsZero(char *);
int IsNumber(char *, long *);
int IsHex(char *, long *);

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
int DS_Swap(DataStack *);
int DS_Dup(DataStack *);
int DS_Rot(DataStack *);
int DS_Drop(DataStack *);
int DS_Over(DataStack *);

typedef struct
{
    const char *key;
    int free;
    void *value;
} KVPair;

KVPair KVP_New(const char *, void *);
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
} Environment;

void DoOp(Intrinsic, Environment *);

Environment Env_New();
void Env_Free(Environment *);
void Env_DeclVar(const char *, void *);

void interp(Token *, size_t, Environment *);

#endif // CODEBOOK_H
