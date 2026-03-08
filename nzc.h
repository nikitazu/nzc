#ifndef NZC_NZC_H
#define NZC_NZC_H

/**
 * Nikita Zuev Common Code Library v3.2.0
 * ======================================
 *
 * [SEC00] ОГЛ Навигация
 * ---------------------
 *
 * [SEC01] ОГЛ Инструкция
 * [SEC02] ОГЛ АПИ
 * [SEC03] ОГЛ Реализация
 *
 * [SEC10] ОГЛ База
 * ----------------
 * [SEC11] ОГЛ Зависимости от стандартной библиотеки C
 * [SEC12] ОГЛ Стандартные типы и литералы
 * [SEC13] ОГЛ Макросы на каждый день
 * [SEC14] ОГЛ Простая математика
 * [SEC15] ОГЛ Операции над байтами
 * [SEC16] ОГЛ Генератор случайных числе PCG
 *
 * [SEC20] ОГЛ Типы данных
 * -----------------------
 * [SEC21] ОГЛ Аллокатор типа арена
 * [SEC22] ОГЛ Векторы 2D
 * [SEC23] ОГЛ Строки
 * [SEC231] ОГЛ Строка (8-бит)
 * [SEC232] ОГЛ Строка (16-бит)
 * [SEC24] ОГЛ Парсинг чисел
 * [SEC25] ОГЛ Хеши
 * [SEC251] ОГЛ MD5
 * [SEC252] ОГЛ SHA1
 * [SEC253] ОГЛ SIPHASH
 * [SEC254] ОГЛ FNV1A
 * [SEC26] ОГЛ GUID
 *
 * [SEC30] ОГЛ Контейнеры
 * ----------------------
 * [SEC31] ОГЛ Двое-связанный список
 * [SEC32] ОГЛ Бинарное древо
 * [SEC33] ОГЛ Хеш-Таблица
 *
 * [SEC40] ОГЛ Логирование (ДЕЛА думаю логи следует вытащить в отдельный файл)
 * [SEC41] ДЕЛА Base32
 * [SEC42] ДЕЛА Base64
 */

/**
 * [SEC01] ЗАГ Инструкция
 * ======================
 *
 * // Подключите заголовочный файл
 * #define NZC_NZC_IMPLEMENTATION                             // определить флаг в файле реализации
 * #define NZC_NZC_MD5_HASH_ENABLED                           // включить хеширование MD5
 * #define NZC_NZC_MD5_HASH_DEBUG_PRINT_CHUNK_MEMORY_ENABLED  // включить отладочную печать MD5
 * #define NZC_NZC_SHA1_HASH_ENABLED                          // включить хеширование SHA1
 * #define NZC_NZC_SHA1_HASH_DEBUG_PRINT_CHUNK_MEMORY_ENABLED // включить отладочную печать SHA1
 * #define NZC_NZC_SIPHASH_ENABLED                            // включить хеширование SipHash-2-4
 * #define NZC_NZC_DOUBLY_LINKED_LIST_ENABLED                 // включить двое-связанный список
 * #define NZC_NZC_BINARY_SEARCH_TREE_LIST_ENABLED            // включить бинарное поисковое древо
 * #define NZC_NZC_HASHTABLE_ENABLED                          // включить хештаблицу
 * #include "nzc.h"
 */

/**
 * [SEC02] ЗАГ АПИ
 * ===============
 *
 * [SEC10] ЗАГ База
 * ----------------
 * [SEC11] ЗАГ Зависимости от стандартной библиотеки C
 */

#include <assert.h>
#include <ctype.h>
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <stdckdint.h>
#include <stddef.h>

/**
 * [SEC12] ЗАГ Стандартные типы и литералы
 */

#ifndef NZC_NZC_H__COMMON_TYPES
#define NZC_NZC_H__COMMON_TYPES

#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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

/**
 * [SEC13] ЗАГ Макросы на каждый день
 */

#define f32_FMT "%f"
#define f64_FMT "%f"
#define i32_FMT "%i"
#define i64_FMT "%lld"
#define u32_FMT "%i"
#define u64_FMT "%lld"

#define UNUSED(x) (void)(x)
#define NAMEOF(x) (#x)
#define KB(x) (((size_t)x) * (size_t)1024)
#define MB(x) ((size_t)KB(x) * (size_t)1024)
#define GB(x) ((size_t)MB(x) * (size_t)1024)

#define HAS_FLAG(NUM, FLAG) ((NUM & FLAG) == FLAG)

#define ARRAY_STATIC_COUNT(ARR) \
    (sizeof(ARR) / sizeof(ARR[0]))

#define U32_ROTATE_BITS_LEFT(NUM, SIZE)             \
    (((NUM) << (SIZE)) | ((NUM) >> (32 - (SIZE))))

#define U64_ROTATE_BITS_LEFT(NUM, SIZE)                     \
    ((u64)(((NUM) << (SIZE)) | ((NUM) >> (64 - (SIZE)))))

#define U32_TO_U8_ARRAY_LE(ARR, VAL)   \
    (ARR)[0] = (u8)((VAL)      );      \
    (ARR)[1] = (u8)((VAL) >>  8);      \
    (ARR)[2] = (u8)((VAL) >> 16);      \
    (ARR)[3] = (u8)((VAL) >> 24);

#define U64_TO_U8_ARRAY_LE(ARR, VAL)                    \
    U32_TO_U8_ARRAY_LE((ARR),     (u32)((VAL)      ));  \
    U32_TO_U8_ARRAY_LE((ARR) + 4, (u32)((VAL) >> 32));

#define U8_ARRAY_TO_U64_LE(ARR)                                   \
    (((u64)((ARR)[0]))       | ((u64)((ARR)[1]) <<  8) |          \
     ((u64)((ARR)[2]) << 16) | ((u64)((ARR)[3]) << 24) |          \
     ((u64)((ARR)[4]) << 32) | ((u64)((ARR)[5]) << 40) |          \
     ((u64)((ARR)[6]) << 48) | ((u64)((ARR)[7]) << 56))

/**
 * Хитрый макрос линуксоидов
 * (версия не требующая GCC или CLANG)
 */
#ifndef NZC_CONTAINER_OF
/**
 * Кастует некий указатель на структуру PTR к типу TYPE,
 * сдвигая его на расстояние до члена структуры MEMBER.
 * @PTR    указатель на структуру типа TYPE
 * @TYPE   тип, к которому приводим указатель PTR
 * @MEMBER член структуры, содержащий указатель на PTR
 */
#define NZC_CONTAINER_OF(PTR, TYPE, MEMBER)             \
    ((TYPE *) ((char *)(PTR) - offsetof(TYPE, MEMBER)))
#endif // NZC_CONTAINER_OF

/**
 * [SEC14] ЗАГ Простая математика
 */
#define Math_Define(T)                                                  \
    inline T T##_Clamp(T v, T vmin, T vmax)                             \
    {                                                                   \
        return (v) < (vmin) ? (vmin) : ((v) > (vmax) ? (vmax) : (v));   \
    }                                                                   \
                                                                        \
    inline void T##_ClampAssign(T* v, T vmin, T vmax)                   \
    {                                                                   \
        *v = T##_Clamp(*(v), (vmin), (vmax));                           \
    }

Math_Define(i32);
Math_Define(i64);
Math_Define(f32);
Math_Define(f64);
Math_Define(u32);
Math_Define(u64);

#undef Math_Define

/**
 * [SEC15] ЗАГ Операции над байтами
 */

typedef union FixedArrayU16
{
    u16 Value;
    u8  Bytes[2];
} FixedArrayU16;

typedef union FixedArrayU32
{
    u32 Value;
    u8  Bytes[4];
} FixedArrayU32;

typedef union FixedArrayU64
{
    u64 Value;
    u32 Int32[2];
    u8  Bytes[8];
} FixedArrayU64;

typedef union FixedArrayU128
{
    struct { u64 Hi; u64 Lo; } Value;
    u8                         Bytes[16];
} FixedArrayU128;

u16 u16_SwapBytes(u16 v);
u32 u32_SwapBytes(u32 v);
u64 u64_SwapBytes(u64 v);
void DebugPrintMemory(FILE* f, u8* buffer, size_t size);

/**
 * [SEC16] ЗАГ Генератор случайных числе PCG
 */

/*
 * Примечание: реализация PCG взята с https://www.pcg-random.org
 * (минимальная версия) и доступна в соответствии с оригинальной
 * лицензией (текст ниже).
 *
 * PCG Random Number Generation for C.
 *
 * Copyright 2014 Melissa O'Neill <oneill@pcg-random.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For additional information about the PCG random number generation scheme,
 * including its license and other licensing options, visit
 *
 *     http://www.pcg-random.org
 */

/*
 * This code is derived from the full C implementation, which is in turn
 * derived from the canonical C++ PCG implementation. The C++ version
 * has many additional features and is preferable if you can use C++ in
 * your project.
 */

#ifndef PCG_BASIC_H_INCLUDED
#define PCG_BASIC_H_INCLUDED 1

#include <inttypes.h>

#if __cplusplus
extern "C" {
#endif

struct pcg_state_setseq_64 {    // Internals are *Private*.
    uint64_t state;             // RNG state.  All values are possible.
    uint64_t inc;               // Controls which RNG sequence (stream) is
                                // selected. Must *always* be odd.
};
typedef struct pcg_state_setseq_64 pcg32_random_t;

// If you *must* statically initialize it, here's one.

#define PCG32_INITIALIZER   { 0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL }

// pcg32_srandom(initstate, initseq)
// pcg32_srandom_r(rng, initstate, initseq):
//     Seed the rng.  Specified in two parts, state initializer and a
//     sequence selection constant (a.k.a. stream id)

void pcg32_srandom(uint64_t initstate, uint64_t initseq);
void pcg32_srandom_r(pcg32_random_t* rng, uint64_t initstate,
                     uint64_t initseq);

// pcg32_random()
// pcg32_random_r(rng)
//     Generate a uniformly distributed 32-bit random number

uint32_t pcg32_random(void);
uint32_t pcg32_random_r(pcg32_random_t* rng);

// pcg32_boundedrand(bound):
// pcg32_boundedrand_r(rng, bound):
//     Generate a uniformly distributed number, r, where 0 <= r < bound

uint32_t pcg32_boundedrand(uint32_t bound);
uint32_t pcg32_boundedrand_r(pcg32_random_t* rng, uint32_t bound);

#if __cplusplus
}
#endif

#endif // PCG_BASIC_H_INCLUDED

#define NZC_RANDOM32_INIT_VALUE ((Random32){PCG32_INITIALIZER, 42U, 54U})

