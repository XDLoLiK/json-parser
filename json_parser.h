#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "json_array.h"
#include "json_object.h"
#include "json_value.h"

#define JSON_PARSER_BUFFER_SIZE 4096

struct JsonParser {
    const char *file_name;
    FILE *file;
    const char *current_line;
    size_t line;
    size_t column;
};

struct JsonParser *json_parser_new(const char *json_file_name);
struct JsonValue *json_parser_get_value(struct JsonParser *json_parser);
void json_parser_delete(struct JsonParser *json_parser);

#endif // JSON_PARSER_H
