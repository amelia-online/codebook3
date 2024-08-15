#ifndef CODEBOOK_H
#define CODEBOOK_H

#include "token.h"
#include <stddef.h>

Token *lex(const char *, size_t *);
char **split(const char *, size_t *);

typedef struct
{
    double *array;
    int top;
    size_t cap;
    size_t size;
} DoubleStackCB;

DoubleStackCB DS_New();
void DS_Free(DoubleStackCB *);
void DS_Resize(DoubleStackCB *);
void DS_Push(DoubleStackCB *, double);
int DS_Pop(DoubleStackCB *, double *);
int DS_Peek(DoubleStackCB *, double *);
void DS_Clear(DoubleStackCB *);

typedef struct
{
    char **array;
    int top;
    size_t cap;
    size_t size;
} StringStackCB;

StringStackCB SS_New();
void SS_Free(StringStackCB *);
void SS_Clear(StringStackCB *);
void SS_Push(StringStackCB *, char *);
void SS_Resize(StringStackCB *);
char *SS_Pop(StringStackCB *);
char *SS_Peek(StringStackCB *);

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
    DoubleStackCB *numberStack;
    StringStackCB *stringStack;
    VariableTableCB *variables;
} EnvironmentCB;

EnvironmentCB Env_New();
void Env_Free(EnvironmentCB *);
void Env_DeclVar(const char *, void *);


#endif // CODEBOOK_H