typedef struct Random32
{
    pcg32_random_t Impl;
    u32            InitState;
    u32            InitSeq;
} Random32;

void Random32_Init(Random32* rng);
u32 Random32_Next(Random32* rng);


/**
 * [SEC20] ЗАГ Типы данных
 * -----------------------
 * [SEC21] ЗАГ Аллокатор типа арена
 */

/**
 * Арена
 *
 * @field Buffer указатель на буфер в памяти
 * @field Offset отступ от начала буффера, указывающий на незанятую область в буфере
 * @field Size   размер буфера в памяти
 */
typedef struct Arena
{
    void*  Buffer;
    size_t Offset;
    size_t Size;
} Arena;

/**
 * Создаёт арену
 *
 * @param size размер буфера, который будет выделен под новую арену
 */
Arena Arena_Create(size_t size);

/**
 * Добавляет новые объекты на арену
 *
 * @param ARENA указатель на арену (тип `Arena*`)
 * @param COUNT кол-во значений для добавления на арену (тип `size_t`)
 * @param TYPE  тип данных значений, добавляемых на арену
 */
#define Arena_Push(ARENA, COUNT, TYPE) \
    (Arena_Alloc(ARENA, COUNT, sizeof(TYPE), _Alignof(TYPE)))

/**
 * Добавляет новые объекты на арену
 *
 * @param arena     указатель на арену
 * @param itemCount кол-во значений для добавления на арену
 * @param itemSize  размер добавляемого значения
 * @param alignSize выравнивание добавляемого значения
 */
void* Arena_Alloc(Arena* arena, size_t itemCount, size_t itemSize, size_t alignSize);

/**
 * Сбрасывает арену, сдвиная на начало и затирая данные
 *
 * @param arena указатель на арену
 */
void Arena_Reset(Arena* arena);

/**
 * Освобождает память, занимаемую ареной
 *
 * @param arena указатель на арену
 */
void Arena_Free(Arena* arena);

/**
 * Создаёт арену внутри другой арены
 *
 * @param parent указатель на родительскую арену
 * @param size   размер создаваемой арены
 * @return       указатель на созданную арену
 */
Arena* Arena_CreateChild(Arena* parent, size_t size);

/**
 * Создаёт копию арены с новым размером, копируя все данные в новое место.
 *
 * @param source  указатель на арену-источник
 * @param newSize размер новой арены (должен быть больше или равен размеру арены-источника)
 * @return        новую арену
 *
 * Внимание: данные копируются как просто байты, без интерпретации.
 */
Arena Arena_CreateCopy(const Arena* source, size_t newSize);

/**
 * [SEC22] ЗАГ Векторы 2D
 */

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

/**
 * [SEC23] ЗАГ Строки
 * [SEC231] ЗАГ Строка (8-бит)
 */

/**
 * Строка (8-бит)
 *
 * @field Length размер строки
 * @field Str    указатель на начало строки
 */
typedef struct String
{
    size_t Length;
    const char* Str;
} String;

/**
 * Создаёт строку из массива символов
 *
 * @param str указатель на массив символов
 */
String String_FromChars(const char* str);

/**
 * Копирует строку в целевой буфер
 *
 * @param source     копируемая строка
 * @param dest       целевой буфер
 * @param destLength размер целевого буфера
 */
bool String_CopyTo(String source, char* dest, size_t destLength);

/**
 * Сравнивает две строки с учётом регистра символов
 *
 * @param a одна строка
 * @param b другая строка
 * @return  истина, если строки идентичны, иначе - ложь
 */
bool String_Equal(String a, String b);

/**
 * Сравнивает два массива символов строки с учётом регистра символов для сортировки
 *
 * @param s1   первый массив символов
 * @param len1 длина первого массива
 * @param s2   второй массив символов
 * @param len2 длина второго массива
 * @return      0 - если массивы одинаковые
 *              1 - если первый массив больше
 *             -1 - если первый массив меньше
 *
 * Примечание: массивы считаются одинаковыми, если их указатели равны
 *             или их длина и содержимое совпадают.
 *
 *             Всякий массив считается больше другого,
 *             если его указатель не nil, а другой указател nil.
 *
 *             Всякий массив считается больше другого,
 *             если в нём есть символы с большим значением
 *             в порядке расположения, в т.ч. и за счёт большей длины.
 */
i32 str_Compare(const char* s1, size_t len1,
                const char* s2, size_t len2);

/**
 * Сравнивает две строки с учётом регистра символов для сортировки
 *
 * @param a первая строка
 * @param b вторая строка
 * @return   0 - если строки одинаковые
 *           1 - если первая строка больше
 *          -1 - если первая строка меньше
 *
 * Примечание: алгоритм сравненеия аналогичен `str_Compare`
 */
i32 String_Compare(String a, String b);

/**
 * Сравнивает строку с массивом символов с учётом регистра символов
 *
 * @param s   строка
 * @param str массив символов
 * @return    истина, если данные идентичны, иначе - ложь
 */
bool String_EqualChars(String s, const char* str);

/**
 * Ищет строку-иголку `needle` в строке-стоге-сена `haystack`
 * без учёта регистра символов
 *
 * @param haystack строка, в которой осуществляется поиск
 * @param needle   искомая подстрока
 * @return         в случае успеха - указатель на расположение подстроки
 *                 в случае неудачи - nil
 *
 * Примечание: ожидает нуль-терминированные массивы символов в качестве строк
 */
char* str_SearchIgnoreCase(const char* haystack, const char* needle);

/**
 * Проверяет, что входная строка содержит положительное целое число в пределах 32 бит.
 *
 * @param s строка
 * @return  истина в случае успеха, иначе - ложь
 *
 * Примечание: ожидает нуль-терминированный массив символов в качестве строки
 */
bool str_IsPositiveInt32(const char* s);

/**
 * [SEC24] ЗАГ Парсинг чисел
 */

/**
 * Распознаёт целое 32-битное число со знаком в предоставленном буфере
 *
 * @param buffer указатель на буфер символов
 * @param offset сдвиг от начала буфера
 * @param count  кол-во элементов после сдвига, которое нужно распознать
 * @return       распознанное число или 0
 */
i32 i32_Parse(const char* buffer, size_t offset, size_t count);

/**
 * Распознаёт целое 64-битное число со знаком в предоставленном буфере
 *
 * @param buffer указатель на буфер символов
 * @param offset сдвиг от начала буфера
 * @param count  кол-во элементов после сдвига, которое нужно распознать
 * @return       распознанное число или 0
 */
i64 i64_Parse(const char* buffer, size_t offset, size_t count);

/**
 * Распознаёт 32-битное число с плавающей запятой со знаком в предоставленном буфере
 *
 * @param buffer указатель на буфер символов
 * @param offset сдвиг от начала буфера
 * @param count  кол-во элементов после сдвига, которое нужно распознать
 * @return       распознанное число или 0.f
 */
f32 f32_Parse(const char* buffer, size_t offset, size_t count);

/**
 * [SEC232] ЗАГ Строка (16-бит)
 */

/**
 * Строка (16-бит) - используется для работы с текстом в формате UTF-16.
 *
 * Примечание:
 *
 *   В Win32 API соответствующие типы и функции имеют приставку или суффикс 'w'.
 *   Символы называются широкими (WideCharacter) и соответствуют типу wchar_t,
 *   доступ к ним спрятан за макросами и активируется флагами UNIСODE и _UNICODE.
 *
 *   Библиотека NZC никак не реагирует на флаги UNICODE и _UNICODE, и предлагает
 *   пользователю самостоятельно без шаманских танцев с бубнами решить
 *   где ему использовать тип String, а где String16.
 *
 * @field Length размер строки
 * @field Str    указатель на начало строки
 */
typedef struct String16
{
    size_t         Length;
    const wchar_t* Str;
} String16;

/**
 * Создаёт строку из массива символов
 *
 * @param str указатель на массив символов
 */
String16 String16_FromChars(const wchar_t* str);

/**
 * Сравнивает две строки с учётом регистра символов
 *
 * @param a одна строка
 * @param b другая строка
 * @return  истина, если строки идентичны, иначе - ложь
 */
bool String16_Equal(String16 a, String16 b);

/**
 * Сравнивает два массива символов строки с учётом регистра символов для сортировки
 *
 * @param s1   первый массив символов
 * @param len1 длина первого массива
 * @param s2   второй массив символов
 * @param len2 длина второго массива
 * @return      0 - если массивы одинаковые
 *              1 - если первый массив больше
 *             -1 - если первый массив меньше
 *
 * Примечание: массивы считаются одинаковыми, если их указатели равны
 *             или их длина и содержимое совпадают.
 *
 *             Всякий массив считается больше другого,
 *             если его указатель не nil, а другой указател nil.
 *
 *             Всякий массив считается больше другого,
 *             если в нём есть символы с большим значением
 *             в порядке расположения, в т.ч. и за счёт большей длины.
 */
i32 str16_Compare(const wchar_t* s1, size_t len1,
                  const wchar_t* s2, size_t len2);

/**
 * Сравнивает две строки с учётом регистра символов для сортировки
 *
 * @param a первая строка
 * @param b вторая строка
 * @return   0 - если строки одинаковые
 *           1 - если первая строка больше
 *          -1 - если первая строка меньше
 *
 * Примечание: алгоритм сравненеия аналогичен `str_Compare`
 */
i32 String16_Compare(String16 a, String16 b);

/**
 * Ищет строку-иголку `needle` в строке-стоге-сена `haystack`
 * без учёта регистра символов
 *
 * @param haystack строка, в которой осуществляется поиск
 * @param needle   искомая подстрока
 * @return         в случае успеха - указатель на расположение подстроки
 *                 в случае неудачи - nil
 *
 * Примечание: ожидает нуль-терминированные массивы символов в качестве строк
 */
const wchar_t* str16_SearchIgnoreCase(const wchar_t* haystack, const wchar_t* needle);

/**
 * [SEC24] ЗАГ Парсинг чисел
 */

/**
 * Распознаёт целое 32-битное число со знаком в предоставленном буфере
 *
 * @param buffer указатель на буфер символов
 * @param offset сдвиг от начала буфера
 * @param count  кол-во элементов после сдвига, которое нужно распознать
 * @return       распознанное число или 0
 */
i32 i32_Parse(const char* buffer, size_t offset, size_t count);

/**
 * Распознаёт целое 64-битное число со знаком в предоставленном буфере
 *
 * @param buffer указатель на буфер символов
 * @param offset сдвиг от начала буфера
 * @param count  кол-во элементов после сдвига, которое нужно распознать
 * @return       распознанное число или 0
 */
i64 i64_Parse(const char* buffer, size_t offset, size_t count);

