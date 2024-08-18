#include "../headers/codebook.h"
#include "../headers/cbtesting.h"

/*
    Tests begin here.
*/

// Misc.

TEST (SimpleHash)
{
    const char *str1 = "hello";
    const char *str2 = "olleh";
    return hash(str1) != hash(str2);
}

TEST (SimpleHash2)
{
    const char *str1 = "hello";
    const char *str2 = "helol";
    return hash(str1) != hash(str2);
}

TEST (SimpleSplitChar)
{
    const char *sentence = "Here is four words.";

    size_t size;
    char **list = split(sentence, &size);

    ASSERT ((size == 4));

    ASSERT (!strcmp("Here", list[0]));

    ASSERT (!strcmp("is", list[1]));

    ASSERT (!strcmp("four", list[2]));

    ASSERT (!strcmp("words.", list[3]));

    for (int i = 0; i < size; i++)
        free(list[i]);
    free(list);

    PASS
}

// Number Stack

TEST (SimpleNumberStack)
{
    DoubleStackCB stack = DS_New();

    DS_Push(&stack, 1.0);
    DS_Push(&stack, 2.0);
    DS_Push(&stack, 3.0);

    double val;

    if (stack.size != 3)
        FAIL;

    if (!DS_Pop(&stack, &val))
        FAIL;
    if (val != 3.0)
        FAIL;

    if (!DS_Pop(&stack, &val))
        FAIL;
    if (val != 2.0)
        FAIL;

    if (!DS_Pop(&stack, &val))
        FAIL;
    if (val != 1.0)
        FAIL;

    if (stack.size != 0)
        FAIL;

    DS_Free(&stack);
    PASS;
}

void DisposeDS(DoubleStackCB *ds)
{
    DS_Free(ds);
    free(ds);
}

TEST (NumberStackStressTest)
{
    DoubleStackCB *stack = malloc(sizeof(DoubleStackCB));
    *stack = DS_New();

    for (int i = 0; i < 1000000; i++)
        DS_Push(stack, (double)rand());

    if (stack->size != 1000000)
    {
        DisposeDS(stack);
        FAIL
    }

    for (int i = 0; i < 500000; i++)
    {
        double val;
        if (!DS_Pop(stack, &val))
        {
            DisposeDS(stack);
            FAIL
         }
    }

    if (stack->size != 500000)
    {
        DisposeDS(stack);
        FAIL
    }

    DS_Free(stack);
    free(stack);
    PASS
}

// String Stack

TEST (SimpleStringStack)
{
    StringStackCB stack = SS_New();

    SS_Push(&stack, "1");
    SS_Push(&stack, "2");
    SS_Push(&stack, "3");

    if (stack.size != 3)
        return 0;

    if (!SS_Pop(&stack))
        return 0;

    if (!SS_Pop(&stack))
        return 0;

    if (!SS_Pop(&stack))
        return 0;

    if (stack.size != 0)
        return 0;

    SS_Free(&stack);
    return 1;
}

void DisposeSS(StringStackCB *ss)
{
    SS_Free(ss);
    free(ss);
}

TEST (StringStackStressTest)
{
    StringStackCB *stack = malloc(sizeof(StringStackCB));
    *stack = SS_New();
    char *checkpoint;

    for (int i = 0; i < 1000000; i++)
    {
        double number = (double)rand();
        int length = snprintf(NULL, 0, "%f", number);
        char num[length+1];
        snprintf(num, length+1, "%f", number);
        SS_Push(stack, num);
        if (i == 500000)
            checkpoint=num;
    }

    if (stack->size != 1000000)
    {
        DisposeSS(stack);
        return 0;
    }

    for (int i = 0; i < 500000; i++)
    {
        char *str = SS_Pop(stack);
        if (!str)
            {
                DisposeSS(stack);
                return 0;
            }
        if (i == 500000)
            if (strcmp(str, checkpoint))
                {
                    DisposeSS(stack);
                    return 0;
                }
    }

    if (stack->size != 500000)
    {
        DisposeSS(stack);
        return 0;
    }

    SS_Free(stack);
    free(stack);
    return 1;
}

// Variable Table

TEST (PutVariableTable)
{

    VariableTableCB vt = VT_New();
    int *x = malloc(sizeof(int));
    *x = 12;

    VT_Put(&vt, "Key", x);

    if (vt.size != 1)
    {
        VT_Free(&vt);
        free(x);
        return 0;
    }

    VT_Free(&vt);
    free(x);

    return 1;
}

TEST (VariableTableContainsKey)
{
    VariableTableCB vt = VT_New();

    int *x = malloc(sizeof(int));
    *x = 12;

    VT_Put(&vt, "Key", x);

    if (!VT_ContainsKey(&vt, "Key"))
    {
        free(x);
        VT_Free(&vt);
        return 0;
    }

    free(x);
    VT_Free(&vt);
    return 1;
}

TEST (VariableTableGet)
{

    VariableTableCB vt = VT_New();
    int *x = malloc(sizeof(int));
    *x = 12;

    VT_Put(&vt, "Key", x);

    int *res = (int *)VT_Get(&vt, "Key");


    if (!res)
    {
        Log("unexpected NULL.");
        VT_Free(&vt);
        free(x);
        return 0;
    }

    if (*res != 12)
    {
        VT_Free(&vt);
        free(x);
        return 0;
    }

    VT_Free(&vt);
    free(x);

    return 1;
}

TEST (VariableTableKeys)
{
    VariableTableCB vt = VT_New();

    VT_Put(&vt, "Key1", NULL);
    VT_Put(&vt, "Key2", NULL);
    VT_Put(&vt, "Key3", NULL);

    size_t size;
    char **keys = VT_Keys(&vt, &size);

    if (size != 3) { VT_Free(&vt); return 0; }

    int foundKey1, foundKey2, foundKey3;
    foundKey1 = foundKey2 = foundKey3 = 0;

    for (size_t i = 0; i < size; i++)
    {
        char *key = keys[i];
        if (!strcmp(key, "Key1")) foundKey1 = 1;
        if (!strcmp(key, "Key2")) foundKey2 = 1;
        if (!strcmp(key, "Key3")) foundKey3 = 1;
    }

    if (!(foundKey1 && foundKey2 && foundKey3))
    {
        VT_Free(&vt);
        return 0;
    }

    VT_Free(&vt);
    return 1;
}

/*
    Tests end here.
*/

#define TESTCOUNT 11

int main()
{
    SETUP

    Test *tests = malloc(sizeof(Test)*TESTCOUNT);

    RegisterTest(tests, &SimpleHash, "Simple Hash");
    RegisterTest(tests, &SimpleNumberStack, "Simple DStack");
    RegisterTest(tests, &SimpleHash2, "Simple Hash 2");
    RegisterTest(tests, &NumberStackStressTest, "DStack Stress Test");
    RegisterTest(tests, &SimpleStringStack, "Simple SStack");
    RegisterTest(tests, &StringStackStressTest, "SStack Stress Test");
    RegisterTest(tests, &PutVariableTable, "Variable Table Put");
    RegisterTest(tests, &VariableTableContainsKey, "Variable Table Contains Key");
    RegisterTest(tests, &VariableTableGet, "Variable Table Get");
    RegisterTest(tests, &VariableTableKeys, "Variable Table Keys");
    RegisterTest(tests, &SimpleSplitChar, "Simple Split");

    for (int i = 0; i < TESTCOUNT; i++)
    {
        RunTest(&tests[i], NULL);
        PrintTest(&tests[i]);
    }


    free(tests);

    END
}
