#include "nzc.h"
#include <stdio.h>


#define PRINT_Failed(FMT, OP, E, A, F, L)                               \
    printf("Error: %s failed, expected " FMT " got " FMT " at %s:%d\n", \
           OP, E.X, E.Y, A.X, A.Y, F, L);


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
            PRINT_Failed(T##_FMT, "Vec2_Add", e, r, __FILE__, __LINE__); \
        }                                                               \
                                                                        \
        e.X = X0 - X1;                                                  \
        e.Y = Y0 - Y1;                                                  \
        r = T##_Sub(a, b);                                              \
                                                                        \
        success = Vec2_Equal(r, e);                                     \
        if (!success) {                                                 \
            PRINT_Failed(T##_FMT, "Vec2_Sub", e, r, __FILE__, __LINE__); \
        }                                                               \
                                                                        \
        e.X = X0 * X1;                                                  \
        e.Y = Y0 * Y1;                                                  \
        r = T##_Mul(a, b);                                              \
                                                                        \
        success = Vec2_Equal(r, e);                                     \
        if (!success) {                                                 \
            PRINT_Failed(T##_FMT, "Vec2_Mul", e, r, __FILE__, __LINE__); \
        }                                                               \
                                                                        \
        e.X = X0 / X1;                                                  \
        e.Y = Y0 / Y1;                                                  \
        r = T##_Div(a, b);                                              \
                                                                        \
        success = Vec2_Equal(r, e);                                     \
        if (!success) {                                                 \
            PRINT_Failed(T##_FMT, "Vec2_Div", e, r, __FILE__, __LINE__); \
        }                                                               \
    }


i32 main(i32 argc, const char** args)
{
    printf("NZC test start\n");

    bool success = true;

    TEST_Vec2(Vec2f32, 1.5f, 2.0f, 0.5f, 1.0f);

    if (success) {
        printf("Success\n");
    }
    else {
        printf("Failed\n");
    }

    return success ? 0 : 1;
}