/**
 * Распознаёт 32-битное число с плавающей запятой со знаком в предоставленном буфере
 *
 * @param buffer указатель на буфер символов
 * @param offset сдвиг от начала буфера
 * @param count  кол-во элементов после сдвига, которое нужно распознать
 * @return       распознанное число или 0.f
 */
f32 f32_Parse(const char* buffer, size_t offset, size_t count);

/**
 * [SEC25] ЗАГ Хеши
 * [SEC251] ЗАГ MD5
 */

#ifdef NZC_NZC_MD5_HASH_ENABLED

/** Размер блока MD5, в байтах */
#define MD5_CHUNK_SIZE          64

/**
 * Размер сегмента с длиной сообщения, в байтах.
 * Примечание:
 *   не запутаться бы, само значение длины в битах,
 *   а вот данная константа - это размер области памяти
 *   где будет длина расположена, и этот размер в байтах.
 */
#define MD5_MESSAGE_LENGTH_SIZE 8

/** Первый байт выравнивания */
#define MD5_PADDING_FIRST_BYTE  0x80

/** Прочие байты выравнивания */
#define MD5_PADDING_FILL_BYTE   0x00

/** Размер массива с данными MD5, в байтах */
#define MD5_BYTE_ARRAY_SIZE     16

/** Длина HEX-строки MD5, в символах */
#define MD5_STRING_LENGTH       32

/** Вектор инициализации MD5, компонент A */
#define MD5_IV_A                0x67452301

/** Вектор инициализации MD5, компонент B */
#define MD5_IV_B                0xEFCDAB89

/** Вектор инициализации MD5, компонент C */
#define MD5_IV_C                0x98BADCFE

/** Вектор инициализации MD5, компонент D */
#define MD5_IV_D                0x10325476

/**
 * Буфер данных MD5.
 *
 * @field Bytes массив байтов MD5
 * @field A     первая компонента MD5
 * @field B     вторая компонента MD5
 * @field C     третья компонента MD5
 * @field D     четвёртая компонента MD5
 *
 * Примечание: см. [RFC-1321](doc/specs/rfc1321.txt)
 */
typedef struct HashMd5
{
    union
    {
        u8 Bytes[MD5_BYTE_ARRAY_SIZE];
        struct
        {
            u32 A;
            u32 B;
            u32 C;
            u32 D;
        };
    };
} HashMd5;

/**
 * Рассчитать хеш MD5.
 *
 * Наполняет буфер `hash' байтами контрольной суммы,
 * используя байты из буфера `buffer' в количестве `size'
 * в качестве источника данных.
 *
 * @param hash   буфер для наполнения контрольной суммой
 * @param buffer буфер для чтения исходных данных
 * @param size   количество байт в `buffer'
 */
void HashMd5_Compute(HashMd5* hash, u8* buffer, size_t size);

/**
 * Вывести хеш MD5 в файл.
 *
 * Печатает байты из буфера `hash' как HEX-строку в файл `f'.
 *
 * @param hash буфер с контрльной суммой MD5
 * @param f    файл для вывода
 */
void HashMd5_Write(HashMd5* hash, FILE* f);

/**
 * Вывести хеш MD5 в буфер.
 *
 * Печатает байты из буфера `hash' как HEX-строку в буфер `buffer'.
 *
 * @param hash          буфер с контрльной суммой MD5
 * @param outBuffer     буфер для вывода
 * @param outBufferSize размер буфера `buffer'
 * @return              признак успеха операции
 */
bool HashMd5_WriteStringToBuffer(HashMd5* hash, char* outBuffer, size_t outBufferSize);

#endif // NZC_NZC_MD5_HASH_ENABLED

/**
 * [SEC252] ЗАГ SHA1
 */

#ifdef NZC_NZC_SHA1_HASH_ENABLED

/** Размер блока SHA1, в байтах */
#define SHA1_CHUNK_SIZE          64

/**
 * Размер сегмента с длиной сообщения, в байтах.
 * Примечание:
 *   не запутаться бы, само значение длины в битах,
 *   а вот данная константа - это размер области памяти
 *   где будет длина расположена, и этот размер в байтах.
 */
#define SHA1_MESSAGE_LENGTH_SIZE 8

/** Первый байт выравнивания */
#define SHA1_PADDING_FIRST_BYTE  0x80

/** Прочие байты выравнивания */
#define SHA1_PADDING_FILL_BYTE   0x00

/** Размер массива с u32 элементами SHA1, в штуках */
#define SHA1_U32_ARRAY_SIZE      5

/** Размер массива с данными SHA1, в байтах */
#define SHA1_BYTE_ARRAY_SIZE     20

/** Длина HEX-строки SHA1, в символах */
#define SHA1_STRING_LENGTH       40

/** Вектор инициализации SHA1, компонент 0 */
#define SHA1_IV_H0               0x67452301

/** Вектор инициализации SHA1, компонент 1 */
#define SHA1_IV_H1               0xEFCDAB89

/** Вектор инициализации SHA1, компонент 2 */
#define SHA1_IV_H2               0x98BADCFE

/** Вектор инициализации SHA1, компонент 3 */
#define SHA1_IV_H3               0x10325476

/** Вектор инициализации SHA1, компонент 4 */
#define SHA1_IV_H4               0xC3D2E1F0

#define SHA1_K1 0x5A827999
#define SHA1_K2 0x6ED9EBA1
#define SHA1_K3 0x8F1BBCDC
#define SHA1_K4 0xCA62C1D6

/**
 * Буфер данных SHA1.
 *
 * @field Bytes   массив байтов SHA1
 * @field Numbers массив u32 чисел SHA1
 *
 * Примечание: см. [RFC-3174](doc/specs/rfc3174.txt)
 */
typedef struct HashSha1
{
    union
    {
        u8  Bytes[SHA1_BYTE_ARRAY_SIZE];
        u32 Numbers[SHA1_U32_ARRAY_SIZE];
    };
} HashSha1;

/**
 * Рассчитать хеш SHA1.
 *
 * Наполняет буфер `hash' байтами контрольной суммы,
 * используя байты из буфера `buffer' в количестве `size'
 * в качестве источника данных.
 *
 * @param hash   буфер для наполнения контрольной суммой
 * @param buffer буфер для чтения исходных данных
 * @param size   количество байт в `buffer'
 */
void HashSha1_Compute(HashSha1* hash, u8* buffer, size_t size);

/**
 * Вывести хеш SHA1 в файл.
 *
 * Печатает байты из буфера `hash' как HEX-строку в файл `f'.
 *
 * @param hash буфер с контрльной суммой SHA1
 * @param f    файл для вывода
 */
void HashSha1_Write(HashSha1* hash, FILE* f);

/**
 * Вывести хеш SHA1 в буфер.
 *
 * Печатает байты из буфера `hash' как HEX-строку в буфер `buffer'.
 *
 * @param hash          буфер с контрльной суммой SHA1
 * @param outBuffer     буфер для вывода
 * @param outBufferSize размер буфера `buffer'
 * @return              признак успеха операции
 */
bool HashSha1_WriteStringToBuffer(HashSha1* hash, char* outBuffer, size_t outBufferSize);

#endif // NZC_NZC_SHA1_HASH_ENABLED

#ifdef NZC_NZC_SIPHASH_ENABLED

/*
 * Примечание: реализация SIPHASH основана на SipHash reference C implemntation,
 * и доступна в соответствии с оригинальной лицензией (текст ниже).
 *
 * SipHash reference C implementation
 *
 * Copyright (c) 2012-2022 Jean-Philippe Aumasson
 * <jeanphilippe.aumasson@gmail.com>
 * Copyright (c) 2012-2014 Daniel J. Bernstein <djb@cr.yp.to>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along
 * with
 * this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

/**
 * [SEC253] ЗАГ SIPHASH-2-4
 */

#define SIPHASH_64_SIZE            8
#define SIPHASH_128_SIZE          16
#define SIPHASH_64_STRING_LENGTH  16
#define SIPHASH_128_STRING_LENGTH 32
#define SIPHASH_C_ROUNDS           2
#define SIPHASH_D_ROUNDS           4

typedef struct SipHash64
{
    void*         Key;
    FixedArrayU64 Digest;
} SipHash64;

typedef struct SipHash128
{
    void*          Key;
    FixedArrayU128 Digest;
} SipHash128;

void SipHash64_Compute(SipHash64* hash,
                       const void* buffer,
                       const size_t bufferSize);

void SipHash128_Compute(SipHash128* hash,
                        const void* buffer,
                        const size_t bufferSize);

void SipHash64_Write(SipHash64* hash, FILE* f);

bool SipHash64_WriteStringToBuffer(SipHash64* hash,
                                   char* outBuffer,
                                   size_t outBufferSize);

void SipHash128_Write(SipHash128* hash, FILE* f);

bool SipHash128_WriteStringToBuffer(SipHash128* hash,
                                    char* outBuffer,
                                    size_t outBufferSize);
#endif // NZC_NZC_SIPHASH_ENABLED

/**
 * [SEC254] ЗАГ FNV1A
 */

#define NZC_FNV1A64_OFFSET 14695981039346656037UL
#define NZC_FNV1A64_PRIME         1099511628211UL

/**
 * Рассчитать хеш FNV-1a.
 *
 * @param buffer буфер для чтения исходных данных
 * @param size   количество байт в `buffer'
 *
 * Возвращает 64-битный хеш для входного буфера данных
 * в виде целого беззнагового числа.
 */
u64 FNV1A64_Compute(const void* buffer, size_t bufferSize);

/*
 * [SEC26] ЗАГ GUID
 */

/** Размер UUID в байтах */
#define GUID_SIZE_BYTES 16

/** Размер строгового представления UUID в символах без черточек и без нуль терминатора */
#define GUID_STRING_LENGTH 32

/** Значение генератора для статической инициализации */
#define NZC_GUIDGEN_INITIAL_VALUE ((GuidGen){.Random = NZC_RANDOM32_INIT_VALUE})

/**
 * Генератор GUID.
 *
 * Содержит в себе источник псевдо-случайности.
 */
typedef struct GuidGen
{
    Random32 Random;
} GuidGen;

/**
 * Форматы GUID (для отображения).
 */
typedef enum Guid_Format
{
    /** Без форматирования. */
    Guid_FormatNone      = 0,                 // 0000

    /** Через чёрточки. */
    Guid_FormatDash      = 1 << 0,            // 0001

    /** В нижнем регистре. */
    Guid_FormatLowerCase = 1 << 1,            // 0010

    /** Как в документации RFC-4122 (через чёрточки в нижнем регистре). */
    Guid_FormatRfc4122   = (1 << 0 | 1 << 1), // 0011
} Guid_Format;

