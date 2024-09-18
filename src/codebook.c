#include "../headers/codebook.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Token *parse(const char *input, size_t *size)
{
    return NULL;
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

DataStack DS_New()
{
    return (DataStack)
    {
      .array = malloc(sizeof(long)*64),
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
    ds->array = realloc(ds->array, sizeof(long)*ds->cap);
}

void DS_Push(DataStack *ds, long d)
{
    if (ds->size+1 > ds->cap)
        DS_Resize(ds);

    ds->top++;
    ds->array[ds->top] = d;
    ds->size++;
}

int DS_Pop(DataStack *ds, long *d)
{
    if (ds->top == -1)
        return 0;

    *d = ds->array[ds->top];
    ds->top--;
    ds->size--;
    return 1;
}

int DS_Peek(DataStack *ds, long *d)
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
KVPair KVP_New(const char *key, void *value, int isConst)
{
    return (KVPair)
    {
        .isConst = isConst,
        .free = 0,
        .key = key,
        .value = value,
    };
}

KVPair KVP_Default()
{
    return (KVPair)
    {
        .isConst = 0,
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
    return kvp1.free == kvp2.free && kvp1.isConst == kvp2.isConst && kvp1.key == kvp2.key && kvp1.value == kvp2.value;
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

     KVPair pair = KVP_New(key, value, 0);
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

EnvironmentCB Env_New()
{
    return (EnvironmentCB)
    {
        .numberStack = malloc(sizeof(DataStack)),
        .variables = malloc(sizeof(VariableTableCB)),
    };
}

void Env_Free(EnvironmentCB *env)
{
    free(env->numberStack);
    free( env->variables );
}
