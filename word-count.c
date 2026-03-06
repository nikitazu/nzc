// Программа `word-count`
//
// Cчитает количество вхождений каждого слова во входном тексте.
//
// Например,
//
//   cat words.txt
//   hello, world!
//   yo yo yo
//
//   word-count.exe words.txt
//   hello, 1
//   world! 1
//   yo 3
//

#define NZC_NZC_IMPLEMENTATION
#define NZC_NZC_HASHTABLE_ENABLED
#include "nzc.h"
#define NZC_NZARG_IMPLEMENTATION
#include "nzarg.h"
#include <stdio.h>


// Настройки
//

// Объём памяти, выделяемый под приложение нап старте
//
#define APP_MAIN_MEMORY_CAPACITY         (MB(10))
#define APP_WORD_MEMORY_INITIAL_CAPACITY (MB(40))

// Начальная вместимость счётчиков слов
//
//  Примечание: специально уменьшен для проверки роста таблицы
//
#define APP_WORD_INITIAL_CAPACITY     (16) //(100 * 1000)

// Размер буфера для содержания прочитанной строки
//
//  Примечание: специально уменьшен для проверки работы граничного случая,
//              когда слово обрезается в конце буфера
//
#define APP_LINE_BUFFER_SIZE  (32)


// Хранилище счётчик-слов в хештаблице
//

typedef struct WordCountEntry
{
    HashEntry Entry;
    i32       Count;
} WordCountEntry;

typedef struct WordCountStore
{
    Arena      Arena;
    HashTable* Table;
} WordCountStore;

static WordCountStore WordCountStore_Create()
{
    const size_t    capacity = APP_WORD_INITIAL_CAPACITY;
    Arena           arena    = Arena_Create(APP_WORD_MEMORY_INITIAL_CAPACITY);
    HashTable*      table    = Arena_Push(&arena, 1, HashTable);
    WordCountEntry* items    = Arena_Push(&arena, capacity, WordCountEntry);

    HashTable_Init(table, capacity, sizeof(WordCountEntry), items);
    WordCountStore store = {
        .Arena = arena,
        .Table = table,
    };

    return store;
}

static bool WordCountStore_Grow(WordCountStore* store)
{
    size_t          newTableCapacity = store->Table->Capacity * 2;
    HashTable*      newTable         = Arena_Push(&store->Arena, 1, HashTable);
    WordCountEntry* newItems         = Arena_Push(&store->Arena, newTableCapacity, WordCountEntry);
    if (newItems == nil)
    {
        return false;
    }
    newTable->Capacity = newTableCapacity;
    newTable->ItemSize = sizeof(WordCountEntry);
    newTable->Items    = newItems;
    HashTable_Copy(store->Table, newTable);
    store->Table       = newTable;
    return true;
}

static bool WordCountStore_Push(WordCountStore* store, String word)
{
    HashTableResult r = HashTable_Find(store->Table, word);
    WordCountEntry* e = r.Entry;

    if (r.Type == HashTableResultType_Match)
    {
        e->Count++;
        return true;
    }

    if (r.Type == HashTableResultType_Empty)
    {
        char* chars = Arena_Push(&store->Arena, word.Length + 1, char);
        String_CopyTo(word, chars, word.Length + 1);
        e->Entry.Key = (String){ .Str = chars, .Length = word.Length };
        e->Count = 1;
        return true;
    }

    if (r.Type == HashTableResultType_CapacityReached)
    {
        if (!WordCountStore_Grow(store))
        {
            return false;
        }

        return WordCountStore_Push(store, word);
    }

    return false;
}

// Отладочный вывод
//

static void Debug_PutBuffer(FILE* output, char* buffer, size_t size)
{
    fputs("Debug: BUFFER ", output);
    for (size_t i = 0; i < size; i++)
    {
        char c = buffer[i];
        if (c == '\r')
        {
            fputs("\\r", output);
            continue;
        }
        if (c == '\n')
        {
            fputs("\\n", output);
            continue;
        }
        if (c == '\t')
        {
            fputs("\\t", output);
            continue;
        }
        if (c == '\0')
        {
            fputs("\\0", output);
            break;
        }
        putc(buffer[i], output);
    }
    fputs("\n", output);
}

static void Debug_PutWord(FILE* output, size_t beginIndex, String word)
{
    fprintf(output, "Debug:    word %2zu:%zu '", beginIndex, word.Length);
    for (size_t i = 0; i < word.Length; i++)
    {
        putc(word.Str[i], output);
    }
    fprintf(output, "'\n");
}

typedef struct FileLineIterator
{
    size_t  Size;
    char*   Data;
    FILE*   File;
    bool    Eof;
    bool    FileOpenErrIterated;
    errno_t FileOpenErr;
} FileLineIterator;

static void File_NextLine(FileLineIterator* it);

static FileLineIterator File_IterateLines(const char* path, char* buffer, size_t bufferSize)
{
    FileLineIterator it = {
        .Size = bufferSize,
        .Data = buffer,
    };
    it.FileOpenErr = fopen_s(&it.File, path, "r");
    File_NextLine(&it);
    return it;
}

static void File_NextLine(FileLineIterator* it)
{
    assert(it != nil);
    bool done = false;

    if (it->Eof)
    {
        done = true;
    }
    else if (it->FileOpenErr != 0)
    {
        if (!it->FileOpenErrIterated)
        {
            // Разрешаем одну итерацию с ошибкой открытия файла,
            // это позволяет обрабатывать ошибку внутри цикла for
            it->FileOpenErrIterated = true;
        }
        else
        {
            // Одна итерация с ошибкой прошла - закрываем итератор,
            // чтобы не уходить в бесконечную итерацию в случае,
            // когда программист не вышел сам из цикла for при `FileOpenErr`
            it->Eof = true;
        }
        done = true;
    }
    else
    {
        void* result = fgets(it->Data, it->Size, it->File);
        if (result == nil)
        {
            it->Eof = true;
            done = true;
        }
    }

    if (done && it->File != nil)
    {
        fclose(it->File);
        it->File = nil;
    }
}