/**
 * Инициализировать генератор GUID.
 *
 * @param gg указатель на структуру генератора.
 */
void Guid_Init(GuidGen* gg);

/**
 * Выдать следующий GUID версии 4 из глобального генератора.
 *
 * @param buf буфер для записи GUID.
 * @return признак успеха операции.
 *
 * Важно: см. `Guid_Nextv4Ex`.
 */
bool Guid_Nextv4(void* buf);

/**
 * Выдать следующий GUID версии 4 из переданного генератора `gg'.
 *
 * @param gg указатель на структуру генератора.
 * @param buf буфер для записи GUID.
 * @return признак успеха операции.
 *
 * Важно: буфер `buf' должен быть размера не менее, чем `GUID_SIZE_BYTES'.
 */
bool Guid_Nextv4Ex(GuidGen* gg, void* buf);

/**
 * Выдать следующий GUID версии 7 из глобального генератора.
 *
 * @param buf буфер для записи GUID.
 * @return признак успеха операции.
 *
 * Важно: см. `Guid_Nextv4Ex`.
 */
bool Guid_Nextv7(void* buf);

/**
 * Выдать следующий GUID версии 7 из переданного генератора `gg'.
 *
 * @param gg указатель на структуру генератора.
 * @param buf буфер для записи GUID.
 * @return признак успеха операции.
 *
 * Важно: буфер `buf' должен быть размера не менее, чем `GUID_SIZE_BYTES'.
 */
bool Guid_Nextv7Ex(GuidGen* gg, void* buf);

/**
 * Вывести GUID в буфер.
 *
 * Печатает байты из буфера `guid' как HEX-строку в буфер `outBuffer'.
 *
 * @param guid          буфер содержащий GUID
 * @param outBuffer     буфер для вывода
 * @param outBufferSize размер буфера `buffer'
 * @param format        формат вывода
 * @return              признак успеха операции
 */
bool Guid_WriteStringToBuffer(void* guid,
                              char* outBuffer, size_t outBufferSize,
                              Guid_Format format);

/**
 * [SEC30] ЗАГ Контейнеры
 * ----------------------
 * [SEC31] ЗАГ Двое-связанный список
 */

#ifdef NZC_NZC_DOUBLY_LINKED_LIST_ENABLED

struct DLNode;
typedef struct DLNode DLNode;

/**
 * Узел двое-связанного списка.
 * @Next указатель на следующий узел списка
 * @Prev указатель на предыдущий узел списка
 *
 * Структура списка такова, что первый узел является
 * корневым, он не содержит данные и используется
 * для идентификации списка, передачи его в функции,
 * а также обозначения начала и конца.
 *
 * Список лишь из одного корневого узла - это пустой список,
 * его указатели Next и Prev замкнуты на самого себя.
 */
struct DLNode
{
    DLNode* Next;
    DLNode* Prev;
};

void DLNode_Init(DLNode* item);
void DLNode_Prepend(DLNode* item, DLNode* toList);
void DLNode_Append(DLNode* item, DLNode* toList);
void DLNode_Remove(DLNode* item);
void DLNode_MovePrepend(DLNode* item, DLNode* toList);
void DLNode_MoveAppend(DLNode* item, DLNode* toList);
bool DLNode_IsEmpty(DLNode* item);
void DLNode_Concat(DLNode* list1, DLNode* list2);

#ifndef DL_EACH
#define DL_EACH(TYPE, IT, LIST, MEMBER)                                 \
    for (TYPE* IT = NZC_CONTAINER_OF((&(LIST)->MEMBER)->Next, TYPE, MEMBER); \
         &IT->MEMBER != &((LIST)->MEMBER);                              \
         IT = NZC_CONTAINER_OF(IT->MEMBER.Next, TYPE, MEMBER))
#endif // DL_EACH

#endif // NZC_NZC_DOUBLY_LINKED_LIST_ENABLED

/**
 * [SEC32] ЗАГ Бинарное древо
 */

#ifdef NZC_NZC_BINARY_SEARCH_TREE_LIST_ENABLED

typedef struct BST BST;

struct BST
{
    BST* Left;
    BST* Right;
};

typedef enum BSTResultType
{
    BSTResultType_MatchThis = 0,
    BSTResultType_EmptyLeft,
    BSTResultType_EmptyRight,
} BSTResultType;

typedef struct BSTResult
{
    BST*          Node;
    BSTResultType Type;
} BSTResult;

// TODO this is left recursion, rewrite as iteration
BSTResult BST_FindInt32(BST* t, size_t keyOffset, i32 key);
BSTResult BST_FindString(BST* t, size_t keyOffset, String key);

typedef void (*BST_WalkProc)(BST* it, void* accum);
void BST_WalkInOrder(BST* t, void* accum, BST_WalkProc proc);

// ДЕЛА реализовать нерекурсивный итератор, с использованием стека

#endif // NZC_NZC_BINARY_SEARCH_TREE_LIST_ENABLED

/**
 * [SEC33] ЗАГ Хеш-Таблица
 */

#ifdef NZC_NZC_HASHTABLE_ENABLED

// TODO define settings
//      hash function
//      initial size
//      grow factor

#define NZC_HASHTABLE_INITIAL_CAPACITY 16
#define NZC_HASHTABLE_GROTH_FACTOR 0.75f

typedef struct HashEntry
{
    String Key;
} HashEntry;

typedef struct HashTable
{
    size_t Capacity;
    size_t ItemSize;
    void*  Items;
} HashTable;

typedef enum HashTableResultType
{
    HashTableResultType_Match = 0,        // нашли ячейку с данными
    HashTableResultType_Empty,            // нашли пустую ячейку
    HashTableResultType_CapacityReached,  // нет места
} HashTableResultType;

typedef struct HashTableResult
{
    HashTableResultType Type;
    void*               Entry;
} HashTableResult;

typedef struct HashTableIterator
{
    HashTable* Table;
    void*      Entry;
    bool       Eof;
    size_t     Index;
} HashTableIterator;

void HashTable_Init(HashTable* ht, size_t capacity, size_t itemSize, void* items);
size_t HashTable_Count(HashTable* ht);
HashTableResult HashTable_Find(HashTable* ht, String key);
HashTableResult HashTable_FindWithHash(HashTable* ht, String key, u64 hash);
HashTableIterator HashTable_Iterate(HashTable* ht);
void HashTable_Next(HashTableIterator* it);
void HashTable_Copy(HashTable* source, HashTable* target);

#endif // NZC_NZC_HASHTABLE_ENABLED


/**
 * [SEC40] ЗАГ Логирование
 */

#ifdef NZC_LOG_ENABLED
#include <time.h>

#ifndef NZC_LOG_FILENAME
#define NZC_LOG_FILENAME "log.txt"
#endif // NZC_LOG_FILENAME

typedef struct LogContext
{
    FILE* File;
    time_t Now;
    struct tm LocalNow;
    char TimeBuf[255];
} LogContext;

static LogContext G_NZC_Log;

#ifdef NZC_LOG_DEBUG

#define LogOpen()         fopen_s(&G_NZC_Log.File, NZC_LOG_FILENAME, "w");
#define LogClose()        fclose(G_NZC_Log.File);
#define LogDebug(...)     fprintf(G_NZC_Log.File, "DEBUG: " __VA_ARGS__);
#define LogDebugMore(...) LogDebug("       " __VA_ARGS__);
#define LogDebugTime(...)                                               \
    {                                                                   \
        errno_t err;                                                    \
        G_NZC_Log.Now = time(nil);                                      \
        err = localtime_s(&G_NZC_Log.LocalNow, &G_NZC_Log.Now);         \
        err = strftime(G_NZC_Log.TimeBuf,                               \
                       sizeof(G_NZC_Log.TimeBuf),                       \
                       "[%F %T]",                                       \
                       &G_NZC_Log.LocalNow);                            \
        fputs("DEBUG: ", G_NZC_Log.File);                               \
        fputs(G_NZC_Log.TimeBuf, G_NZC_Log.File);                       \
        fprintf(G_NZC_Log.File, " " __VA_ARGS__);                       \
        UNUSED(err);                                                    \
    }

#else // NZC_LOG_DEBUG

#define LogOpen() ;
#define LogClose() ;
#define LogDebug(...) ;
#define LogDebugMore(...) ;
#define LogDebugTime(...) ;

#endif // NZC_LOG_DEBUG

#define LogError(...)     fprintf(G_NZC_Log.File, "ERROR: " __VA_ARGS__);
#endif // NZC_LOG_ENABLED


#endif // NZC_NZC_H

/**
 * [SEC03] Реализация
 * ==================
 */

#ifdef NZC_NZC_IMPLEMENTATION

/* [SEC10] РЕА База
 * ----------------
 * [SEC15] РЕА Операции над байтами
 */

u16 u16_SwapBytes(u16 v)
{
    FixedArrayU16 a = { .Value = v };
    FixedArrayU16 b;
    b.Bytes[0] = a.Bytes[1];
    b.Bytes[1] = a.Bytes[0];
    return b.Value;
}

u32 u32_SwapBytes(u32 v)
{
    FixedArrayU32 a = { .Value = v };
    FixedArrayU32 b;
    b.Bytes[0] = a.Bytes[3];
    b.Bytes[1] = a.Bytes[2];
    b.Bytes[2] = a.Bytes[1];
    b.Bytes[3] = a.Bytes[0];
    return b.Value;
}

u64 u64_SwapBytes(u64 v)
{
    FixedArrayU64 a = { .Value = v };
    FixedArrayU64 b;
    b.Bytes[0] = a.Bytes[7];
    b.Bytes[1] = a.Bytes[6];
    b.Bytes[2] = a.Bytes[5];
    b.Bytes[3] = a.Bytes[4];
    b.Bytes[4] = a.Bytes[3];
    b.Bytes[5] = a.Bytes[2];
    b.Bytes[6] = a.Bytes[1];
    b.Bytes[7] = a.Bytes[0];
    return b.Value;
}

void DebugPrintMemory(FILE* f, u8* buffer, size_t size)
{
    fprintf(stderr,
            "Debug: memory dump\n"
            "       ");

    for (size_t i = 0; i < size; i++)
    {
        fprintf(stderr, "%02X", buffer[i]);
        if (i == size - 1)
        {
            fprintf(stderr, "\n");
        }
        else if (i % 16 == 15)
        {
            fprintf(stderr, "\n       ");
        }
        else if (i % 4 == 3)
        {
            fprintf(stderr, "  ");
        }
        else
        {
            fprintf(stderr, " ");
        }
    }
}

/**
 * [SEC16] РЕА Генератор случайных числе PCG
 */

// state for global RNGs

static pcg32_random_t pcg32_global = PCG32_INITIALIZER;

