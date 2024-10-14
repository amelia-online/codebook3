#include "../headers/codebook.h"
#include "../headers/cbtesting.h"
#include "../headers/token.h"
#include "../headers/lexer.h"
#include <stdbool.h>


/*
    Tests begin here.
*/
TEST (LexerNew)
{
  Lexer l = Lexer_Make("Line One\nLine Two\n");

  if (l.numlines != 2)
  {
    Lexer_Free(&l);
    FAIL;
  }

  Lexer_Free(&l);
  PASS;
}

TEST (StrIncludesSimple)
{
  char *fl = "52.86";
  bool res = StrIncludes(fl, ".");
  ASSERT (res);
  PASS;
}

TEST (IsFloatSimple)
{
  char *fl = "52.86";
  double res;
  bool isfl = IsFloat(fl, &res);
  ASSERT (isfl);
  ASSERT (res == 52.86);
  PASS;
}

TEST (BufferGetRange)
{
  char *text = malloc(10);
  Buffer buf = Buffer_Make("Lorum Ipsum");
  bool res = Buffer_GetRange(&buf, 0, 5, &text);

  if (!strcmp("Lorum", text))
  {
    free(text);
    FAIL;
  }

  free(text);
  PASS;
}

TEST (BufferGet)
{
  char c;
  Buffer buf = Buffer_Make("Lorum Ipsum");
  bool res  = Buffer_Get(&buf, 4, &c);
  ASSERT (c == 'm');
  ASSERT (res);
  PASS;
}

TEST (BufferPush)
{
  Buffer buf = Buffer_Make("Lorum");
  Buffer_Push(&buf, " Ipsum");
  ASSERT (!strcmp(buf.str, "Lorum Ipsum"));
  PASS;
}

TEST (BufferTest)
{
    Buffer buf = Buffer_Make("Lorum Ipsum");
    ASSERT (buf.size == 11);
    ASSERT (buf.cap == 12);
    PASS;
}

TEST (SimpleStrCount2)
{
    const char *str = "..,..,,,..,..";
    int count = StrCount(str, ",,,");
    ASSERT (count == 1);
    PASS;
}

TEST (SimpleStrCount)
{
    const char *str = "..,..,,,..,..";
    int count = StrCount(str, ",");
    ASSERT (count == 5);
    PASS;
}

TEST (SimpleSubstr)
{
    const char *str = "This is a string";
    char *sub = Substr(str, 4, 7);
    int result = 0;

    if (!strcmp(sub, " is"))
        result = 1;

    free(sub);
    
    return result;
}

// Misc.
TEST (SimpleParse)
{
    PASS;
    size_t len;
    Token *tokens = parse("1 2 +", &len);

    ASSERT (len == 3);

    ASSERT (tokens[0].value.integer == 1);

    ASSERT (tokens[1].value.integer == 2);

    ASSERT (tokens[2].value.intr == Plus);

    free(tokens);

    PASS;
}

TEST (MatchIntrinsicSimple)
{
    Intrinsic out;

    bool found = MatchIntrinsic("->Int", &out);

    ASSERT (found);
    ASSERT (out == CastInt);

    found = MatchIntrinsic("@", &out);

    ASSERT (found);
    ASSERT (out == Load);

    found = MatchIntrinsic("dwef", &out);

    ASSERT (!found);

    PASS;
}

TEST (SplitlnSimple)
{
    char *str = "Two\nLines";
    size_t len;
    char **lines = splitln(str, &len);

    ASSERT (lines);

    ASSERT (len == 2);

    ASSERT (!strcmp(lines[0], "Two"));

    ASSERT (!strcmp(lines[1], "Lines"));

    for (size_t i = 0; i < len; i++)
        free(lines[i]);
    free(lines);

    PASS;
}

TEST (StrEndsWithSimple)
{
    char *rem = (malloc(7));
    rem[6] = '\0';
    char *str = "Hello, world!";
    bool res = StrEndsWith(str, " world!", &rem);

    ASSERT (res);

    ASSERT (strcmp(rem, "Hello,"));

    free(rem);

    PASS;
}

TEST (StrStartsWithSimple)
{
    char *rem = RegisterObj(malloc(7));
    rem[6] = '\0';
    char *str = "Hello, world!";
    bool res = StrStartsWith(str, "Hello, ", &rem);

    ASSERT (res);

    ASSERT (!strcmp(rem, "world!"));

    PASS;
}

TEST (IsNumberSimple)
{
    long val;
    bool is_number = IsNumber("12", &val);

    ASSERT (is_number == true);
    ASSERT (val == 12);

    is_number = IsNumber("nope", &val);

    ASSERT(is_number == false)

    PASS;
}

