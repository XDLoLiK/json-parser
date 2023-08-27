#include "json_object.h"
#include "json_value.h"

struct JsonObject *json_object_new() {
    return NULL;
}

void json_object_delete(struct JsonObject *json_object) {
    (void)json_object;
}

void json_object_insert(
    struct JsonObject *json_object,
    const char *key,
    struct JsonValue *value
) {
    (void)json_object;
    (void)key;
    (void)value;
}

void json_object_erase(struct JsonObject *json_object, const char *key) {
    (void)json_object;
    (void)key;
}

struct JsonObjectEntry *json_object_get(
    struct JsonObject *json_object,
    const char *key
) {
    (void)json_object;
    (void)key;
    return NULL;
}

void json_object_print(struct JsonObject *json_object, FILE *file) {
    fprintf(file, "{\n");
    (void)json_object;
    (void)file;
    fprintf(file, "}\n");
}
