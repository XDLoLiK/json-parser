#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief 
 * 
 */
enum JsonParserErrorCodes {
    NoError,
    StringExpected,
    LiteralNameExpected,
    ParserErrorCodeMax,
};

/**
 * @brief 
 * 
 */
struct JsonParserError {
    enum JsonParserErrorCodes error_code;
    const char *error_message;
    void (*error_handler)(void);
};

/**
 * @brief 
 * 
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
