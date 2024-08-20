#ifndef CB_TESTING_H
#define CB_TESTING_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

#define TestFnPtr(T) int (*T)(void *)
#define TEST(T) int T(void *params)
#define SETUP srand(time(NULL)); \
     printf("Running tests...\n"); \
     printf("================================================\n");
#define END printf("================================================\n"); \
    DisposeAll(); \
    if (disposable) free(disposable); \
    printf("...Done.\n");
#define PASS return 1;
#define FAIL return 0;
#define ASSERT(EXPR) if (!(EXPR)) \
        return 0;

typedef struct TObject
{
    void *ptr;
    struct TObject *next;
} TObject;

TObject MakeTObject(void *ptr)
{
    return (TObject)
    {
        .ptr = ptr,
        .next = NULL,
    };
}

TObject *disposable = NULL;
volatile size_t disposable_size = 0;
volatile size_t disposable_cap = 0;

void _ResizeDisposable()
{
    disposable_cap *= 2;
    disposable = (TObject *)realloc(disposable, disposable_cap*sizeof(TObject));
}

void *RegisterObj(void *ptr)
{
    if (!ptr) return NULL;

    if (!disposable)
    {
        disposable_size += 1;
        disposable_cap = 4;
        disposable = (TObject *)malloc(sizeof(TObject)*4);
        disposable[0] = MakeTObject(ptr);
        return ptr;
    }

    if (disposable_size + 1 > disposable_cap) _ResizeDisposable();

    disposable_size += 1;

    volatile TObject *temp = disposable;
    while (temp->next) temp = temp->next;

    *temp->next = MakeTObject(ptr);

    return ptr;
}

void DisposeAll()
{
    if (!disposable) return;

    TObject *temp = disposable;

    while (temp)
    {
        free(temp->ptr);
        temp = temp->next;
    }
}

void Cleanup(size_t num_ptrs, ...)
{
    va_list args;
    va_start(args, num_ptrs);

    for (int i = 0; i < num_ptrs; i++)
        free(va_arg(args, void *));

    va_end(args);
}


typedef struct
{
    TestFnPtr(testFunc);
    int passed;
    char *name;
} Test;

Test NewTest(TestFnPtr(testFunc), char *name)
{
    return (Test)
    {
        .passed = 0,
        .testFunc = testFunc,
        .name = name,
    };
}

int RunTest(Test *test, void *params)
{
    test->passed = test->testFunc(params);
    return test->passed;
}

void PrintTest(Test *test)
{
    printf("-------------------------------------\n");
    printf("Test: %s\n", test->name);
    if (test->passed)
        printf("Status: Passed!\n");
    else
        printf("Status: FAILED\n");
    printf("-------------------------------------\n");
}

void RegisterTest(Test *tests, TestFnPtr(TestFunc), char *name, ...)
{
    va_list args;
    va_start(args, name);
    int reset = va_arg(args, int);

    static size_t index = 0;

    if (reset) index = 0;

    va_end(args);

    tests[index++] = NewTest(TestFunc, name);
}

void Log(char *data)
{
    printf("LOG: %s\n", data);
}

#endif