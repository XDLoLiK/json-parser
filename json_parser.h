/**
 * @file json_parser.h
 * @author Goriainov Stanislav (stangoryainov@gmail.com)
 * @brief This file implements JSON parser itself
 * @version 0.1
 * @date 2023-09-02
 * 
 * @copyright Copyright (c) 2023
 */

#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define JSON_ASSERT(expr)                               \
do {                                                    \
    if (!(expr)) {                                      \
        error_report(json_parser, LibraryInternal);     \
    }                                                   \
} while (0)

/**
 * @brief 
 */
enum JsonParserErrorCodes {
    NoError,
    MissingDoubleQuotes,
    LiteralNameExpected,
    CommentExpected,
    LibraryInternal,
    MissingCurlyBraces,
    MissingSquareBraces,
    ParserErrorCodeMax,
};

/**
 * @brief 
 */
struct JsonParserError {
    enum JsonParserErrorCodes code;
    const char *message;
    void (*handler)(void);
};

/**
 * @brief 
 */
struct JsonParser {
    const char *file_name;
    FILE *file;
    const char *line_start;
    const char *line_current;
    size_t line;
    size_t column;
};

/**
 * @brief 
 * 
 * @param json_file_name 
 * @return struct JsonParser* 
 */
struct JsonParser *json_parser_new(const char *json_file_name);

/**
 * @brief 
 * 
 * @param json_parser 
 */
void json_parser_delete(struct JsonParser *json_parser);

/**
 * @brief 
 * 
 * @param json_parser 
 * @return struct JsonValue* 
 */
struct JsonValue *json_parser_get_value(struct JsonParser *json_parser);

#endif // JSON_PARSER_H
