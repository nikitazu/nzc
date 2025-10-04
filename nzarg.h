#ifndef NZC_NZARG_H
#define NZC_NZARG_H

/* Nikita Zuev Common Code Library - Command-Line Argument Parser v1.0.0
 * =====================================================================
 *
 * [SEC00] Навигация
 * -----------------
 *
 * [SEC01] Инструкция
 * [SEC02] АПИ
 * [SEC03] Реалиация
 */

/**
 * [SEC01] Инструкция
 * ==================
 *
 * // 1. Подключите заголовочные файлы
 *
 * #define NZC_NZC_IMPLEMENTATION
 * #include "nzc.h"
 * #define NZC_NZARG_IMPLEMENTATION
 * #include "nzarg.h"
 *
 * // 2. Определите структуру для хранения настроек
 *
 * typedef struct Settings
 * {
 *     bool   DebugMode;
 *     String Name;
 *     i32    Age;
 * } Settings;
 *
 * // 3. Настройте парсер и запустите его в точке входа
 *
 * i32 main(size_t argc, const char** argv)
 * {
 *     Settings settings = {0};
 *     NZArg_DefineBool('d', "debug", &settings.DebugMode);
 *     NZArg_DefineString('n', "name", &settings.Name);
 *     NZArg_DefineInt32('a', "age", &settings.Age);
 *     NZArgResult argResult = NZArg_Parse(0, argc, argv);
 *
 *     // Проверьте успешность вызова парсинга
 *     if (!argResult.Success)
 *     {
 *         NZArg_PrintError(stderr, argResult);
 *         return 1;
 *     }
 *
 *     // Пользуйтесь значениями настроек
 *     if (settings.DebugMode)
 *     {
 *         printf("РЕЖИМ ОТЛАДЖКИ\n");
 *     }
 *
 *     printf("Приветствую, %s! Твой возраст %d\n",
 *            settings.Name.Str,
 *            settings.Age);
 *
 *     return 0;
 * }
 */

/**
 * [SEC02] АПИ
 * ===========
 */

/**
 * Тип данных аргумента
 *
 * @value NZArgType_Bool   булевый
 * @value NZArgType_Int32  целочисленный 32-бита со знаком
 * @value NZArgType_Int64  целочисленный 64-бита со знаком
 * @value NZArgType_String строковый (см. определение типа `String` в `nzc.h`)
 */
typedef enum NZArgType
{
    NZArgType_Bool,
    NZArgType_Int32,
    NZArgType_Int64,
    NZArgType_String,
} NZArgType;

/**
 * Определение аргумента
 *
 * @field Type      тип аргумента (определяет тип значения в поле `Value`)
 * @field ShortName короткое имя (1 символ после черты)
 * @field LongName  длинное имя (множество символов после двух черт)
 * @field Value     указатель на типизированное значение аргумента
 */
typedef struct NZArg
{
    NZArgType Type;
    char      ShortName;
    String    LongName;
    void*     Value;
} NZArg;

/**
 * Вместимость парсера (максимальное кол-во аргументов)
 */
#define NZARG_RESULT_CAPACITY 255

/**
 * Парсер
 *
 * @field Count         кол-во аргументов в поле `Results`
 * @field Results       массив распознанных аргументов
 * @field RestArgCounts кол-во аргументов в поле `RestArgs`
 * @field RestArgs      массив прочих аргументов
 *
 * Примечание: структура заполняется вызовом `NZArg_Parse`
 */
typedef struct NZArgParser
{
    size_t  Count;
    NZArg   Results[NZARG_RESULT_CAPACITY];
    size_t* RestArgCount;
    String* RestArgVals;
} NZArgParser;

/**
 * Синглтон парсера
 */
NZArgParser g_NZArgParser;

/**
 * Определяет булевый аргумент
 *
 * @param shortName короткое имя аргумента
 * @param longName  длинное имя аргумента
 * @param value     указатель на значение аргумента (инициализируется в 0 при вызове)
 *
 * Функция оперирует на синглтоне `g_NZArgParser`
 */
void NZArg_DefineBool(char shortName, const char* longName, bool* value);

/**
 * Определяет целочисленный 32-битный со знаком аргумент
 *
 * @param shortName короткое имя аргумента
 * @param longName  длинное имя аргумента
 * @param value     указатель на значение аргумента (инициализируется в 0 при вызове)
 *
 * Функция оперирует на синглтоне `g_NZArgParser`
 */
void NZArg_DefineInt32(char shortName, const char* longName, i32* value);

/**
 * Определяет целочисленный 64-битный со знаком аргумент
 *
 * @param shortName короткое имя аргумента
 * @param longName  длинное имя аргумента
 * @param value     указатель на значение аргумента (инициализируется в 0 при вызове)
 *
 * Функция оперирует на синглтоне `g_NZArgParser`
 */