TEST (IsHexSimple)
{
    long val;
    bool is_number = IsHex("0x09", &val);

    ASSERT (is_number == true);
    ASSERT (val == 9);

    is_number = IsNumber("nope", &val);

    ASSERT(is_number == false)

    PASS;
}

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

    ASSERT (size == 4);

    ASSERT (!strcmp("Here", list[0]));

    ASSERT (!strcmp("is", list[1]));

    ASSERT (!strcmp("four", list[2]));

    ASSERT (!strcmp("words.", list[3]));

    for (int i = 0; i < size; i++)
        free(list[i]);
    free(list);

    PASS;
}

// Number Stack

TEST (SimpleNumberStack)
{
    DataStack stack = DS_New();

    DS_Push(&stack, 1, Int);
    DS_Push(&stack, 2, Int);
    DS_Push(&stack, 3, Int);

    Value val;

    if (stack.size != 3)
        FAIL;

    if (!DS_Pop(&stack, &val))
        FAIL;
    if (val.val != 3)
        FAIL;

    if (!DS_Pop(&stack, &val))
        FAIL;
    if (val.val != 2)
        FAIL;

    if (!DS_Pop(&stack, &val))
        FAIL;
    if (val.val != 1)
        FAIL;

    if (stack.size != 0)
        FAIL;

    DS_Free(&stack);
    PASS;
}

void DisposeDS(DataStack *ds)
{
    DS_Free(ds);
    free(ds);
}

TEST (NumberStackStressTest)
{
    DataStack *stack = malloc(sizeof(DataStack));
    *stack = DS_New();

    for (int i = 0; i < 1000000; i++)
        DS_Push(stack, (long)rand(), Int);

    if (stack->size != 1000000)
    {
        DisposeDS(stack);
        FAIL;
    }

    for (int i = 0; i < 500000; i++)
    {
        Value val;
        if (!DS_Pop(stack, &val))
        {
            DisposeDS(stack);
            FAIL;
        }
    }

    if (stack->size != 500000)
    {
        DisposeDS(stack);
        FAIL;
    }

    DS_Free(stack);
    free(stack);
    PASS;
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

#define TESTCOUNT 26

int main()
{
    SETUP

    Test *tests = malloc(sizeof(Test)*TESTCOUNT);

    RegisterTest(tests, &SimpleHash, "Simple Hash");
    RegisterTest(tests, &SimpleNumberStack, "Simple DStack");
    RegisterTest(tests, &SimpleHash2, "Simple Hash 2");
    RegisterTest(tests, &NumberStackStressTest, "DStack Stress Test");
    RegisterTest(tests, &PutVariableTable, "Variable Table Put");
    RegisterTest(tests, &VariableTableContainsKey, "Variable Table Contains Key");
    RegisterTest(tests, &VariableTableGet, "Variable Table Get");
    RegisterTest(tests, &VariableTableKeys, "Variable Table Keys");
    RegisterTest(tests, &SimpleSplitChar, "Simple Split");
    RegisterTest(tests, &IsNumberSimple, "Simple IsNumber");
    RegisterTest(tests, &IsHexSimple, "Simple IsHex");
    RegisterTest(tests, &StrStartsWithSimple, "Simple StrStartsWith");
    RegisterTest(tests, &StrEndsWithSimple, "Simple StrEndsWith");
    RegisterTest(tests, &SplitlnSimple, "Simple Splitln");
    RegisterTest(tests, &MatchIntrinsicSimple, "Simple MatchIntrinsic");
    RegisterTest(tests, &SimpleParse, "Simple Parse");
    RegisterTest(tests, &SimpleSubstr, "Simple Substr");
    RegisterTest(tests, &SimpleStrCount, "Simple StrCount 1");
    RegisterTest(tests, &SimpleStrCount2, "Simple StrCount 2");
    RegisterTest(tests, &BufferTest, "Buffer Test");
    RegisterTest(tests, &BufferPush, "Buffer Push Simple");
    RegisterTest(tests, &BufferGet, "Buffer Get Simple");
    RegisterTest(tests, &BufferGetRange, "Buffer GetRange Simple");
    RegisterTest(tests, &IsFloatSimple, "IsFloat Simple");
    RegisterTest(tests, &StrIncludesSimple, "StrIncludes Simple");
    RegisterTest(tests, &LexerNew, "Lexer New Test");


    for (int i = 0; i < TESTCOUNT; i++)
    {
        RunTest(&tests[i], NULL);
        PrintTest(&tests[i]);
    }

    free(tests);

    END
}
