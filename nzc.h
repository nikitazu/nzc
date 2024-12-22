#ifndef NZC_NZC_H
#define NZC_NZC_H

/* Nikita Zuev Common Code Library v0.1.0
 * ======================================
 */


// Зависимости от стандартной библиотеки C
//

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>


// Литералы
//

#define nil NULL

#define U8_MAX UCHAR_MAX
#define U8_MIN UCHAR_MIN
#define U32_MIN 0
#define U32_MAX UINT_MAX

#define I8_MAX CHAR_MAX
#define I8_MIN CHAR_MIN
#define I32_MIN INT_MIN
#define I32_MAX INT_MAX


// Стандартные типы
//

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;


#define f32_FMT "%f"
#define f64_FMT "%f"
#define i32_FMT "%i"
#define i64_FMT "%lld"
#define u32_FMT "%i"
#define u64_FMT "%lld"


// Макросы на каждый день
//

#define UNUSED(x) (void)(x)
#define NAMEOF(x) (#x)


// Простая математика
//

#define Math_Define(T)                                  \
    inline T T##_Clip(T v, T vmin, T vmax)              \
    {                                                   \
        return v < vmin ? vmin : (v > vmax ? vmax : v); \
    }                                                   \
                                                        \
    inline void T##_ClipAssign(T* v, T vmin, T vmax)    \
    {                                                   \
        *v = T##_Clip(*v, vmin, vmax);                  \
    }

Math_Define(i32);
Math_Define(i64);
Math_Define(f32);
Math_Define(f64);
Math_Define(u32);
Math_Define(u64);

#undef Math_Define


// Векторы 2D
//

#define Vec2_Define(T)                                          \
    typedef struct Vec2##T##Tag {                               \
        T X;                                                    \
        T Y;                                                    \
    } Vec2##T;                                                  \
                                                                \
    inline Vec2##T Vec2##T##_Add(Vec2##T a, Vec2##T b)          \
    {                                                           \
        return (Vec2##T){ a.X + b.X, a.Y + b.Y };               \
    }                                                           \
                                                                \
    inline Vec2##T Vec2##T##_Sub(Vec2##T a, Vec2##T b)          \
    {                                                           \
        return (Vec2##T){ a.X - b.X, a.Y - b.Y };               \
    }                                                           \
                                                                \
    inline Vec2##T Vec2##T##_Mul(Vec2##T a, Vec2##T b)          \
    {                                                           \
        return (Vec2##T){ a.X * b.X, a.Y * b.Y };               \
    }                                                           \
                                                                \
    inline Vec2##T Vec2##T##_Div(Vec2##T a, Vec2##T b)          \
    {                                                           \
        return (Vec2##T){ a.X / b.X, a.Y / b.Y };               \
    }                                                           \
                                                                \
    inline bool Vec2##T##_Eq(Vec2##T a, Vec2##T b)              \
    {                                                           \
        return a.X == b.X && a.Y == b.Y;                        \
    }                                                           \
                                                                \
    inline Vec2##T Vec2##T##_Clip(Vec2##T a, T vmin, T vmax)    \
    {                                                           \
        return (Vec2##T){                                       \
            T##_Clip(a.X, vmin, vmax),                          \
            T##_Clip(a.Y, vmin, vmax),                          \
        };                                                      \
    }


Vec2_Define(f32);
Vec2_Define(f64);
Vec2_Define(i32);
Vec2_Define(i64);
Vec2_Define(u32);
Vec2_Define(u64);

#undef Vec2_Define


typedef Vec2f32 fvec2;
typedef Vec2f64 dvec2;
typedef Vec2i32 ivec2;
typedef Vec2i64 lvec2;
typedef Vec2u32 uvec2;
typedef Vec2u64 ulvec2;


#define FVEC2(a, b) ((fvec2){ a, b })
#define DVEC2(a, b) ((dvec2){ a, b })
#define IVEC2(a, b) ((ivec2){ a, b })
#define LVEC2(a, b) ((lvec2){ a, b })
#define UVEC2(a, b) ((uvec2){ a, b })
#define ULVEC2(a, b) ((ulvec2){ a, b })


#define Vec2f32_ZERO ((fvec2){0})
#define Vec2f64_ZERO ((dvec2){0})
#define Vec2i32_ZERO ((ivec2){0})
#define Vec2i64_ZERO ((lvec2){0})
#define Vec2u32_ZERO ((uvec2){0})
#define Vec2u64_ZERO ((ulvec2){0})


#define Vec2f32_FMT "{%f;%f}"
#define Vec2f64_FMT "{%f;%f}"
#define Vec2i32_FMT "{%i;%i}"
#define Vec2i64_FMT "{%lld;%lld}"
#define Vec2u32_FMT "{%i;%i}"
#define Vec2u64_FMT "{%lld;%lld}"


#define Vec2_Equal(A, B) \
    ((A.X == B.X) && (A.Y == B.Y))


#endif // NZC_NZC_H