void NZArg_DefineInt64(char shortName, const char* longName, i64* value);

/**
 * Определяет строковый аргумент
 *
 * @param shortName короткое имя аргумента
 * @param longName  длинное имя аргумента
 * @param value     указатель на значение аргумента (инициализируется в 0 при вызове)
 *
 * Функция оперирует на синглтоне `g_NZArgParser`
 * Примечание: см. определение типа `String` в `nzc.h`
 */
void NZArg_DefineString(char shortName, const char* longName, String* value);

/**
 * Определяет массив для хранения остальных аргументов
 *
 * @param count  указатель на кол-во элементов в массиве
 * @param values указатель на массив
 */
void NZArg_DefineRest(size_t* count, String* values);

/**
 * Тип ошибки парсера
 *
 * @value NZArgParseError_None            нет ошибки
 * @value NZArgParseError_InvalidType     некорректный тип аргумента (ошибка программиста)
 * @value NZArgParseError_KeyWithoutValue ключ без значения (ошибка ввода)
 * @value NZArgParseError_KeyInvalidValue ключ с некорректным значением (ошибка ввода)
 */
typedef enum NZArgParseError
{
    NZArgParseError_None = 0,
    NZArgParseError_InvalidType,
    NZArgParseError_KeyWithoutValue,
    NZArgParseError_KeyInvalidValue,
} NZArgParseError;

/**
 * Результат парсера
 *
 * @field Success   признак успеха
 * @field InputPos  позиция во входном массиве аргументов
 * @field ParserPos позиция в результирующем массиве рагрументов
 * @field Error     тип ошибки парсера
 */
typedef struct NZArgParseResult
{
    bool            Success;
    size_t          InputPos;
    size_t          ParserPos;
    NZArgParseError Error;
} NZArgParseResult;

/**
 * Парсит входящий массив аргументов командной строки `args` от `startPos` до `endPos`
 *
 * @param startPos позиция в массиве аргументов, с которой начинается парсинг
 * @param endPos   позиция в массиве аргументов, на которой заканчивается парсинг (не включительно)
 * @param args     массив аргументов
 * @return         результат обработки
 *
 * Функция оперирует на синглтоне `g_NZArgParser`
 * Перед вызовом, следует определить аргументы через функции семейства NZArg_Define*:
 * - `NZArg_DefineBool`
 * - `NZArg_DefineInt32`
 * - `NZArg_DefineInt64`
 * - `NZArg_DefineString`
 */
NZArgParseResult NZArg_Parse(size_t startPos, size_t endPos, const char** args);

/**
 * Печатает ошибку из результата прасера `result` в файл `f`
 *
 * @param f      файл для печати
 * @param result результат, содержащий ошибку
 *
 * Запрещено вызывать данную функцию на результате,
 * который не является ошибкой, чтобы избежать некорректного
 * поведения перед вызовом необходимо проверить,
 * что значение поля `Success` структуры `result` - истинно.
 */
void NZArg_PrintError(FILE* f, NZArgParseResult result);

/**
 * Добавляет определение агрумента в парсер `parser`
 *
 * @param parser    парсер
 * @param type      тип аргумента
 * @param shortName короткое имя аргумента
 * @param longName  длинное имя аргумента
 * @param value     указатель на значение аргумента
 *
 * Внутренний АПИ, используется для общей реализации функций семейства NZArg_Define*
 */
void NZArgParser_Add(
    NZArgParser* parser,
    NZArgType type,
    char shortName,
    const char* longName,
    void* value);

#endif // NZC_NZARG_H


/**
 * [SEC03] Реализация
 * ==================
 */

#ifdef NZC_NZARG_IMPLEMENTATION

static NZArgParser g_NZArgParser;

void NZArg_DefineBool(char shortName, const char* longName, bool* value)
{
    NZArgParser_Add(&g_NZArgParser, NZArgType_Bool, shortName, longName, value);
    *((bool*)value) = false;
}

void NZArg_DefineInt32(char shortName, const char* longName, i32* value)
{
    NZArgParser_Add(&g_NZArgParser, NZArgType_Int32, shortName, longName, value);
    *((i32*)value) = 0;
}

void NZArg_DefineInt64(char shortName, const char* longName, i64* value)
{
    NZArgParser_Add(&g_NZArgParser, NZArgType_Int64, shortName, longName, value);
    *((i64*)value) = 0;
}

void NZArg_DefineString(char shortName, const char* longName, String* value)
{
    NZArgParser_Add(&g_NZArgParser, NZArgType_String, shortName, longName, value);
    *((String*)value) = (String){0};
}

void NZArg_DefineRest(size_t* count, String* values)
{
    g_NZArgParser.RestArgCount = count;
    g_NZArgParser.RestArgVals = values;
    *count = 0;
}

