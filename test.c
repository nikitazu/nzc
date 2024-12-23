#include "nzc.h"
#include <stdio.h>


#define PRINT_Failed(FMT, OP, E, A, F, L)                               \
    printf("Error: %s failed, expected " FMT " got " FMT " at %s:%d\n", \
           OP, E, A, F, L);

#define PRINT_Vec2Failed(FMT, OP, E, A, F, L)                           \
    printf("Error: %s failed, expected " FMT " got " FMT " at %s:%d\n", \
           OP, E.X, E.Y, A.X, A.Y, F, L);


#define TEST_Clamp(T, VMIN, VMAX)                                        \
    {                                                                   \
        T r, e;                                                         \
        e = VMIN;                                                       \
        r = VMIN - 1;                                                   \
        T##_ClampAssign(&r, VMIN, VMAX);                                 \
        success = (r == VMIN);                                          \
        if (!success) {                                                 \
            PRINT_Failed(T##_FMT, "Clamp", e, r, __FILE__, __LINE__);    \
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
    }


i32 main(i32 argc, const char** args)
{
    printf("NZC test start\n");

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

    if (success) {
        printf("Success\n");
    }
    else {
        printf("Failed\n");
    }

    return success ? 0 : 1;
}
