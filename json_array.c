#include "json_array.h"
#include "json_value.h"

struct JsonArray *json_array_new() {
    struct JsonArray *json_array = calloc(1, sizeof(struct JsonArray));

    if (!json_array) {
        return NULL;
    }

    json_array->data = calloc(1, sizeof(struct JsonValue));

    if (!json_array->data) {
        json_array_delete(json_array);
        return NULL;
    }

    json_array->size = 0;
    json_array->capacity = 1;
    return json_array;
}

void json_array_delete(struct JsonArray *json_array) {
    free(json_array->data);
    json_array->data = NULL;
    json_array->size = 0;
    json_array->capacity = 0;
    free(json_array);
}

static void json_array_reserve(struct JsonArray *json_array, size_t capacity) {
    // TODO: check realloc for faulures
    json_array->data =
        realloc(json_array->data, capacity * sizeof(struct JsonValue));
    json_array->capacity = capacity;
}

void json_array_insert(
    struct JsonArray *json_array,
    struct JsonValue *value,
    size_t position
) {
    if (json_array->size >= json_array->capacity) {
        json_array_reserve(
            json_array,
            json_array->capacity * JSON_ARRAY_GROW_RATE
        );
    }

    position = MIN(json_array->size, position);
    memmove(
        &json_array->data[position + 1],
        &json_array->data[position],
        json_array->size - position
    );
    memcpy(
        &json_array->data[json_array->size++],
        value,
        sizeof(struct JsonValue)
    );
}

void json_array_erase(struct JsonArray *json_array, size_t position) {
    position = MIN(json_array->size, position);
    memmove(
        &json_array->data[position],
        &json_array->data[position + 1],
        json_array->size - (position + 1)
    );
    json_array->size--;
}

struct JsonValue *json_array_at(struct JsonArray *json_array, size_t position) {
    if (position >= json_array->size) {
        return NULL;
    }

    return &json_array->data[position];
}

void json_array_print(struct JsonArray *json_array, FILE *file) {
    fprintf(file, "[\n");

    for (size_t i = 0; i < json_array->size; i++) {
        json_value_print(json_array_at(json_array, i), file);
    }
    
    fprintf(file, "]\n");
}
