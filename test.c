#define DOUBLY_LINKED_LIST_IMPLEMENTATION
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
        success = T##_Eq(r, e);                                         \
        if (!success) {                                                 \
            PRINT_Vec2Failed(T##_FMT, "Vec2_Add", e, r, __FILE__, __LINE__); \
        }                                                               \
                                                                        \
        e.X = X0 - X1;                                                  \
        e.Y = Y0 - Y1;                                                  \
        r = T##_Sub(a, b);                                              \
                                                                        \
        success = T##_Eq(r, e);                                         \
        if (!success) {                                                 \
            PRINT_Vec2Failed(T##_FMT, "Vec2_Sub", e, r, __FILE__, __LINE__); \
        }                                                               \
                                                                        \
        e.X = X0 * X1;                                                  \
        e.Y = Y0 * Y1;                                                  \
        r = T##_Mul(a, b);                                              \
                                                                        \
        success = T##_Eq(r, e);                                         \
        if (!success) {                                                 \
            PRINT_Vec2Failed(T##_FMT, "Vec2_Mul", e, r, __FILE__, __LINE__); \
        }                                                               \
                                                                        \
        e.X = X0 / X1;                                                  \
        e.Y = Y0 / Y1;                                                  \
        r = T##_Div(a, b);                                              \
                                                                        \
        success = T##_Eq(r, e);                                         \
        if (!success) {                                                 \
            PRINT_Vec2Failed(T##_FMT, "Vec2_Div", e, r, __FILE__, __LINE__); \
        }                                                               \
                                                                        \
        e.X = X0 * X1;                                                  \
        e.Y = Y0 * X1;                                                  \
        r = T##_Scale(a, b.X);                                          \
                                                                        \
        success = T##_Eq(r, e);                                         \
        if (!success) {                                                 \
            PRINT_Vec2Failed(T##_FMT, "Vec2_Scale", e, r, __FILE__, __LINE__); \
        }                                                               \
                                                                        \
        e.X = X0 * X1;                                                  \
        e.Y = Y0 * X1;                                                  \
        r = a;                                                          \
        T##_ScaleAssign(&r, b.X);                                       \
                                                                        \
        success = T##_Eq(r, e);                                         \
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


void TEST_ChildArena(bool* success)
{
    Arena arena = Arena_Create(KB(10));
    assert(arena.Buffer != nil && "Failed to create parent arena");

    i32* num = Arena_Push(&arena, 1, i32);
    assert(num != nil && "Failed to push i32");

    *num = 4;

    Arena* ca1 = Arena_CreateChild(&arena, KB(3));
    Arena* ca2 = Arena_CreateChild(&arena, KB(3));
    Arena* ca3 = Arena_CreateChild(&arena, KB(5));

    if (*num != 4)
    {
        PRINT_Failed("%d", "ChildArena (number was corrupted)",
                     *num, 4);
        *success = false;
        goto cleanup;
    }

    if (ca1 == nil)
    {
        PRINT_Failed("%s", "Arena_CreateChild (normal size 1)",
                     "pointer", "nil");
        *success = false;
        goto cleanup;
    }

    if (ca2 == nil)
    {
        PRINT_Failed("%s", "Arena_CreateChild (normal size 2)",
                     "pointer", "nil");
        *success = false;
        goto cleanup;
    }

    if (ca3 != nil)
    {
        PRINT_Failed("%s", "Arena_CreateChild (too big size 3)",
                     "nil", "pointer");
        *success = false;
        goto cleanup;
    }

    f32* numbers1 = Arena_Push(ca1, 10, f32);
    if (numbers1 == nil)
    {
        PRINT_Failed("%s", "ChildArena (push 10 f32 into ca1)",
                     "pointer", "nil");
        *success = false;
        goto cleanup;
    }

    f32* numbers2 = Arena_Push(ca2, 10, f32);
    if (numbers2 == nil)
    {
        PRINT_Failed("%s", "ChildArena (push 10 f32 into ca2)",
                     "pointer", "nil");
        *success = false;
        goto cleanup;
    }

    for (i32 i = 0; i < 10; i++)
    {
        numbers1[i] = i * 2.f;
        numbers2[i] = i * 3.f;
    }

    for (i32 i = 0; i < 10; i++)
    {
        if (numbers1[i] != i * 2.f)
        {
            PRINT_Failed("%f", "ChildArena (ca1 number corrupted)",
                         i * 2.f, numbers1[i]);
            *success = false;
            goto cleanup;
        }
        if (numbers2[i] != i * 3.f)
        {
            PRINT_Failed("%f", "ChildArena (ca2 number corrupted)",
                         i * 2.f, numbers1[i]);
            *success = false;
            goto cleanup;
        }
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

    i32 compareResult = str_Compare(nil, 0, nil, 0);
    if (compareResult != 0)
    {
        PRINT_Failed("%d", "str_Compare(nil, 0, nil, 0)", 0, compareResult);
        *success = false;
    }

    compareResult = str_Compare(nil, 1, nil, 1);
    if (compareResult != 0)
    {
        PRINT_Failed("%d", "str_Compare(nil, 1, nil, 1)", 0, compareResult);
        *success = false;
    }

    compareResult = str_Compare("a", 1, nil, 0);
    if (compareResult != -1)
    {
        PRINT_Failed("%d", "str_Compare('a', 1, nil, 0)", -1, compareResult);
        *success = false;
    }

    compareResult = str_Compare(nil, 0, "a", 1);
    if (compareResult != 1)
    {
        PRINT_Failed("%d", "str_Compare(nil, 0, 'a', 1)", 1, compareResult);
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



// TEST Doubly Linked List
//

typedef struct IntNode
{
    DLNode Node;
    i32    Value;
} IntNode;

typedef struct IntList
{
    Arena*   Arena;
    IntNode* Root;
} IntList;

IntList IntList_Create(Arena* a)
{
    IntNode* root = Arena_Push(a, 1, IntNode);
    DLNode_Init(&root->Node);
    IntList list = { .Arena = a, .Root = root };
    return list;
}

void IntList_Append(IntList list, i32 value)
{
    IntNode* item = Arena_Push(list.Arena, 1, IntNode);
    item->Value = value;
    DLNode_Init(&item->Node);
    DLNode_Append(&item->Node, &list.Root->Node);
}

void TEST_DoublyLinkedListOnArena(bool* success)
{
    Arena arena = Arena_Create(KB(10));
    assert(arena.Buffer != nil && "Failed to create arena");

    IntList a = IntList_Create(&arena);
    IntList b = IntList_Create(&arena);

    IntList_Append(a, 1);
    IntList_Append(a, 2);
    IntList_Append(a, 3);

    IntList_Append(b, 10);
    IntList_Append(b, 20);
    IntList_Append(b, 30);

    i32 sum = 0;
    DL_EACH(IntNode, it, a.Root, Node) {
        sum += it->Value;
    }

    if (sum != 6)
    {
        PRINT_Failed("%d", "DoublyLinkedList on Arena (sum loop 1)", 6, sum);
        *success = false;
        goto cleanup;
    }

    sum = 0;
    DL_EACH(IntNode, it, b.Root, Node) {
        sum += it->Value;
    }

    if (sum != 60)
    {
        PRINT_Failed("%d", "DoublyLinkedList on Arena (sum loop 2)", 60, sum);
        *success = false;
        goto cleanup;
    }

    DLNode_Concat(&a.Root->Node, &b.Root->Node);

    sum = 0;
    DL_EACH(IntNode, it, a.Root, Node) {
        sum += it->Value;
    }

    if (sum != 66)
    {
        PRINT_Failed("%d", "DoublyLinkedList on Arena (sum loop 3)", 66, sum);
        *success = false;
        goto cleanup;
    }

cleanup:
    Arena_Free(&arena);
}

void TEST_DoublyLinkedListOnStack(bool* success)
{
    IntNode aList, bList;
    IntNode a[3];
    IntNode b[3];

    DLNode_Init(&aList.Node);
    DLNode_Init(&bList.Node);

    for (i32 i = 0; i < 3; i++)
    {
        DLNode_Init(&a[i].Node);
        DLNode_Append(&a[i].Node, &aList.Node);
        a[i].Value = i + 1;

        DLNode_Init(&b[i].Node);
        DLNode_Append(&b[i].Node, &bList.Node);
        b[i].Value = (i + 1) * 10;
    }

    i32 sum = 0;
    DL_EACH(IntNode, it, &aList, Node) {
        sum += it->Value;
    }

    if (sum != 6)
    {
        PRINT_Failed("%d", "DoublyLinkedList on Stack (sum loop 1)", 6, sum);
        *success = false;
        goto cleanup;
    }

    sum = 0;
    DL_EACH(IntNode, it, &bList, Node) {
        sum += it->Value;
    }

    if (sum != 60)
    {
        PRINT_Failed("%d", "DoublyLinkedList on Stack (sum loop 2)", 60, sum);
        *success = false;
        goto cleanup;
    }

    DLNode_Concat(&aList.Node, &bList.Node);

    sum = 0;
    DL_EACH(IntNode, it, &aList, Node) {
        sum += it->Value;
    }

    if (sum != 66)
    {
        PRINT_Failed("%d", "DoublyLinkedList on Stack (sum loop 3)", 66, sum);
        *success = false;
        goto cleanup;
    }

cleanup:
    { /* nothing to cleanup */ }
}


// TEST Binary Search Tree
//

typedef struct IntBst
{
    BST Node;
    i32 Key;
} IntBst;

typedef struct IntBstSumCheckList
{
    i32   Sum;
    i32   Pos;
    i32*  Keys;
    i32   KeyCount;
    bool  Failed;
    char* FailedReason;
} IntBstSumCheckList;

void IntBst_SumTreeInOrder_Callback(BST* it, void* accum)
{
    IntBstSumCheckList* checkList = accum;
    if (checkList->Failed) { return; }

    if (checkList->Pos >= checkList->KeyCount)
    {
        checkList->Failed = true;
        checkList->FailedReason = "position out of bounds";
        return;
    }

    IntBst* node = (void*)it;
    if (checkList->Keys[checkList->Pos] != node->Key)
    {
        checkList->Failed = true;
        checkList->FailedReason = "unexpected key value at position";
        return;
    }

    // i32 sum0 = checkList->Sum;
    checkList->Sum += node->Key;
    checkList->Pos++;

    // fprintf(stderr, "BST %d + %d = %d\n", sum0, node->Key, checkList->Sum);
}

void IntBst_SumTreeInOrder(IntBst* t, IntBstSumCheckList* checkList)
{
    BST_WalkInOrder(&t->Node, checkList, IntBst_SumTreeInOrder_Callback);
}

void TEST_BinarySearchTreeOnStack(bool* success)
{
    IntBst root = {0};
    BSTResult r;

    r = BST_FindInt32(&root.Node, offsetof(IntBst, Key), 0);

    if (r.Type != BSTResultType_MatchThis)
    {
        PRINT_Failed("%d", "BinarySearchTree on Stack (find 0) result type check",
                     BSTResultType_MatchThis, r.Type);
        *success = false;
        goto cleanup;
    }

    if (r.Node != &root.Node)
    {
        PRINT_Failed("%s", "BinarySearchTree on Stack (find 0) result node check",
                     "root node", "something else");
        *success = false;
        goto cleanup;
    }

    r = BST_FindInt32(&root.Node, offsetof(IntBst, Key), 5);

    if (r.Type != BSTResultType_EmptyRight)
    {
        PRINT_Failed("%d", "BinarySearchTree on Stack (find 5) result type check",
                     BSTResultType_EmptyRight, r.Type);
        *success = false;
        goto cleanup;
    }

    if (r.Node != &root.Node)
    {
        PRINT_Failed("%s", "BinarySearchTree on Stack (find 5) result node check",
                     "root node", "something else");
        *success = false;
        goto cleanup;
    }

    IntBst n5 = { .Key = 5 };
    r.Node->Right = &n5.Node;

    r = BST_FindInt32(&root.Node, offsetof(IntBst, Key), 5);

    if (r.Type != BSTResultType_MatchThis)
    {
        PRINT_Failed("%d", "BinarySearchTree on Stack (find 5') result type check",
                     BSTResultType_MatchThis, r.Type);
        *success = false;
        goto cleanup;
    }

    if (r.Node != &n5.Node)
    {
        PRINT_Failed("%s", "BinarySearchTree on Stack (find 5') result node check",
                     "n5 node", "something else");
        *success = false;
        goto cleanup;
    }

    r = BST_FindInt32(&root.Node, offsetof(IntBst, Key), -5);

    if (r.Type != BSTResultType_EmptyLeft)
    {
        PRINT_Failed("%d", "BinarySearchTree on Stack (find -5) result type check",
                     BSTResultType_EmptyRight, r.Type);
        *success = false;
        goto cleanup;
    }

    if (r.Node != &root.Node)
    {
        PRINT_Failed("%s", "BinarySearchTree on Stack (find -5) result node check",
                     "root node", "something else");
        *success = false;
        goto cleanup;
    }

    IntBst n5neg = { .Key = -5 };
    r.Node->Left = &n5neg.Node;

    r = BST_FindInt32(&root.Node, offsetof(IntBst, Key), -5);

    if (r.Type != BSTResultType_MatchThis)
    {
        PRINT_Failed("%d", "BinarySearchTree on Stack (find -5') result type check",
                     BSTResultType_MatchThis, r.Type);
        *success = false;
        goto cleanup;
    }

    if (r.Node != &n5neg.Node)
    {
        PRINT_Failed("%s", "BinarySearchTree on Stack (find -5') result node check",
                     "n5neg node", "something else");
        *success = false;
        goto cleanup;
    }

    r = BST_FindInt32(&root.Node, offsetof(IntBst, Key), 3);

    if (r.Type != BSTResultType_EmptyLeft)
    {
        PRINT_Failed("%d", "BinarySearchTree on Stack (find 3) result type check",
                     BSTResultType_EmptyRight, r.Type);
        *success = false;
        goto cleanup;
    }

    if (r.Node != &n5.Node)
    {
        PRINT_Failed("%s", "BinarySearchTree on Stack (find 3) result node check",
                     "n5 node", "something else");
        *success = false;
        goto cleanup;
    }

    IntBst n3 = { .Key = 3 };
    r.Node->Left = &n3.Node;

    r = BST_FindInt32(&root.Node, offsetof(IntBst, Key), 3);

    if (r.Type != BSTResultType_MatchThis)
    {
        PRINT_Failed("%d", "BinarySearchTree on Stack (find 3') result type check",
                     BSTResultType_MatchThis, r.Type);
        *success = false;
        goto cleanup;
    }

    if (r.Node != &n3.Node)
    {
        PRINT_Failed("%s", "BinarySearchTree on Stack (find 3') result node check",
                     "n3 node", "something else");
        *success = false;
        goto cleanup;
    }

    r = BST_FindInt32(&root.Node, offsetof(IntBst, Key), 5);

    if (r.Type != BSTResultType_MatchThis)
    {
        PRINT_Failed("%d", "BinarySearchTree on Stack (find 5'') result type check",
                     BSTResultType_MatchThis, r.Type);
        *success = false;
        goto cleanup;
    }

    if (r.Node != &n5.Node)
    {
        PRINT_Failed("%s", "BinarySearchTree on Stack (find 5'') result node check",
                     "n5 node", "something else");
        *success = false;
        goto cleanup;
    }

    i32 keyList[4] = { -5, 0, 3, 5 };
    IntBstSumCheckList checkList =
        {
            .Sum = 0,
            .Pos = 0,
            .Keys = &keyList[0],
            .KeyCount = 4,
            .Failed = false,
        };

    IntBst_SumTreeInOrder(&root, &checkList);

    if (checkList.Failed)
    {
        PRINT_Failed("%s", "BinarySearchTree on Stack (sum) checklist check",
                     "not failed", checkList.FailedReason);
        *success = false;
        goto cleanup;
    }

    if (checkList.Sum != 3)
    {
        PRINT_Failed("%d", "BinarySearchTree on Stack (sum) result check",
                     3, checkList.Sum);
        *success = false;
        goto cleanup;
    }

cleanup:
    { /* nothing to cleanup */ }
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
    TEST_ChildArena(&success);
    TEST_String(&success);

    TEST_DoublyLinkedListOnArena(&success);
    TEST_DoublyLinkedListOnStack(&success);

    TEST_BinarySearchTreeOnStack(&success);

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