typedef struct CharBuffer
{
    size_t Size;
    char*  Data;
} CharBuffer;

static CharBuffer* CharBuffer_Create(Arena* a, size_t size)
{
    CharBuffer* buffer = Arena_Push(a, 1, CharBuffer);
    buffer->Size = size;
    buffer->Data = Arena_Push(a, size, char);
    return buffer;
}

typedef struct FileProcessState
{
    bool            DebugOutput;
    WordCountStore* WordCountStore;
    size_t          WordIndex;
    size_t          WordLength;
    bool            WordFound;
    size_t          WordLeftOverLength;
} FileProcessState;

static int WordCountEntry_Compare(const void *a, const void *b) {
    const WordCountEntry* valA = a;
    const WordCountEntry* valB = b;
    i32 countA = valA->Count;
    i32 countB = valB->Count;

    if (countA < countB) return -1;
    if (countA > countB) return 1;
    i32 stringResult = String_Compare(valA->Entry.Key, valB->Entry.Key);
    return stringResult;
}


// Точка входа
//

i32 main(i32 argc, const char** args)
{
    if (argc < 2)
    {
        fprintf(stderr, "Error: expected argument 1 to be input file path\n");
        return 1;
    }

    const char*       fileName       = args[1];
    Arena             mainArena      = Arena_Create(APP_MAIN_MEMORY_CAPACITY);
    WordCountStore    wordCountStore = WordCountStore_Create();
    CharBuffer*       buffer         = CharBuffer_Create(&mainArena, APP_LINE_BUFFER_SIZE);
    CharBuffer*       leftOverBuffer = CharBuffer_Create(&mainArena, APP_LINE_BUFFER_SIZE * 2);
    FileProcessState* state          = Arena_Push(&mainArena, 1, FileProcessState);

    state->DebugOutput = true;
    state->WordCountStore = &wordCountStore;

    for (FileLineIterator it = File_IterateLines(fileName, buffer->Data, buffer->Size);
         !it.Eof;
         File_NextLine(&it))
    {
        if (it.FileOpenErr != 0)
        {
            fprintf(stderr, "Error: failed to open file '%s'\n", fileName);
            return 1;
        }

        if (state->DebugOutput)
        {
            Debug_PutBuffer(stdout, buffer->Data, buffer->Size);
        }

        for (size_t i = 0; i < buffer->Size; i++)
        {
            char c = buffer->Data[i];
            if (c == '\0')
            {
                if (state->WordLeftOverLength > 0)
                {
                    // двойной перенос буфера не предусмотрен, пропускаем слово
                    state->WordLeftOverLength = 0;
                }
                else if (state->WordFound)
                {
                    // перенос буфера
                    state->WordLeftOverLength = i - state->WordIndex;
                    memset(leftOverBuffer->Data, '\0', leftOverBuffer->Size);
                    memcpy(leftOverBuffer->Data,
                           buffer->Data + state->WordIndex,
                           state->WordLeftOverLength);
                }
                break;
            }

            bool isWhiteSpace = false;
            if (c == ' ' || c == '\r' || c == '\n')
            {
                isWhiteSpace = true;
            }

            if (!state->WordFound && !isWhiteSpace)
            {
                state->WordFound = true;
                state->WordIndex = i;
                continue;
            }

            if (state->WordFound && isWhiteSpace)
            {
                String word;

                if (state->WordLeftOverLength > 0)
                {
                    if (state->DebugOutput)
                    {
                        fprintf(stdout, "Debug: wordLeftOverLength %zu\n",
                                state->WordLeftOverLength);
                        Debug_PutBuffer(stdout,
                                        leftOverBuffer->Data,
                                        leftOverBuffer->Size);
                    }
                    memcpy(leftOverBuffer->Data + state->WordLeftOverLength,
                           buffer->Data,
                           i);
                    word.Str = leftOverBuffer->Data;
                    word.Length = state->WordLeftOverLength + i;
                    state->WordLeftOverLength = 0;
                }
                else
                {
                    state->WordLength = i - state->WordIndex;
                    word.Str = buffer->Data + state->WordIndex;
                    word.Length = state->WordLength;
                }

                state->WordFound = false;

                if (state->DebugOutput)
                {
                    Debug_PutWord(stdout, state->WordIndex, word);
                }

                bool success = WordCountStore_Push(state->WordCountStore, word);
                if (!success)
                {
                    fprintf(stderr, "Error: word limit reached!\n");
                    it.Eof = true;
                    break;
                }
                continue;
            }
        }

        state->WordIndex = 0;
    }

    i32 wordsCount = HashTable_Count(wordCountStore.Table);
    WordCountEntry* words = Arena_Push(&mainArena, wordsCount, WordCountEntry);
    i32 wordsIndex = 0;

    for (HashTableIterator it = HashTable_Iterate(wordCountStore.Table);
         !it.Eof;
         HashTable_Next(&it))
    {
        WordCountEntry* entry = it.Entry;
        words[wordsIndex++] = *entry;
    }

    qsort(words, wordsCount, sizeof(WordCountEntry),
          WordCountEntry_Compare);

    for (i32 i = 0; i < wordsCount; i++)
    {
        WordCountEntry* entry = &words[i];
        String word = entry->Entry.Key;
        i32 count = entry->Count;
        fprintf(stdout, "%s %d\n", word.Str, count);
    }

    return 0;
}
