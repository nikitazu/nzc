#ifndef NZC_NZC_H
#define NZC_NZC_H

/* Nikita Zuev Common Code Library v0.2.1
 * ======================================
 */


// Зависимости от стандартной библиотеки C
//

#include <assert.h>
#include <limits.h>
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <stdckdint.h>



// Стандартные типы и литералы
//

#ifndef NZC_NZC_H__COMMON_TYPES
#define NZC_NZC_H__COMMON_TYPES

#include <stdint.h>
#include <stdbool.h>

#define nil NULL

#define U8_MAX UCHAR_MAX
#define U8_MIN UCHAR_MIN
#define U32_MIN 0
#define U32_MAX UINT_MAX

#define I8_MAX CHAR_MAX
#define I8_MIN CHAR_MIN
#define I32_MIN INT_MIN
#define I32_MAX INT_MAX


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


#endif // NZC_NZC_H__COMMON_TYPES



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
#define KB(x) (x * 1024)


// Простая математика
//

#define Math_Define(T)                                  \
    inline T T##_Clamp(T v, T vmin, T vmax)              \
    {                                                   \
        return v < vmin ? vmin : (v > vmax ? vmax : v); \
    }                                                   \
                                                        \
    inline void T##_ClampAssign(T* v, T vmin, T vmax)    \
    {                                                   \
        *v = T##_Clamp(*v, vmin, vmax);                  \
    }

Math_Define(i32);
Math_Define(i64);
Math_Define(f32);
Math_Define(f64);
Math_Define(u32);
Math_Define(u64);

#undef Math_Define


// Арена
//

typedef struct ArenaTag
{
    void* Buffer;
    size_t Offset;
    size_t Size;
} Arena;

Arena Arena_Create(size_t size)
{
    Arena arena = {0};
    arena.Buffer = malloc(size);
    if (arena.Buffer != nil)
    {
        arena.Size = size;
    }
    return arena;
}

#define Arena_Push(ARENA, COUNT, TYPE) \
    (Arena_Alloc(ARENA, COUNT, sizeof(TYPE), _Alignof(TYPE)))

void* Arena_Alloc(Arena* arena, size_t itemCount, size_t itemSize, size_t alignSize)
{
    assert(arena != nil && "Arena_Alloc: Arena pointer must be initialized");
    assert(itemCount > 0 && "Arena_Alloc: Item count must be non negative");
    assert(itemSize > 0 && "Arena_Alloc: Item size must be non negative");

    // Проверка что `alignSize` в степени 2
    if (alignSize == 0 && (alignSize & (alignSize - 1)) == 1)
    {
        return nil;
    }

    uintptr_t allocationSize;
    if (ckd_mul(&allocationSize, itemCount, itemSize))
    {
        return nil;
    }

    uintptr_t totalOffset = (uintptr_t)arena->Buffer + (uintptr_t)arena->Offset;

    // то же самое, что и totalOffset % alignSize, но быстрее
    uintptr_t padding = (~totalOffset + 1) & (alignSize - 1);

    totalOffset += padding;

    // Проверка на вместимость
    if (totalOffset + allocationSize > (uintptr_t)arena->Buffer + (uintptr_t)arena->Size)
    {
        return nil;
    }

    arena->Offset += padding;
    arena->Offset += allocationSize;

    void* object = (void*)totalOffset;
    memset(object, 0, allocationSize);
    return object;
}

void Arena_Reset(Arena* arena)
{
    arena->Offset = 0;
    memset(arena->Buffer, 0, arena->Size);
}

void Arena_Free(Arena* arena)
{
    arena->Offset = 0;
    arena->Size = 0;
    free(arena->Buffer);
    arena->Buffer = nil;
}


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
    inline Vec2##T Vec2##T##_Clamp(Vec2##T a, T vmin, T vmax)   \
    {                                                           \
        return (Vec2##T){                                       \
            T##_Clamp(a.X, vmin, vmax),                         \
            T##_Clamp(a.Y, vmin, vmax),                         \
        };                                                      \
    }                                                           \
                                                                        \
    inline void Vec2##T##_ClampAssign(Vec2##T* a, T vmin, T vmax)       \
    {                                                                   \
        T##_ClampAssign(&a->X, vmin, vmax);                             \
        T##_ClampAssign(&a->Y, vmin, vmax);                             \
    }                                                                   \
                                                                        \
    inline Vec2##T Vec2##T##_Scale(Vec2##T a, T val)                    \
    {                                                                   \
        return (Vec2##T){ a.X * val, a.Y * val };                       \
    }                                                                   \
                                                                        \
    inline void Vec2##T##_ScaleAssign(Vec2##T* a, T val)                \
    {                                                                   \
        a->X *= val;                                                    \
        a->Y *= val;                                                    \
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