// pcg32_srandom(initstate, initseq)
// pcg32_srandom_r(rng, initstate, initseq):
//     Seed the rng.  Specified in two parts, state initializer and a
//     sequence selection constant (a.k.a. stream id)

void pcg32_srandom_r(pcg32_random_t* rng, uint64_t initstate, uint64_t initseq)
{
    rng->state = 0U;
    rng->inc = (initseq << 1u) | 1u;
    pcg32_random_r(rng);
    rng->state += initstate;
    pcg32_random_r(rng);
}

void pcg32_srandom(uint64_t seed, uint64_t seq)
{
    pcg32_srandom_r(&pcg32_global, seed, seq);
}

// pcg32_random()
// pcg32_random_r(rng)
//     Generate a uniformly distributed 32-bit random number

uint32_t pcg32_random_r(pcg32_random_t* rng)
{
    uint64_t oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005ULL + rng->inc;
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

uint32_t pcg32_random()
{
    return pcg32_random_r(&pcg32_global);
}

// pcg32_boundedrand(bound):
// pcg32_boundedrand_r(rng, bound):
//     Generate a uniformly distributed number, r, where 0 <= r < bound

uint32_t pcg32_boundedrand_r(pcg32_random_t* rng, uint32_t bound)
{
    // To avoid bias, we need to make the range of the RNG a multiple of
    // bound, which we do by dropping output less than a threshold.
    // A naive scheme to calculate the threshold would be to do
    //
    //     uint32_t threshold = 0x100000000ull % bound;
    //
    // but 64-bit div/mod is slower than 32-bit div/mod (especially on
    // 32-bit platforms).  In essence, we do
    //
    //     uint32_t threshold = (0x100000000ull-bound) % bound;
    //
    // because this version will calculate the same modulus, but the LHS
    // value is less than 2^32.

    uint32_t threshold = -bound % bound;

    // Uniformity guarantees that this loop will terminate.  In practice, it
    // should usually terminate quickly; on average (assuming all bounds are
    // equally likely), 82.25% of the time, we can expect it to require just
    // one iteration.  In the worst case, someone passes a bound of 2^31 + 1
    // (i.e., 2147483649), which invalidates almost 50% of the range.  In 
    // practice, bounds are typically small and only a tiny amount of the range
    // is eliminated.
    for (;;) {
        uint32_t r = pcg32_random_r(rng);
        if (r >= threshold)
            return r % bound;
    }
}

uint32_t pcg32_boundedrand(uint32_t bound)
{
    return pcg32_boundedrand_r(&pcg32_global, bound);
}

void Random32_Init(Random32* rng)
{
    pcg32_srandom_r(&rng->Impl, rng->InitState, rng->InitSeq);
}

u32 Random32_Next(Random32* rng)
{
    return pcg32_random_r(&rng->Impl);
}

/**
 * [SEC20] РЕА Типы данных
 * -----------------------
 * [SEC21] РЕА Аллокатор типа арена
 */

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

Arena* Arena_CreateChild(Arena* parent, size_t size)
{
    assert(parent != nil && "Arena_CreateChild: Parent arena pointer must be initialized");
    Arena* child = Arena_Push(parent, 1, Arena);
    if (child == nil) { return nil; }
    u8* buffer = Arena_Push(parent, size, u8);
    if (buffer == nil) { return nil; } // TODO free child
    child->Buffer = buffer;
    child->Offset = 0;
    child->Size = size;
    return child;
}

Arena Arena_CreateCopy(const Arena* source, size_t newSize)
{
    assert(source != nil);
    assert(newSize >= source->Size);
    Arena copy = Arena_Create(newSize);
    memcpy(copy.Buffer, source->Buffer, source->Size);
    copy.Offset = source->Offset;
    return copy;
}

/**
 * [SEC23] РЕА Строки
 * [SEC231] РЕА Строка (8-бит)
 */

String String_FromChars(const char* str)
{
    size_t len = str == nil ? 0 : strlen(str);
    return (String){ .Length = len, .Str = str };
}

bool String_CopyTo(String source, char* dest, size_t destLength)
{
    if (source.Length + 1 > destLength) { return false; }
    size_t length = destLength;
    if (source.Length < destLength) { length = source.Length; }
    memcpy(dest, source.Str, length);
    dest[length] = '\0';
    return true;
}

bool String_Equal(String a, String b)
{
    if (a.Str == b.Str) { return true; }
    if (a.Length != b.Length) { return false; }
    if (a.Str == nil) { return false; }
    if (b.Str == nil) { return false; }
    return memcmp(a.Str, b.Str, a.Length) == 0;
}

i32 str_Compare(const char* s1, size_t len1,
                const char* s2, size_t len2)
{
    if (s1 == s2) { return 0; }
    if (s1 == nil && s2 != nil) { return  1; }
    if (s2 == nil && s1 != nil) { return -1; }
    for (size_t i = 0; i < len1 && i < len2; i++)
    {
        if (s1[i] > s2[i]) { return  1; }
        if (s1[i] < s2[i]) { return -1; }
    }
    if (len1 < len2) { return -1; }
    if (len1 > len2) { return  1; }
    return 0;
}

i32 String_Compare(String a, String b)
{
    return str_Compare(a.Str, a.Length, b.Str, b.Length);
}

bool String_EqualChars(String s, const char* str)
{
    if (s.Str == str) { return true; }
    if (s.Str == nil) { return false; }
    if (str == nil) { return false; }
    return memcmp(s.Str, str, s.Length) == 0;
}

char* str_SearchIgnoreCase(const char* haystack, const char* needle)
{
    if (haystack == nil || needle == nil || *needle == '\0')
    {
        return nil;
    }

    for (const char* hs = haystack; *hs; hs++)
    {
        const char* h = hs;
        const char* n = needle;
        bool match = true;

        for (;;)
        {
            if (*n == '\0') { break; }
            if (*h == '\0') { match = false; break; }

            unsigned char ch = (unsigned char)*h;
            unsigned char cn = (unsigned char)*n;
            if (tolower(ch) != tolower(cn)) { match = false; break; }

            h++;
            n++;
        }

        if (match)
        {
            return (char*)hs;
        }
    }
    return nil;
}

bool str_IsPositiveInt32(const char* s)
{
    if (s == nil) { return false; }
    if (*s == '\0') { return false; }
    const i32 limit = 9;
    i32 i = 0;
    char c;
    for (i = 0, c = s[i]; c != '\0'; i++, c = s[i])
    {
        if (!isdigit(c)) { return false; }
        if (i == limit) { return false; }
    }
    return true;
}

/**
 * [SEC232] РЕА Строка (16-бит)
 */

String16 String16_FromChars(const wchar_t* str)
{
    size_t len = str == nil ? 0 : wcslen(str);
    return (String16){ .Length = len, .Str = str };
}

bool String16_Equal(String16 a, String16 b)
{
    if (a.Str == b.Str) { return true; }
    if (a.Length != b.Length) { return false; }
    if (a.Str == nil) { return false; }
    if (b.Str == nil) { return false; }
    return memcmp(a.Str, b.Str, a.Length * sizeof(wchar_t)) == 0;
}

i32 str16_Compare(const wchar_t* s1, size_t len1,
                  const wchar_t* s2, size_t len2)
{
    if (s1 == s2) { return 0; }
    if (s1 == nil && s2 != nil) { return  1; }
    if (s2 == nil && s1 != nil) { return -1; }
    for (size_t i = 0; i < len1 && i < len2; i++)
    {
        if (s1[i] > s2[i]) { return  1; }
        if (s1[i] < s2[i]) { return -1; }
    }
    if (len1 < len2) { return -1; }
    if (len1 > len2) { return  1; }
    return 0;
}

i32 String16_Compare(String16 a, String16 b)
{
    return str16_Compare(a.Str, a.Length, b.Str, b.Length);
}

const wchar_t* str16_SearchIgnoreCase(const wchar_t* haystack, const wchar_t* needle)
{
    if (haystack == nil || needle == nil || *needle == '\0')
    {
        return nil;
    }

    for (const wchar_t* hs = haystack; *hs; hs++)
    {
        const wchar_t* h = hs;
        const wchar_t* n = needle;
        bool match = true;

        for (;;)
        {
            if (*n == '\0') { break; }
            if (*h == '\0') { match = false; break; }

            wint_t ch = (wint_t)*h;
            wint_t cn = (wint_t)*n;
            if (towlower(ch) != towlower(cn)) { match = false; break; }

            h++;
            n++;
        }

        if (match)
        {
            return hs;
        }
    }
    return nil;
}


/**
 * [SEC24] РЕА Парсинг чисел
 */

i32 i32_Parse(const char* buffer, size_t offset, size_t count)
{
    if (buffer == nil) { return 0; }
    i32 value = 0;
    i32 sign = 1;
    size_t i = offset;
    size_t size = offset + count;
    if (buffer[i] == '-')
    {
        sign = -1;
        i++;
    }
    for (; i < size; i++)
    {
        char c = buffer[i];
        if (c < '0' || c > '9') { i++; break; }
        if (value != 0) { value *= 10; }
        value += (c - '0');
    }
    value *= sign;
    return value;
}

i64 i64_Parse(const char* buffer, size_t offset, size_t count)
{
    if (buffer == nil) { return 0; }
    i64 value = 0;
    i64 sign = 1;
    size_t i = offset;
    size_t size = offset + count;
    if (buffer[i] == '-')
    {
        sign = -1;
        i++;
    }
    for (; i < size; i++)
    {
        char c = buffer[i];
        if (c < '0' || c > '9') { i++; break; }
        if (value != 0) { value *= 10; }
        value += (c - '0');
    }
    value *= sign;
    return value;
}

f32 f32_Parse(const char* buffer, size_t offset, size_t count)
{
    f32 value = 0.f;
    f32 value2 = 0.f;
    size_t i = offset;
    size_t size = offset + count;
    bool isNegative = false;
    if (buffer[i] == '-')
    {
        isNegative = true;
        i++;
    }
    if (i < size)
    {
        for (; i < size; i++)
        {
            char c = buffer[i];
            if (c == '.') { i++; break; }
            if (c < '0' || c > '9') { i++; break; }
            value += (c - '0');
            value *= 10.f;
        }
        value /= 10.f;
    }
    i32 numDecimals = 0;
    if (i < size)
    {
        for (; i < size; i++)
        {
            char c = buffer[i];
            if (c < '0' || c > '9') { break; }
            numDecimals += 1;
            value2 += (c - 48);
            value2 *= 10.f;
        }
        value2 /= 10.f;
    }
    while (numDecimals-- > 0)
    {
        value2 /= 10.f;
    }
    f32 result = value + value2;
    if (isNegative)
    {
        result *= -1.f;
    }
    return result;
}

/**
 * [SEC25] РЕА Хеши
 * [SEC251] РЕА MD5
 */

#ifdef NZC_NZC_MD5_HASH_ENABLED

static u32 g_Md5Table_S[64] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,
};

