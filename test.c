#include "nzc.h"
#include <stdio.h>


#define PRINT(FMT, ARGS...) (fprintf(stderr, FMT "\n", ARGS))


#define PRINT_Failed(FMT, OP, E, A)                                     \
    fprintf(stderr,                                                     \
            "Error: %s failed\n"                                        \
            "  expected " FMT "\n"                                      \
            "  got " FMT "\n"                                           \
            "  at %s:%d\n\n",                                           \
            OP, E, A, __FILE__, __LINE__);

#define PRINT_Vec2Failed(FMT, OP, E, A, F, L)                           \
    fprintf(stderr,                                                     \
            "Error: %s failed, expected " FMT " got " FMT " at %s:%d\n", \
            OP, E.X, E.Y, A.X, A.Y, F, L);


#define TEST_Clamp(T, VMIN, VMAX)                                       \
    {                                                                   \
        T r, e;                                                         \
        e = VMIN;                                                       \
        r = VMIN - 1;                                                   \
        T##_ClampAssign(&r, VMIN, VMAX);                                \
        success = (r == VMIN);                                          \
        if (!success) {                                                 \
            PRINT_Failed(T##_FMT, "Clamp", e, r);                      \
        }                                                               \
    }


#define TEST_Vec2(T, X0, Y0, X1, Y1)            \
    {                                           \
        T a = { X0, Y0 };                       \
        T b = { X1, Y1 };                       \
        T r;                                    \
        T e;                                    \
                                                \
        e.X = X0 + X1;                          \
        e.Y = Y0 + Y1;                          \
        r = T##_Add(a, b);                      \
                                                \
        success = Vec2_Equal(r, e);                                     \
        if (!success) {                                                 \
            PRINT_Vec2Failed(T##_FMT, "Vec2_Add", e, r, __FILE__, __LINE__); \
        }                                                               \
                                                                        \
        e.X = X0 - X1;                                                  \
        e.Y = Y0 - Y1;                                                  \
        r = T##_Sub(a, b);                                              \
                                                                        \
        success = Vec2_Equal(r, e);                                     \
        if (!success) {                                                 \
            PRINT_Vec2Failed(T##_FMT, "Vec2_Sub", e, r, __FILE__, __LINE__); \
        }                                                               \
                                                                        \
        e.X = X0 * X1;                                                  \
        e.Y = Y0 * Y1;                                                  \
        r = T##_Mul(a, b);                                              \
                                                                        \
        success = Vec2_Equal(r, e);                                     \
        if (!success) {                                                 \
            PRINT_Vec2Failed(T##_FMT, "Vec2_Mul", e, r, __FILE__, __LINE__); \
        }                                                               \
                                                                        \
        e.X = X0 / X1;                                                  \
        e.Y = Y0 / Y1;                                                  \
        r = T##_Div(a, b);                                              \
                                                                        \
        success = Vec2_Equal(r, e);                                     \
        if (!success) {                                                 \
            PRINT_Vec2Failed(T##_FMT, "Vec2_Div", e, r, __FILE__, __LINE__); \
        }                                                               \
                                                                        \
        e.X = X0 * X1;                                                  \
        e.Y = Y0 * X1;                                                  \
        r = T##_Scale(a, b.X);                                          \
                                                                        \
        success = Vec2_Equal(r, e);                                     \
        if (!success) {                                                 \
            PRINT_Vec2Failed(T##_FMT, "Vec2_Scale", e, r, __FILE__, __LINE__); \
        }                                                               \
                                                                        \
        e.X = X0 * X1;                                                  \
        e.Y = Y0 * X1;                                                  \
        r = a;                                                          \
        T##_ScaleAssign(&r, b.X);                                       \
                                                                        \
        success = Vec2_Equal(r, e);                                     \
        if (!success) {                                                 \
            PRINT_Vec2Failed(T##_FMT, "Vec2_ScaleAssign", e, r, __FILE__, __LINE__); \
        }                                                               \
    }


void TEST_Arena(bool* success)
{
    Arena arena = Arena_Create(KB(10));

    if (arena.Buffer == nil)
    {
        PRINT_Failed("%s", "Arena_Create", "arena.Buffer", "nil");
        *success = false;
        return;
    }

    if (arena.Size == 0)
    {
        PRINT_Failed("%s", "Arena_Create", "arena.Size", "0");
        *success = false;
        return;
    }

    char* buffer = Arena_Push(&arena, 255, char); // TODO cast inside macro
    if (buffer == nil)
    {
        PRINT_Failed("%s", "Arena_Alloc (char* buffer)", "pointer", "nil");
        *success = false;
        goto cleanup;
    }

    char* foo = Arena_Push(&arena, 100, char);
    if (foo == nil)
    {
        PRINT_Failed("%s", "Arena_Alloc (char foo[100])", "pointer", "nil");
        *success = false;
        goto cleanup;
    }

    strcpy_s(foo, 100, "ma string!");
    sprintf(buffer, "`%s`", foo);

    if (strcmp(buffer, "`ma string!`") != 0)
    {
        PRINT_Failed("%s", "Arena_Alloc (sprintf)", "`ma string!`", buffer);
        *success = false;
        goto cleanup;
    }

    foo = Arena_Push(&arena, 50, char);
    if (foo == nil)
    {
        PRINT_Failed("%s", "Arena_Alloc (char foo[50])", "pointer", "nil");
        *success = false;
        goto cleanup;
    }

    strcpy_s(foo, 50, "no ma string!");
    sprintf(buffer, "`%s`", foo);

    if (strcmp(buffer, "`no ma string!`") != 0)
    {
        PRINT_Failed("%s", "Arena_Alloc (sprintf)", "`no ma string!`", buffer);
        *success = false;
        goto cleanup;
    }

    foo = Arena_Push(&arena, 10, char);
    if (foo == nil)
    {
        PRINT_Failed("%s", "Arena_Alloc (char foo[10])", "pointer", "nil");
        *success = false;
        goto cleanup;
    }

    i32* barN1 = Arena_Push(&arena, 1, i32);
    if (barN1 == nil)
    {
        PRINT_Failed("%s", "Arena_Alloc (int*)", "pointer", "nil");
        *success = false;
        goto cleanup;
    }

    f32* barN2 = Arena_Push(&arena, 1, f32);
    if (barN2 == nil)
    {
        PRINT_Failed("%s", "Arena_Alloc (float*)", "pointer", "nil");
        *success = false;
        goto cleanup;
    }

    strcpy_s(foo, 10, "howdy!");
    *barN1 = 123;
    *barN2 = 0.456f;

    sprintf(buffer, "s: %s d: %d f: %f", foo, *barN1, *barN2);

    if (strcmp(buffer, "s: howdy! d: 123 f: 0.456000") != 0)
    {
        PRINT_Failed("%s", "Arena_Alloc (sprintf)",
                      "s: howdy! d: 123 f: 0.456", buffer);
        *success = false;
        goto cleanup;
    }

    Arena_Reset(&arena);

    if (arena.Offset > 0)
    {
        PRINT_Failed("%zu", "Arena_Reset (arena.Offset == 0)",
                     (size_t)0, arena.Offset);
        *success = false;
        goto cleanup;
    }

    foo = Arena_Push(&arena, 18446744073709551615U, i32);
    if (foo != nil)
    {
        PRINT_Failed("%s", "Arena_Push (overflow)",
                     "nil", "object");
        *success = false;
        goto cleanup;
    }

cleanup:
    Arena_Free(&arena);
}


void TEST_String(bool* success)
{
    const char* testString = "test";
    String s = String_FromChars(testString);

    if (s.Str != testString)
    {
        PRINT_Failed("%s", "String_FromChars (.Str)", testString, s.Str);
        *success = false;
    }

    if (s.Length != strlen(testString))
    {
        PRINT_Failed("%s", "String_FromChars (.Length)", testString, s.Str);
        *success = false;
    }

    char tmp[100];
    if (!String_CopyTo(s, tmp, 100))
    {
        PRINT_Failed("%s", "String_CopyTo (return value)", "true", "false");
        *success = false;
    }

    if (strcmp(s.Str, tmp) != 0)
    {
        PRINT_Failed("%s", "String_CopyTo (dest)", s.Str, tmp);
        *success = false;
    }

    if (!String_EqualChars(s, tmp))
    {
        PRINT_Failed("%s", "String_EqualChars", s.Str, tmp);
        *success = false;
    }

    if (String_EqualChars(s, nil))
    {
        PRINT_Failed("%s", "String_EqualChars (nil)", "false", "true");
        *success = false;
    }

    String tmps = String_FromChars(tmp);
    if (!String_Equal(s, tmps))
    {
        PRINT_Failed("%s", "String_Equal", s.Str, tmps.Str);
        *success = false;
    }

    if (!str_IsPositiveInt32("123456789"))
    {
        PRINT_Failed("%s", "str_IsPositiveInt32 (123456789)", "true", "false");
        *success = false;
    }

    if (str_IsPositiveInt32("-123456789"))
    {
        PRINT_Failed("%s", "str_IsPositiveInt32 (-123456789)", "false", "true");
        *success = false;
    }

    if (str_IsPositiveInt32("abc"))
    {
        PRINT_Failed("%s", "str_IsPositiveInt32 (abc)", "false", "true");
        *success = false;
    }

    if (str_IsPositiveInt32(""))
    {
        PRINT_Failed("%s", "str_IsPositiveInt32 (empty)", "false", "true");
        *success = false;
    }

    if (str_IsPositiveInt32(nil))
    {
        PRINT_Failed("%s", "str_IsPositiveInt32 (nil)", "false", "true");
        *success = false;
    }

    char* match = str_SearchIgnoreCase("foo", "foo");
    if (strcmp(match, "foo") != 0)
    {
        PRINT_Failed("%s", "str_SearchIgnoreCase (foo, f)", "foo", match);
        *success = false;
    }

    match = str_SearchIgnoreCase("foo", "o");
    if (strcmp(match, "oo") != 0)
    {
        PRINT_Failed("%s", "str_SearchIgnoreCase (foo, o)", "oo", match);
        *success = false;
    }

    match = str_SearchIgnoreCase("foo", "x");
    if (match != nil)
    {
        PRINT_Failed("%s", "str_SearchIgnoreCase (foo, x)", "nil", match);
        *success = false;
    }

    {
        const char* input1 = "abcdefg";
        const char* input2 = "123abcd456";
        String substr1 = { .Str = input1, .Length = 3 };
        String substr2 = { .Str = input2 + 3, .Length = 3 };
        if (!String_Equal(substr1, substr2))
        {
            PRINT_Failed("%s", "String_Equal (substr1, substr2)", "true", "false");
            *success = false;
        }
    }
}


i32 main(i32 argc, const char** args)
{
    PRINT("NZC test start");

    bool success = true;

    TEST_Clamp(f32, 1.0f, 2.0f);
    TEST_Clamp(f64, 1.0, 2.0);
    TEST_Clamp(i32, 1, 2);
    TEST_Clamp(i64, 1, 2);
    TEST_Clamp(u32, 1, 2);
    TEST_Clamp(u64, 1, 2);

    TEST_Vec2(Vec2f32, 1.5f, 2.0f, 0.5f, 1.0f);
    TEST_Vec2(Vec2f64, 1.5, 2.0, 0.5, 1.0);
    TEST_Vec2(Vec2i32, 15, 20, 05, 10);
    TEST_Vec2(Vec2i64, 15, 20, 05, 10);
    TEST_Vec2(Vec2u32, 15, 20, 05, 10);
    TEST_Vec2(Vec2u64, 15, 20, 05, 10);

    TEST_Arena(&success);
    TEST_String(&success);

    if (success)
    {
        PRINT("Success");
    }
    else
    {
        PRINT("Failed");
    }

    return success ? 0 : 1;
}
