#ifndef JSON_ARRAY_H
#define JSON_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json_math.h"

#define JSON_ARRAY_GROW_RATE 2

struct JsonArray {
    struct JsonValue *data;
    size_t size;
    size_t capacity;
};

struct JsonArray *json_array_new();
void json_array_delete(struct JsonArray *json_array);

void json_array_insert(struct JsonArray *json_array, struct JsonValue *value, size_t position);
void json_array_erase(struct JsonArray *json_array, size_t position);
struct JsonValue *json_array_at(struct JsonArray *json_array, size_t position);
void json_array_print(struct JsonArray *json_array, FILE *file);

#endif // JSON_ARRAY_H