static u32 g_Md5TableK[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};

void HashMd5_Compute(HashMd5* hash, u8* buffer, size_t size)
{
    assert(hash != nil);
    assert(buffer != nil);

    const size_t messageSize     = size;
    const size_t messageTailSize = messageSize % MD5_CHUNK_SIZE;
    const size_t paddingSize     = (MD5_CHUNK_SIZE - messageTailSize > MD5_MESSAGE_LENGTH_SIZE)
                                 ? (MD5_CHUNK_SIZE - messageTailSize - MD5_MESSAGE_LENGTH_SIZE)
                                 : (2 * MD5_CHUNK_SIZE - messageTailSize - MD5_MESSAGE_LENGTH_SIZE);
    const size_t totalSize       = messageSize + paddingSize + MD5_MESSAGE_LENGTH_SIZE;
    const size_t chunkCount      = totalSize / MD5_CHUNK_SIZE;

    u8 chunk[MD5_CHUNK_SIZE];

    size_t messageBytesToCopy = messageSize;
    size_t paddingBytesToCopy = paddingSize;
    size_t messageBytesCopied = 0;
    size_t paddingBytesCopied = 0;

    u32 a0 = MD5_IV_A;
    u32 b0 = MD5_IV_B;
    u32 c0 = MD5_IV_C;
    u32 d0 = MD5_IV_D;

    for (size_t chunkId = 0; chunkId < chunkCount; chunkId++)
    {
        const size_t offset = chunkId * MD5_CHUNK_SIZE;

        // copy message bytes
        const size_t messageBytesToCopyThisChunk = messageBytesToCopy > MD5_CHUNK_SIZE
            ? MD5_CHUNK_SIZE
            : messageBytesToCopy;

        if (messageBytesToCopyThisChunk > 0)
        {
            memcpy(chunk, buffer + offset, messageBytesToCopyThisChunk);
            messageBytesCopied += messageBytesToCopyThisChunk;
            messageBytesToCopy -= messageBytesCopied;
        }

        // copy padding bytes
        const size_t chunkSizeRemains = MD5_CHUNK_SIZE - messageBytesToCopyThisChunk;
        if (chunkSizeRemains > 0)
        {
            const size_t paddingBytesToCopyThisChunk = paddingBytesToCopy > chunkSizeRemains
                ? chunkSizeRemains
                : paddingBytesToCopy;

            memset(chunk + messageBytesToCopyThisChunk,
                   MD5_PADDING_FILL_BYTE,
                   paddingBytesToCopyThisChunk);

            if (paddingBytesCopied == 0)
            {
                chunk[messageBytesToCopyThisChunk] = MD5_PADDING_FIRST_BYTE;
            }

            paddingBytesCopied += paddingBytesToCopyThisChunk;
            paddingBytesToCopy -= paddingBytesCopied;

            // copy message length bytes
            if (paddingBytesCopied == paddingSize)
            {
                const u64 messageSizeInBits = messageSize * 8;
                memcpy(chunk + messageBytesToCopyThisChunk + paddingBytesToCopyThisChunk,
                       &messageSizeInBits,
                       8);
            }
        }

#ifdef NZC_NZC_MD5_HASH_DEBUG_PRINT_CHUNK_MEMORY_ENABLED
        DebugPrintMemory(stderr, chunk, MD5_CHUNK_SIZE);
#endif // NZC_NZC_MD5_HASH_DEBUG_PRINT_CHUNK_MEMORY_ENABLED

        u32* m = (u32*)chunk;
        u32 a = a0;
        u32 b = b0;
        u32 c = c0;
        u32 d = d0;
        for (u32 i = 0; i < MD5_CHUNK_SIZE; i++)
        {
            u32 f, g;
            if (i < 16)
            {
                f = (b & c) | ((~b) & d);
                g = i;
            }
            else if (i < 32)
            {
                f = (b & d) | (c & (~d));
                g = (5*i + 1) % 16;
            }
            else if (i < 48)
            {
                f = b ^ c ^ d;
                g = (3*i + 5) % 16;
            }
            else
            {
                f = c ^ (b | (~d));
                g = (7*i) % 16;
            }
            f = f + a + g_Md5TableK[i] + m[g];
            a = d;
            d = c;
            c = b;
            b = b + ((f << g_Md5Table_S[i]) | (f >> (32 - g_Md5Table_S[i])));
        }
        a0 += a;
        b0 += b;
        c0 += c;
        d0 += d;
    }

    hash->A = a0;
    hash->B = b0;
    hash->C = c0;
    hash->D = d0;
}

void HashMd5_Write(HashMd5* hash, FILE* f)
{
    assert(hash != nil);
    assert(f != nil);
    for (u32 i = 0; i < MD5_BYTE_ARRAY_SIZE; i++)
    {
        fprintf(f, "%02X", hash->Bytes[i]);
    }
}

bool HashMd5_WriteStringToBuffer(HashMd5* hash, char* outBuffer, size_t outBufferSize)
{
    assert(hash != nil);
    assert(outBuffer != nil);
    if (outBufferSize < MD5_STRING_LENGTH) { return false; }
    for (u32 i = 0; i < MD5_BYTE_ARRAY_SIZE; i++)
    {
        sprintf(outBuffer + 2*i, "%02X", hash->Bytes[i]);
    }
    return true;
}

#endif // NZC_NZC_MD5_HASH_ENABLED

/**
 * [SEC252] РЕА SHA1
 */

#ifdef NZC_NZC_SHA1_HASH_ENABLED

void HashSha1_Compute(HashSha1* hash, u8* buffer, size_t size)
{
    assert(hash != nil);
    assert(buffer != nil);

    const size_t messageSize     = size;
    const size_t messageTailSize = messageSize % SHA1_CHUNK_SIZE;
    const size_t paddingSize     = (SHA1_CHUNK_SIZE - messageTailSize > SHA1_MESSAGE_LENGTH_SIZE)
                                 ? (SHA1_CHUNK_SIZE - messageTailSize - SHA1_MESSAGE_LENGTH_SIZE)
                                 : (2 * SHA1_CHUNK_SIZE - messageTailSize - SHA1_MESSAGE_LENGTH_SIZE);
    const size_t totalSize       = messageSize + paddingSize + SHA1_MESSAGE_LENGTH_SIZE;
    const size_t chunkCount      = totalSize / SHA1_CHUNK_SIZE;

    u8 chunk[SHA1_CHUNK_SIZE];

    size_t messageBytesToCopy = messageSize;
    size_t paddingBytesToCopy = paddingSize;
    size_t messageBytesCopied = 0;
    size_t paddingBytesCopied = 0;

    u32 h[SHA1_U32_ARRAY_SIZE];
    h[0] = SHA1_IV_H0;
    h[1] = SHA1_IV_H1;
    h[2] = SHA1_IV_H2;
    h[3] = SHA1_IV_H3;
    h[4] = SHA1_IV_H4;

    for (size_t chunkId = 0; chunkId < chunkCount; chunkId++)
    {
        const size_t offset = chunkId * SHA1_CHUNK_SIZE;

        // copy message bytes
        const size_t messageBytesToCopyThisChunk = messageBytesToCopy > SHA1_CHUNK_SIZE
            ? SHA1_CHUNK_SIZE
            : messageBytesToCopy;

        if (messageBytesToCopyThisChunk > 0)
        {
            memcpy(chunk, buffer + offset, messageBytesToCopyThisChunk);
            messageBytesCopied += messageBytesToCopyThisChunk;
            messageBytesToCopy -= messageBytesCopied;
        }

        // copy padding bytes
        const size_t chunkSizeRemains = SHA1_CHUNK_SIZE - messageBytesToCopyThisChunk;
        if (chunkSizeRemains > 0)
        {
            const size_t paddingBytesToCopyThisChunk = paddingBytesToCopy > chunkSizeRemains
                ? chunkSizeRemains
                : paddingBytesToCopy;

            memset(chunk + messageBytesToCopyThisChunk,
                   SHA1_PADDING_FILL_BYTE,
                   paddingBytesToCopyThisChunk);

            if (paddingBytesCopied == 0)
            {
                chunk[messageBytesToCopyThisChunk] = SHA1_PADDING_FIRST_BYTE;
            }

            paddingBytesCopied += paddingBytesToCopyThisChunk;
            paddingBytesToCopy -= paddingBytesCopied;

            // copy message length bytes
            if (paddingBytesCopied == paddingSize)
            {
                const u64 messageSizeInBits = messageSize * 8;
                const u64 messageSizeInBitsBigEndian = u64_SwapBytes(messageSizeInBits);
                memcpy(chunk + messageBytesToCopyThisChunk + paddingBytesToCopyThisChunk,
                       &messageSizeInBitsBigEndian,
                       8);
            }
        }

#ifdef NZC_NZC_SHA1_HASH_DEBUG_PRINT_CHUNK_MEMORY_ENABLED
        DebugPrintMemory(stderr, chunk, SHA1_CHUNK_SIZE);
#endif // NZC_NZC_SHA1_HASH_DEBUG_PRINT_CHUNK_MEMORY_ENABLED

        u32* m = (u32*)chunk;
        u32 w[80];

        for (u32 i = 0; i < 16; i++)
        {
            w[i] = u32_SwapBytes(m[i]);
        }

        for (u32 i = 16; i < 80; i++)
        {
            u32 wi = w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16];
            w[i] = U32_ROTATE_BITS_LEFT(wi, 1);
        }

        u32 a = h[0];
        u32 b = h[1];
        u32 c = h[2];
        u32 d = h[3];
        u32 e = h[4];
        for (u32 i = 0; i < 80; i++)
        {
            u32 f, k;
            if (i < 20)
            {
                f = (b & c) | ((~b) & d);
                k = SHA1_K1;
            }
            else if (i < 40)
            {
                f = b ^ c ^ d;
                k = SHA1_K2;
            }
            else if (i < 60)
            {
                f = (b & c) | (b & d) | (c & d);
                k = SHA1_K3;
            }
            else
            {
                f = b ^ c ^ d;
                k = SHA1_K4;
            }
            u32 temp = U32_ROTATE_BITS_LEFT(a, 5) + f + e + k + w[i];
            e = d;
            d = c;
            c = U32_ROTATE_BITS_LEFT(b, 30);
            b = a;
            a = temp;
        }
        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
        h[4] += e;
    }

    for (u32 i = 0; i < SHA1_U32_ARRAY_SIZE; i++)
    {
        hash->Numbers[i] = h[i];
    }
}

