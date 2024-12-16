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


// Макросы на каждый день
//

#define UNUSED(x) (void)(x)
#define NAMEOF(x) (#x)


// Векторы 2D
//

typedef struct Vec2f32Tag { f32 X, Y; } Vec2f32;
typedef struct Vec2f64Tag { f64 X, Y; } Vec2f64;

typedef struct Vec2i32Tag { i32 X, Y; } Vec2i32;
typedef struct Vec2i64Tag { i64 X, Y; } Vec2i64;

typedef struct Vec2u32Tag { u32 X, Y; } Vec2u32;
typedef struct Vec2u64Tag { u64 X, Y; } Vec2u64;

typedef Vec2f32 fvec2;
typedef Vec2f64 dvec2;

typedef Vec2i32 ivec2;
typedef Vec2i64 lvec2;

typedef Vec2u32 uvec2;
typedef Vec2u64 ulvec2;

#define VEC2F32(a, b) ((Vec2f32){ a, b })
#define VEC2F64(a, b) ((Vec2f64){ a, b })

#define VEC2I32(a, b) ((Vec2i32){ a, b })
#define VEC2I64(a, b) ((Vec2i64){ a, b })

#define VEC2U32(a, b) ((Vec2u32){ a, b })
#define VEC2U64(a, b) ((Vec2u64){ a, b })

#define VECTOR_OP_FUNC(TYPE, FUNC_NAME, OP) \
    TYPE FUNC_NAME(TYPE a, TYPE b) { return (TYPE){ a.X OP b.X, a.Y OP b.Y }; }

inline VECTOR_OP_FUNC(Vec2f32, Vec2f32_Add, +)
inline VECTOR_OP_FUNC(Vec2f64, Vec2f64_Add, +)

inline VECTOR_OP_FUNC(Vec2i32, Vec2i32_Add, +)
inline VECTOR_OP_FUNC(Vec2i64, Vec2i64_Add, +)

inline VECTOR_OP_FUNC(Vec2u32, Vec2u32_Add, +)
inline VECTOR_OP_FUNC(Vec2u64, Vec2u64_Add, +)


inline VECTOR_OP_FUNC(Vec2f32, Vec2f32_Sub, -)
inline VECTOR_OP_FUNC(Vec2f64, Vec2f64_Sub, -)

inline VECTOR_OP_FUNC(Vec2i32, Vec2i32_Sub, -)
inline VECTOR_OP_FUNC(Vec2i64, Vec2i64_Sub, -)

inline VECTOR_OP_FUNC(Vec2u32, Vec2u32_Sub, -)
inline VECTOR_OP_FUNC(Vec2u64, Vec2u64_Sub, -)


inline VECTOR_OP_FUNC(Vec2f32, Vec2f32_Mul, *)
inline VECTOR_OP_FUNC(Vec2f64, Vec2f64_Mul, *)

inline VECTOR_OP_FUNC(Vec2i32, Vec2i32_Mul, *)
inline VECTOR_OP_FUNC(Vec2i64, Vec2i64_Mul, *)

inline VECTOR_OP_FUNC(Vec2u32, Vec2u32_Mul, *)
inline VECTOR_OP_FUNC(Vec2u64, Vec2u64_Mul, *)


inline VECTOR_OP_FUNC(Vec2f32, Vec2f32_Div, /)
inline VECTOR_OP_FUNC(Vec2f64, Vec2f64_Div, /)

inline VECTOR_OP_FUNC(Vec2i32, Vec2i32_Div, /)
inline VECTOR_OP_FUNC(Vec2i64, Vec2i64_Div, /)

inline VECTOR_OP_FUNC(Vec2u32, Vec2u32_Div, /)
inline VECTOR_OP_FUNC(Vec2u64, Vec2u64_Div, /)

#undef VECTOR_OP_FUNC

#define Vec2_Equal(A, B) \
    ((A.X == B.X) && (A.Y == B.Y))

#endif // NZC_NZC_H
