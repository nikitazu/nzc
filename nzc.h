#ifndef NZC_NZC_H
#define NZC_NZC_H

/**
 * Nikita Zuev Common Code Library v3.0.0
 * ======================================
 *
 * [SEC00] Навигация
 * -----------------
 *
 * [SEC01] Инструкция
 * [SEC02] АПИ
 * [SEC03] Реализация
 *
 * [SEC10] База
 * ------------
 * [SEC11] Зависимости от стандартной библиотеки C
 * [SEC12] Стандартные типы и литералы
 * [SEC13] Макросы на каждый день
 * [SEC14] Простая математика
 * [SEC15] Хеши
 *
 * [SEC20] Типы данных
 * -------------------
 * [SEC21] Аллокатор типа арена
 * [SEC22] Векторы 2D
 * [SEC23] Строки
 * [SEC24] Парсинг чисел
 *
 * [SEC30] Контейнеры
 * ------------------
 * [SEC31] Двое-связанный список
 * [SEC32] Бинарное древо
 *
 * [SEC40] Логирование (ДЕЛА думаю логи следует вытащить в отдельный файл)
 */

/**
 * [SEC01] Инструкция
 * ==================
 *
 * // Подключите заголовочный файл
 * #define NZC_NZC_IMPLEMENTATION                  // определить флаг в файле реализации
 * #define NZC_NZC_DOUBLY_LINKED_LIST_ENABLED      // подключить двое-связанный список
 * #define NZC_NZC_BINARY_SEARCH_TREE_LIST_ENABLED // подключить бинарное поисковое древо
 * #include "nzc.h"
 */

/**
 * [SEC02] АПИ
 * ===========
 *
 * [SEC10] База
 * ------------
 * [SEC11] Зависимости от стандартной библиотеки C
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
 * [SEC12] Стандартные типы и литералы
 */

#ifndef NZC_NZC_H__COMMON_TYPES
#define NZC_NZC_H__COMMON_TYPES

#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

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
 * [SEC13] Макросы на каждый день
 */

#define f32_FMT "%f"
#define f64_FMT "%f"
#define i32_FMT "%i"
#define i64_FMT "%lld"
#define u32_FMT "%i"
#define u64_FMT "%lld"

#define UNUSED(x) (void)(x)
#define NAMEOF(x) (#x)
#define KB(x) (x * 1024)

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
 * [SEC14] Простая математика
 */
#define Math_Define(T)                                  \
    inline T T##_Clamp(T v, T vmin, T vmax)             \
    {                                                   \
        return v < vmin ? vmin : (v > vmax ? vmax : v); \
    }                                                   \
                                                        \
    inline void T##_ClampAssign(T* v, T vmin, T vmax)   \
    {                                                   \
        *v = T##_Clamp(*v, vmin, vmax);                 \
    }

Math_Define(i32);
Math_Define(i64);
Math_Define(f32);
Math_Define(f64);
Math_Define(u32);
Math_Define(u64);

#undef Math_Define

/**
 * [SEC15] Хеши
 */

#define MD5_CHUNK_SIZE          64
#define MD5_MESSAGE_LENGTH_SIZE 8
#define MD5_PADDING_FIRST_BYTE  0x80
#define MD5_PADDING_FILL_BYTE   0x00

typedef struct HashMd5
{
    union
    {
        u8 Bytes[16];
        struct
        {
            u32 A;
            u32 B;
            u32 C;
            u32 D;
        };
    };
} HashMd5;

void HashMd5_Compute(HashMd5* hash, u8* buffer, size_t size)
{
    assert(hash != nil);
    assert(buffer != nil);
    fprintf(stderr, "input size is %d\n", (i32)size);
    // TODO use static initialization
    // TODO hide under flag to not waste memory if unused
    u32 s[64] = {
        7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
        5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
        4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
        6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,
    };
    u32 k[64] = {
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

    u32 a0 = 0x67452301;
    u32 b0 = 0xefcdab89;
    u32 c0 = 0x98badcfe;
    u32 d0 = 0x10325476;

    const size_t messageSize     = size;
    const size_t messageTailSize = messageSize % MD5_CHUNK_SIZE;
    const size_t paddingSize     = (MD5_CHUNK_SIZE - messageTailSize > MD5_MESSAGE_LENGTH_SIZE)
                                 ? (MD5_CHUNK_SIZE - messageTailSize - MD5_MESSAGE_LENGTH_SIZE)
                                 : (2 * MD5_CHUNK_SIZE - messageTailSize - MD5_MESSAGE_LENGTH_SIZE);
    const size_t totalSize       = messageSize + paddingSize + MD5_MESSAGE_LENGTH_SIZE;
    const size_t chunkCount      = totalSize / MD5_CHUNK_SIZE;

    fprintf(stderr, // 43 43 13 64 1
            "Debug: messageSize:     %zu\n"
            "       messageTailSize: %zu\n"
            "       paddingSize:     %zu\n"
            "       totalSize:       %zu\n"
            "       chunkCount:      %zu\n",
            messageSize,
            messageTailSize,
            paddingSize,
            totalSize,
            chunkCount);

    u8 chunk[MD5_CHUNK_SIZE];

    size_t messageBytesToCopy = messageSize;
    size_t paddingBytesToCopy = paddingSize;
    size_t messageBytesCopied = 0;
    size_t paddingBytesCopied = 0;

    for (size_t chunkId = 0; chunkId < chunkCount; chunkId++)
    {
        const size_t offset = chunkId * MD5_CHUNK_SIZE;

        fprintf(stderr,
                "Debug: chunkId:                     %zu\n"
                "       offset:                      %zu\n"
                "       messageBytesCopied (before): %zu\n"
                "       paddingBytesCopied (before): %zu\n",
                chunkId,
                offset,
                messageBytesCopied,
                paddingBytesCopied);

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

        fprintf(stderr,
                "Debug: messageBytesCopied (after): %zu\n"
                "       paddingBytesCopied (after): %zu\n",
                messageBytesCopied,
                paddingBytesCopied);

        fprintf(stderr, "Debug: CHUNK BYTES:\n");
        for (size_t i = 0; i < MD5_CHUNK_SIZE; i++)
        {
            fprintf(stderr, "%02X", chunk[i]);
            if (i % 16 == 15)
            {
                fprintf(stderr, "\n");
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
        fprintf(stderr, "\n");

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
            f = f + a + k[i] + m[g];
            a = d;
            d = c;
            c = b;
            b = b + ((f << s[i]) | (f >> (32 - s[i])));
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
    for (u32 i = 0; i < 16; i++)
    {
        fprintf(f, "%02X", hash->Bytes[i]);
    }
}


/**
 * [SEC20] Типы данных
 * -------------------
 * [SEC21] Аллокатор типа арена
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
 * [SEC22] Векторы 2D
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
 * [SEC23] Строки
 */

/**
 * Строка
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
 * [SEC24] Парсинг
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
 * [SEC30] Контейнеры
 * ------------------
 * [SEC31] Двое-связанный список
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
 * [SEC32] Бинарное древо
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
 * [SEC40] Логирование
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

/**
 * [SEC20] Типы данных
 * -------------------
 * [SEC21] Аллокатор типа арена
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

/**
 * [SEC23] Строки
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
    for (i32 i = 0; i < len1 && i < len2; i++)
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
 * [SEC24] Парсинг
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
 * [SEC30] Контейнеры
 * ------------------
 * [SEC31] Двое-связанный список
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
 * [SEC32] Бинарное древо
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

#endif // NZC_NZC_IMPLEMENTATION
