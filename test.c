#include "nzc.h"
#include <stdio.h>


#define TEST_Vec2(TEST, TYPE, FUNC_NAME, FMT, A, B, E)     \
    {                                                             \
        TYPE a = A;                                               \
        TYPE b = B;                                               \
        TYPE e = E;                                               \
        TYPE r = FUNC_NAME(a, b);                                 \
        if (!Vec2_Equal(r, e)) {                                  \
            success = false;                                      \
            printf(                                               \
                "Error: " TEST                                    \
                " expected " FMT " got " FMT " at %s:%d\n",       \
                e.X, e.Y, r.X, r.Y, __FILE__, __LINE__);          \
        }                                                         \
    }


i32 main(i32 argc, const char** args)
{
    printf("NZC test start\n");

    bool success = true;

    // Сложение векторов
    //

    TEST_Vec2("Vec2f32_Add", Vec2f32, Vec2f32_Add, "{%f;%f}",
              VEC2F32(1.5f, 2.3f), VEC2F32(0.5f, 0.7f), VEC2F32(2.0f, 3.0f));

    TEST_Vec2("Vec2f64_Add", Vec2f64, Vec2f64_Add, "{%f;%f}",
              VEC2F64(1.5, 2.3), VEC2F64(0.5, 0.7), VEC2F64(2.0, 3.0));

    TEST_Vec2("Vec2i32_Add", Vec2i32, Vec2i32_Add, "{%i;%i}",
              VEC2I32(1, 2), VEC2I32(3, 4), VEC2I32(4, 6));

    TEST_Vec2("Vec2i64_Add", Vec2i64, Vec2i64_Add, "{%lld;%lld}",
              VEC2I64(1, 2), VEC2I64(3, 4), VEC2I64(4, 6));

    TEST_Vec2("Vec2u32_Add", Vec2u32, Vec2u32_Add, "{%i;%i}",
              VEC2U32(1, 2), VEC2U32(3, 4), VEC2U32(4, 6));

    TEST_Vec2("Vec2u64_Add", Vec2u64, Vec2u64_Add, "{%lld;%lld}",
              VEC2U64(1, 2), VEC2U64(3, 4), VEC2U64(4, 6));

    // Вычитание векторов
    //

    TEST_Vec2("Vec2f32_Sub", Vec2f32, Vec2f32_Sub, "{%f;%f}",
              VEC2F32(1.5f, 2.3f), VEC2F32(0.5f, 0.3f), VEC2F32(1.0f, 2.0f));

    TEST_Vec2("Vec2f64_Sub", Vec2f64, Vec2f64_Sub, "{%f;%f}",
              VEC2F64(1.5, 2.4), VEC2F64(0.5, 0.4), VEC2F64(1.0, 2.0));

    TEST_Vec2("Vec2i32_Sub", Vec2i32, Vec2i32_Sub, "{%i;%i}",
              VEC2I32(1, 2), VEC2I32(3, 4), VEC2I32(-2, -2));

    TEST_Vec2("Vec2i64_Sub", Vec2i64, Vec2i64_Sub, "{%lld;%lld}",
              VEC2I64(1, 2), VEC2I64(3, 4), VEC2I64(-2, -2));

    TEST_Vec2("Vec2u32_Sub", Vec2u32, Vec2u32_Sub, "{%u;%u}",
              VEC2U32(1, 2), VEC2U32(1, 2), VEC2U32(0, 0));

    TEST_Vec2("Vec2u64_Sub", Vec2u64, Vec2u64_Sub, "{%llu;%llu}",
              VEC2U64(1, 2), VEC2U64(1, 2), VEC2U64(0, 0));

    // Умножение векторов
    //

    TEST_Vec2("Vec2f32_Mul", Vec2f32, Vec2f32_Mul, "{%f;%f}",
              VEC2F32(3.0f, 3.0f), VEC2F32(2.0f, 2.0f), VEC2F32(6.0f, 6.0f));

    TEST_Vec2("Vec2f64_Mul", Vec2f64, Vec2f64_Mul, "{%f;%f}",
              VEC2F64(3., 3.), VEC2F64(2., 2.), VEC2F64(6., 6.));

    TEST_Vec2("Vec2i32_Mul", Vec2i32, Vec2i32_Mul, "{%i;%i}",
              VEC2I32(1, 2), VEC2I32(3, 4), VEC2I32(3, 8));

    TEST_Vec2("Vec2i64_Mul", Vec2i64, Vec2i64_Mul, "{%lld;%lld}",
              VEC2I64(1, 2), VEC2I64(3, 4), VEC2I64(3, 8));

    TEST_Vec2("Vec2u32_Mul", Vec2u32, Vec2u32_Mul, "{%i;%i}",
              VEC2U32(1, 2), VEC2U32(3, 4), VEC2U32(3, 8));

    TEST_Vec2("Vec2u64_Mul", Vec2u64, Vec2u64_Mul, "{%lld;%lld}",
              VEC2U64(1, 2), VEC2U64(3, 4), VEC2U64(3, 8));

    // Деление векторов
    //

    TEST_Vec2("Vec2f32_Div", Vec2f32, Vec2f32_Div, "{%f;%f}",
              VEC2F32(3.0f, 3.0f), VEC2F32(2.0f, 2.0f), VEC2F32(1.5f, 1.5f));

    TEST_Vec2("Vec2f64_Div", Vec2f64, Vec2f64_Div, "{%f;%f}",
              VEC2F64(3., 3.), VEC2F64(2., 2.), VEC2F64(1.5, 1.5));

    TEST_Vec2("Vec2i32_Div", Vec2i32, Vec2i32_Div, "{%i;%i}",
              VEC2I32(4, 2), VEC2I32(2, 2), VEC2I32(2, 1));

    TEST_Vec2("Vec2i64_Div", Vec2i64, Vec2i64_Div, "{%lld;%lld}",
              VEC2I64(4, 2), VEC2I64(2, 2), VEC2I64(2, 1));

    TEST_Vec2("Vec2u32_Div", Vec2u32, Vec2u32_Div, "{%i;%i}",
              VEC2U32(4, 2), VEC2U32(2, 2), VEC2U32(2, 1));

    TEST_Vec2("Vec2u64_Div", Vec2u64, Vec2u64_Div, "{%lld;%lld}",
              VEC2U64(4, 2), VEC2U64(2, 2), VEC2U64(2, 1));

    if (success) {
        printf("Success\n");
    }
    else {
        printf("Failed\n");
    }

    return success ? 0 : 1;
}
