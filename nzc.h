#ifndef NZC_NZC_H
#define NZC_NZC_H

/* Nikita Zuev Common Code Library v0.8.3
 * ======================================
 */


// Зависимости от стандартной библиотеки C
//

#include <assert.h>
#include <ctype.h>
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <stdckdint.h>
#include <stddef.h>



// Стандартные типы и литералы
//

#ifndef NZC_NZC_H__COMMON_TYPES
#define NZC_NZC_H__COMMON_TYPES

#include <limits.h>
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


// Хитрый макрос линуксоидов
// (версия не требующая GCC или CLANG)
//
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


// Простая математика
//

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


// Строки
//

typedef struct String
{
    size_t Length;
    const char* Str;
} String;


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


#ifndef NZC_DOUBLY_LINKED_LIST_H
#define NZC_DOUBLY_LINKED_LIST_H


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


#ifdef DOUBLY_LINKED_LIST_IMPLEMENTATION


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

#endif // DOUBLY_LINKED_LIST_IMPLEMENTATION

#endif // NZC_DOUBLY_LINKED_LIST_H



#ifndef NZC_BINARY_SEARCH_TREE_LIST_H
#define NZC_BINARY_SEARCH_TREE_LIST_H

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


typedef void (*BST_WalkProc)(BST* it, void* accum);
void BST_WalkInOrder(BST* t, void* accum, BST_WalkProc proc)
{
    if (t == nil) { return; }
    BST_WalkInOrder(t->Left, accum, proc);
    proc(t, accum);
    BST_WalkInOrder(t->Right, accum, proc);
}

// ДЕЛА реализовать нерекурсивный итератор, с использованием стека
// ДЕЛА заценить RAD debugger

#endif // NZC_BINARY_SEARCH_TREE_LIST_H


#endif // NZC_NZC_H