void HashSha1_Write(HashSha1* hash, FILE* f)
{
    assert(hash != nil);
    assert(f != nil);
    for (u32 i = 0; i < SHA1_U32_ARRAY_SIZE; i++)
    {
        fprintf(f, "%08X", hash->Numbers[i]);
    }
}

bool HashSha1_WriteStringToBuffer(HashSha1* hash, char* outBuffer, size_t outBufferSize)
{
    assert(hash != nil);
    assert(outBuffer != nil);
    if (outBufferSize < SHA1_STRING_LENGTH) { return false; }
    for (u32 i = 0; i < SHA1_U32_ARRAY_SIZE; i++)
    {
        sprintf(outBuffer + 2*4*i, "%08X", hash->Numbers[i]);
    }
    return true;
}

#endif // NZC_NZC_SHA1_HASH_ENABLED

/**
 * [SEC253] РЕА SIPHASH
 */

#ifdef NZC_NZC_SIPHASH_ENABLED

#define SIPHASH_ROUND                         \
    do {                                      \
        v0 += v1;                             \
        v1 = U64_ROTATE_BITS_LEFT(v1, 13);    \
        v1 ^= v0;                             \
        v0 = U64_ROTATE_BITS_LEFT(v0, 32);    \
        v2 += v3;                             \
        v3 = U64_ROTATE_BITS_LEFT(v3, 16);    \
        v3 ^= v2;                             \
        v0 += v3;                             \
        v3 = U64_ROTATE_BITS_LEFT(v3, 21);    \
        v3 ^= v0;                             \
        v2 += v1;                             \
        v1 = U64_ROTATE_BITS_LEFT(v1, 17);    \
        v1 ^= v2;                             \
        v2 = U64_ROTATE_BITS_LEFT(v2, 32);    \
    } while (0)

void SipHash64_Compute(SipHash64* hash,
                       const void* buffer,
                       const size_t bufferSize)
{
    const u8* ni = buffer;
    const u8* kk = hash->Key;
    u64 v0 = UINT64_C(0x736f6d6570736575);
    u64 v1 = UINT64_C(0x646f72616e646f6d);
    u64 v2 = UINT64_C(0x6c7967656e657261);
    u64 v3 = UINT64_C(0x7465646279746573);
    u64 k0 = U8_ARRAY_TO_U64_LE(kk);
    u64 k1 = U8_ARRAY_TO_U64_LE(kk + 8);
    u64 m;
    const u8* end = ni + bufferSize - (bufferSize % sizeof(u64));
    const i32 left = bufferSize & 7;
    u64 b = ((u64)bufferSize) << 56;
    v3 ^= k1;
    v2 ^= k0;
    v1 ^= k1;
    v0 ^= k0;

    for (; ni != end; ni += 8)
    {
        m = U8_ARRAY_TO_U64_LE(ni);
        v3 ^= m;

        for (i32 i = 0; i < SIPHASH_C_ROUNDS; i++)
        {
            SIPHASH_ROUND;
        }

        v0 ^= m;
    }

    switch (left)
    {
    case 7: b |= ((u64)ni[6]) << 48;
    case 6: b |= ((u64)ni[5]) << 40;
    case 5: b |= ((u64)ni[4]) << 32;
    case 4: b |= ((u64)ni[3]) << 24;
    case 3: b |= ((u64)ni[2]) << 16;
    case 2: b |= ((u64)ni[1]) <<  8;
    case 1: b |= ((u64)ni[0]);
    }

    v3 ^= b;

    for (i32 i = 0; i < SIPHASH_C_ROUNDS; i++)
    {
        SIPHASH_ROUND;
    }

    v0 ^= b;
    v2 ^= 0xFF;

    for (i32 i = 0; i < SIPHASH_D_ROUNDS; i++)
    {
        SIPHASH_ROUND;
    }

    b = v0 ^ v1 ^ v2 ^ v3;
    U64_TO_U8_ARRAY_LE(hash->Digest.Bytes, b);
}

void SipHash128_Compute(SipHash128* hash,
                        const void* buffer,
                        const size_t bufferSize)
{
    const u8* ni = buffer;
    const u8* kk = hash->Key;
    u64 v0 = UINT64_C(0x736f6d6570736575);
    u64 v1 = UINT64_C(0x646f72616e646f6d);
    u64 v2 = UINT64_C(0x6c7967656e657261);
    u64 v3 = UINT64_C(0x7465646279746573);
    u64 k0 = U8_ARRAY_TO_U64_LE(kk);
    u64 k1 = U8_ARRAY_TO_U64_LE(kk + 8);
    u64 m;
    const u8* end = ni + bufferSize - (bufferSize % sizeof(u64));
    const i32 left = bufferSize & 7;
    u64 b = ((u64)bufferSize) << 56;
    v3 ^= k1;
    v2 ^= k0;
    v1 ^= k1;
    v0 ^= k0;
    v1 ^= 0xEE;

    for (; ni != end; ni += 8)
    {
        m = U8_ARRAY_TO_U64_LE(ni);
        v3 ^= m;

        for (i32 i = 0; i < SIPHASH_C_ROUNDS; i++)
        {
            SIPHASH_ROUND;
        }

        v0 ^= m;
    }

    switch (left)
    {
    case 7: b |= ((u64)ni[6]) << 48;
    case 6: b |= ((u64)ni[5]) << 40;
    case 5: b |= ((u64)ni[4]) << 32;
    case 4: b |= ((u64)ni[3]) << 24;
    case 3: b |= ((u64)ni[2]) << 16;
    case 2: b |= ((u64)ni[1]) <<  8;
    case 1: b |= ((u64)ni[0]);
    }

    v3 ^= b;

    for (i32 i = 0; i < SIPHASH_C_ROUNDS; i++)
    {
        SIPHASH_ROUND;
    }

    v0 ^= b;
    v2 ^= 0xEE;

    for (i32 i = 0; i < SIPHASH_D_ROUNDS; i++)
    {
        SIPHASH_ROUND;
    }

    b = v0 ^ v1 ^ v2 ^ v3;
    U64_TO_U8_ARRAY_LE(hash->Digest.Bytes, b);

    v1 ^= 0xDD;

    for (i32 i = 0; i < SIPHASH_D_ROUNDS; i++)
    {
        SIPHASH_ROUND;
    }

    b = v0 ^ v1 ^ v2 ^ v3;
    U64_TO_U8_ARRAY_LE(hash->Digest.Bytes + 8, b);
}

#undef SIPHASH_ROUND

void SipHash64_Write(SipHash64* hash, FILE* f)
{
    assert(hash != nil);
    assert(f != nil);
    for (u32 i = 0; i < SIPHASH_64_SIZE; i++)
    {
        const u8 byte = hash->Digest.Bytes[SIPHASH_64_SIZE - 1 - i];
        fprintf(f, "%02X", byte);
    }
}

bool SipHash64_WriteStringToBuffer(SipHash64* hash,
                                   char* outBuffer,
                                   size_t outBufferSize)
{
    assert(hash != nil);
    assert(outBuffer != nil);
    if (outBufferSize < SIPHASH_64_SIZE * 2) { return false; }
    for (u32 i = 0; i < SIPHASH_64_SIZE; i++)
    {
        const u8 byte = hash->Digest.Bytes[SIPHASH_64_SIZE - 1 - i];
        sprintf(outBuffer + 2*i, "%02X", byte);
    }
    return true;
}

void SipHash128_Write(SipHash128* hash, FILE* f)
{
    assert(hash != nil);
    assert(f != nil);
    for (u32 i = 0; i < SIPHASH_128_SIZE; i++)
    {
        const u8 byte = hash->Digest.Bytes[SIPHASH_128_SIZE - 1 - i];
        fprintf(f, "%02X", byte);
    }
}

bool SipHash128_WriteStringToBuffer(SipHash128* hash,
                                    char* outBuffer,
                                    size_t outBufferSize)
{
    assert(hash != nil);
    assert(outBuffer != nil);
    if (outBufferSize < SIPHASH_128_SIZE * 2) { return false; }
    for (u32 i = 0; i < SIPHASH_128_SIZE; i++)
    {
        const u8 byte = hash->Digest.Bytes[SIPHASH_128_SIZE - 1 - i];
        sprintf(outBuffer + 2*i, "%02X", byte);
    }
    return true;
}

#endif // NZC_NZC_SIPHASH_ENABLED

/**
 * [SEC254] РЕА FNV1A
 */

u64 FNV1A64_Compute(const void* buffer, size_t bufferSize)
{
    u64 hash = NZC_FNV1A64_OFFSET;
    const u8* data = buffer;
    for (size_t i = 0; i < bufferSize; i++)
    {
        hash ^= (u64)data[i];
        hash *= NZC_FNV1A64_PRIME;
    }
    return hash;
}

/*
 * [SEC26] РЕА GUID
 */

static GuidGen G_NZC_GuidGen = NZC_GUIDGEN_INITIAL_VALUE;

void Guid_Init(GuidGen* gg)
{
    Random32_Init(&gg->Random);
}

bool Guid_Nextv4(void* buf)
{
    return Guid_Nextv4Ex(&G_NZC_GuidGen, buf);
}

bool Guid_Nextv4Ex(GuidGen* gg, void* buf)
{
    if (buf == nil) { return false; }
    u8* target = buf;

    *((u32*)(target + 0*4)) = Random32_Next(&gg->Random);
    *((u32*)(target + 1*4)) = Random32_Next(&gg->Random);
    *((u32*)(target + 2*4)) = Random32_Next(&gg->Random);
    *((u32*)(target + 3*4)) = Random32_Next(&gg->Random);

    // Установить биты версии 4
    // биты: 12й, 13й, 14й, 15й
    // 4 = 0000 0100 (мы ставим верхние биты в 0100)
    //
    target[6] |= 0x40; // 0100 0000
    target[6] &= 0x4F; // 0100 1111

    // Установить биты зарезервированные RFC-4122
    //
    target[8] |= 0x80; // 1000 0000 : 7й бит = 1
    target[8] &= 0xBF; // 1011 1111 : 6й бит = 0

    return true;
}

bool Guid_Nextv7(void* buf)
{
    return Guid_Nextv7Ex(&G_NZC_GuidGen, buf);
}