void NZArgParser_Add(
    NZArgParser* parser,
    NZArgType type,
    char shortName,
    const char* longName,
    void* value)
{
    assert(parser != nil);
    assert(parser->Count < NZARG_RESULT_CAPACITY);
    if (parser->Count >= NZARG_RESULT_CAPACITY) { return; }
    String longNameString = String_FromChars(longName);
    NZArg arg = {
        .Type = type,
        .ShortName = shortName,
        .LongName = longNameString,
        .Value = value,
    };
    parser->Results[parser->Count++] = arg;
}

NZArgParseResult NZArg_Parse(size_t argStart, size_t argCount, const char** argValues)
{
    const String trueString = String_FromChars("true");
    const String falseString = String_FromChars("false");
    NZArgParseResult r = {0};
    for (r.InputPos = argStart;
         r.Error == NZArgParseError_None && r.InputPos < argCount;
         r.InputPos++)
    {
        String inputArg = String_FromChars(argValues[r.InputPos]);
        bool isShortName = inputArg.Length > 0 && inputArg.Str[0] == '-' && inputArg.Str[1] != '-';
        bool isLongName  = inputArg.Length > 0 && inputArg.Str[0] == '-' && inputArg.Str[1] == '-';
        if (isShortName || isLongName)
        {
            String longName = {0};
            if (isLongName)
            {
                longName = String_FromChars(inputArg.Str + 2);
            }
            bool isMatch = false;
            for (r.ParserPos = 0;
                 !isMatch && r.ParserPos < g_NZArgParser.Count;
                 r.ParserPos++)
            {
                NZArg* resultArg = &g_NZArgParser.Results[r.ParserPos];
                if (isShortName)
                {
                    isMatch = resultArg->ShortName == inputArg.Str[1];
                }
                else
                {
                    isMatch = String_Equal(resultArg->LongName, longName);
                }
                if (!isMatch) { continue; }

                size_t valueId = r.InputPos + 1;
                String valueString = {0};
                if (valueId < argCount)
                {
                    valueString = String_FromChars(argValues[valueId]);
                }

                if (resultArg->Type != NZArgType_Bool && valueId > argCount)
                {
                    r.Error = NZArgParseError_KeyWithoutValue;
                    break;
                }

                if (resultArg->Type == NZArgType_Bool)
                {
                    bool boolValue = true;
                    if (valueId < argCount)
                    {
                        if (valueString.Length == 0)
                        {
                            r.Error = NZArgParseError_KeyInvalidValue;
                            break;
                        }
                        if (valueString.Str[0] != '-')
                        {
                            if (String_Equal(trueString, valueString))
                            {
                                boolValue = true;
                            }
                            else if (String_Equal(falseString, valueString))
                            {
                                boolValue = false;
                            }
                            else
                            {
                                r.Error = NZArgParseError_KeyInvalidValue;
                                break;
                            }
                        }
                    }
                    *((bool*)resultArg->Value) = boolValue;
                    break;
                }

                if (resultArg->Type == NZArgType_Int32)
                {
                    i32 valueI32 = i32_Parse(valueString.Str, 0, valueString.Length);
                    *((i32*)resultArg->Value) = valueI32;
                    break;
                }

                if (resultArg->Type == NZArgType_Int64)
                {
                    i64 valueI64 = i64_Parse(valueString.Str, 0, valueString.Length);
                    *((i64*)resultArg->Value) = valueI64;
                    break;
                }

                if (resultArg->Type == NZArgType_String)
                {
                    *((String*)resultArg->Value) = valueString;
                    break;
                }

                r.Error = NZArgParseError_InvalidType;
                break;
            }
        }
        else if (g_NZArgParser.RestArgCount != nil
                 && *g_NZArgParser.RestArgCount < NZARG_RESULT_CAPACITY
                 && g_NZArgParser.RestArgVals != nil)
        {
            size_t restArgCount = *g_NZArgParser.RestArgCount;
            g_NZArgParser.RestArgVals[restArgCount] = inputArg;
            *g_NZArgParser.RestArgCount = restArgCount + 1;
        }
    }
    r.Success = r.Error == NZArgParseError_None;
    return r;
}

void NZArg_PrintError(FILE* f, NZArgParseResult result)
{
    assert(!result.Success);
    assert(result.Error != NZArgParseError_None);

    const char* message = "UnknownError";
    switch (result.Error)
    {
    case NZArgParseError_InvalidType:     message = "InvalidType"; break;
    case NZArgParseError_KeyWithoutValue: message = "KeyWithoutValue"; break;
    case NZArgParseError_KeyInvalidValue: message = "KeyInvalidValue"; break;
    default: break;
    }

    fprintf(f,
            "Error: invalid command line input\n"
            "  arg: %zu\n"
            "  pos: %zu\n"
            "  msg: %s (%d)\n",
            result.InputPos,
            result.ParserPos,
            message,
            result.Error);
}

#endif // NZC_NZARG_IMPLEMENTATION