bool Guid_Nextv7Ex(GuidGen* gg, void* buf)
{
    if (buf == nil) { return false; }
    u8* target = buf;

    FixedArrayU64 x;
    struct timespec ts;
    if (timespec_get(&ts, TIME_UTC) != TIME_UTC) { return false; }
    x.Int32[0] = ts.tv_nsec;
    x.Int32[1] = ts.tv_sec;

    for (size_t i = 0; i < 8; i++)
    {
        target[7 - i] = x.Bytes[i];
    }

    *((u32*)(target + 2*4)) = Random32_Next(&gg->Random);
    *((u32*)(target + 3*4)) = Random32_Next(&gg->Random);

    // Установить биты версии 4
    // биты: 12й, 13й, 14й, 15й
    // 7 = 0000 0111 (мы ставим верхние биты в 0111)
    //
    target[6] |= 0x70; // 0111 0000
    target[6] &= 0x7F; // 0111 1111

    // Установить биты зарезервированные RFC-4122
    //
    target[8] |= 0x80; // 1000 0000 : 7й бит = 1
    target[8] &= 0xBF; // 1011 1111 : 6й бит = 0

    return true;
}

bool Guid_WriteStringToBuffer(void* guid,
                              char* outBuffer, size_t outBufferSize,
                              Guid_Format format)
{
    assert(guid != nil);
    assert(outBuffer != nil);
    u8* guidArr = guid;

    size_t requiredSize = GUID_STRING_LENGTH + 1;
    if (HAS_FLAG(format, Guid_FormatDash)) { requiredSize += 4; }
    if (outBufferSize < requiredSize) { return false; }

    char* formatString = "%02X";
    if (HAS_FLAG(format, Guid_FormatLowerCase)) { formatString = "%02x"; }

    char* dashFormatString = "-%02X";
    if (HAS_FLAG(format, Guid_FormatLowerCase)) { dashFormatString = "-%02x"; }

    char* outBuf = outBuffer;
    if (HAS_FLAG(format, Guid_FormatDash))
    {
        for (size_t i = 0; i < GUID_SIZE_BYTES; i++)
        {
            if (i == 4 || i == 6 || i == 8 || i == 10)
            {
                sprintf(outBuf, dashFormatString, guidArr[i]);
                outBuf += 3;
            }
            else
            {
                sprintf(outBuf, formatString, guidArr[i]);
                outBuf += 2;
            }
        }
    }
    else
    {
        for (size_t i = 0; i < GUID_SIZE_BYTES; i++)
        {
            sprintf(outBuf, formatString, guidArr[i]);
            outBuf += 2;
        }
    }

    return true;
}


/**
 * [SEC30] РЕА Контейнеры
 * ----------------------
 * [SEC31] РЕА Двое-связанный список
 */

#ifdef NZC_NZC_DOUBLY_LINKED_LIST_ENABLED

void DLNode_Init(DLNode* item)
{
    item->Next = item;
    item->Prev = item;
}

static void i_DLNode_InsertBetween(DLNode* item, DLNode* prev, DLNode* next)
{
    // ... prev >     < next ...
    //          | new |
    //
    item->Next = next;
    item->Prev = prev;
    next->Prev = item;
    prev->Next = item;
}

void DLNode_Prepend(DLNode* item, DLNode* toList)
{
    // +------------------------------------------+
    // |                                          |
    // +- list -> <- x1 -> <- x2 -> .. <- xn -> <-+
    //           ^
    //           x0
    //
    DLNode* prev = toList;
    DLNode* next = toList->Next;
    i_DLNode_InsertBetween(item, prev, next);
}

void DLNode_Append(DLNode* item, DLNode* toList)
{
    // +---------------------------------------+
    // |                                       |
    // +- list -> <- x1 -> <- x2 -> <- xn -> <-+
    //                                      ^
    //                                      xn+1
    //
    DLNode* prev = toList->Prev;
    DLNode* next = toList;
    i_DLNode_InsertBetween(item, prev, next);
}

static void i_DLNode_EraseBetween(DLNode* prev, DLNode* next)
{
    prev->Next = next;
    next->Prev = prev;
}

void DLNode_Remove(DLNode* item)
{
    i_DLNode_EraseBetween(item->Prev, item->Next);
    item->Next = item;
    item->Prev = item;
}

void DLNode_MovePrepend(DLNode* item, DLNode* toList)
{
    i_DLNode_EraseBetween(item->Prev, item->Next);
    DLNode_Prepend(item, toList);
}

void DLNode_MoveAppend(DLNode* item, DLNode* toList)
{
    i_DLNode_EraseBetween(item->Prev, item->Next);
    DLNode_Append(item, toList);
}

bool DLNode_IsEmpty(DLNode* list)
{
    return list == list->Next && list == list->Prev;
}

void DLNode_Concat(DLNode* list1, DLNode* list2)
{
    DLNode* last1  = list1->Prev;
    DLNode* first2 = list2->Next;
    DLNode* last2  = list2->Prev;

    last1->Next  = first2;
    first2->Prev = last1;

    last2->Next = list1;
    list1->Prev = last2;

    DLNode_Init(list2);
}

#endif // NZC_NZC_DOUBLY_LINKED_LIST_ENABLED

/**
 * [SEC32] РЕА Бинарное древо
 */

#ifdef NZC_NZC_BINARY_SEARCH_TREE_LIST_ENABLED

// TODO this is left recursion, rewrite as iteration
BSTResult BST_FindInt32(BST* t, size_t keyOffset, i32 key)
{
    BSTResult result = { .Node = t, .Type = BSTResultType_MatchThis };
    const i32 thisKey = *(i32*)(((void*)t) + keyOffset);
    if (key < thisKey)
    {
        if (t->Left == nil)
        {
            result.Type = BSTResultType_EmptyLeft;
            return result;
        }
        return BST_FindInt32(t->Left, keyOffset, key);
    }
    if (key > thisKey)
    {
        if (t->Right == nil)
        {
            result.Type = BSTResultType_EmptyRight;
            return result;
        }
        return BST_FindInt32(t->Right, keyOffset, key);
    }
    return result;
}

BSTResult BST_FindString(BST* t, size_t keyOffset, String key)
{
    BSTResult result = { .Node = t, .Type = BSTResultType_MatchThis };
    String thisKey = *(String*)(((void*)t) + keyOffset);
    i32 compareResult = String_Compare(key, thisKey);
    if (compareResult < 0)
    {
        if (t->Left == nil)
        {
            result.Type = BSTResultType_EmptyLeft;
            return result;
        }
        return BST_FindString(t->Left, keyOffset, key);
    }
    if (compareResult > 0)
    {
        if (t->Right == nil)
        {
            result.Type = BSTResultType_EmptyRight;
            return result;
        }
        return BST_FindString(t->Right, keyOffset, key);
    }
    return result;
}

void BST_WalkInOrder(BST* t, void* accum, BST_WalkProc proc)
{
    if (t == nil) { return; }
    BST_WalkInOrder(t->Left, accum, proc);
    proc(t, accum);
    BST_WalkInOrder(t->Right, accum, proc);
}

#endif // NZC_NZC_BINARY_SEARCH_TREE_LIST_ENABLED

/**
 * [SEC33] РЕА Хеш-Таблица
 */

#ifdef NZC_NZC_HASHTABLE_ENABLED

void HashTable_Init(HashTable* ht, size_t capacity, size_t itemSize, void* items)
{
    ht->Capacity = capacity;
    ht->ItemSize = itemSize;
    ht->Items    = items;
}

HashTableResult HashTable_Find(HashTable* ht, String key)
{
    const u64 hash = FNV1A64_Compute(key.Str, key.Length);
    return HashTable_FindWithHash(ht, key, hash);
}

HashTableResult HashTable_FindWithHash(HashTable* ht, String key, u64 hash)
{
    HashTableResult result = {0};
    // ДЕЛА в статье почемуто & вместо модуля, проверить
    //      https://benhoyt.com/writings/hash-table-in-c/
    size_t     capacity = ht->Capacity;
    size_t     index    = hash % (capacity - 1);
    size_t     count    = 0;
    size_t     itemSize = ht->ItemSize;
    u8*        items    = ht->Items;
    HashEntry* item;
    while (true)
    {
        item = (HashEntry*)(items + index * itemSize);
        String itemKey = item->Key;
        if (itemKey.Str == nil)
        {
            break;
        }
        count++;
        // ДЕЛА (оптимизация)
        //      можно хранить где-то рядом признак наличия коллизий,
        //      и если он не выставлен, то тогда сразу говорить,
        //      что запись найдена и пропускать сравнение строк
        if (String_Equal(itemKey, key))
        {
            result.Type = HashTableResultType_Match;
            result.Entry = item;
            return result;
        }
        if (count >= capacity)
        {
            result.Type = HashTableResultType_CapacityReached;
            return result;
        }
        index = index < capacity - 1 ? index + 1 : 0;
    }
    result.Type = HashTableResultType_Empty;
    result.Entry = item;
    return result;
}

size_t HashTable_Count(HashTable* ht)
{
    size_t count    = 0;
    size_t itemSize = ht->ItemSize;
    u8*    items    = ht->Items;
    for (size_t i = 0; i < ht->Capacity; i++)
    {
        HashEntry* item    = (HashEntry*)(items + i * itemSize);
        String     itemKey = item->Key;
        if (itemKey.Str != nil)
        {
            count++;
        }
    }
    return count;
}

HashTableIterator HashTable_Iterate(HashTable* ht)
{
    assert(ht != nil);
    HashTableIterator it = { .Table = ht };
    HashTable_Next(&it);
    return it;
}

void HashTable_Next(HashTableIterator* it)
{
    assert(it != nil);
    HashTable* ht       = it->Table;
    size_t     itemSize = ht->ItemSize;
    u8*        items    = ht->Items;
    if (it->Entry != nil)
    {
        it->Index++;
    }
    for (; it->Index < it->Table->Capacity; it->Index++)
    {
        HashEntry* item    = (HashEntry*)(items + it->Index * itemSize);
        String     itemKey = item->Key;
        if (itemKey.Str != nil)
        {
            it->Entry = item;
            return;
        }
    }
    it->Eof = true;
}

void HashTable_Copy(HashTable* source, HashTable* target)
{
    assert(source != nil);
    assert(target != nil);
    assert(source->Items != nil);
    assert(target->Items != nil);
    assert(target->ItemSize == source->ItemSize);
    assert(target->Capacity >= source->Capacity);
    for (HashTableIterator it = HashTable_Iterate(source);
         !it.Eof;
         HashTable_Next(&it))
    {
        HashEntry* sourceEntry = it.Entry;
        String key = sourceEntry->Key;
        if (key.Str == nil)
        {
            continue;
        }
        HashTableResult r = HashTable_Find(target, key);
        if (r.Type != HashTableResultType_CapacityReached)
        {
            memcpy(r.Entry, it.Entry, source->ItemSize);
        }
    }
}

#endif // NZC_NZC_HASHTABLE_ENABLED

#endif // NZC_NZC_IMPLEMENTATION
