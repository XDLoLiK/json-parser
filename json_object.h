#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

enum JsonObjectEntryStatuses {
    Empty,
    Busy,
    EntryStatusesMax,
};

struct JsonObjectEntry {
    const char *key;
    struct JsonValue *value;
};

struct JsonObject {
    struct JsonObjectEntry *data;
    size_t size;
};

struct JsonObject *json_object_new();
void json_object_delete(struct JsonObject *json_object);

void json_object_insert(struct JsonObject *json_object, const char *key, struct JsonValue *value);
void json_object_erase(struct JsonObject *json_object, const char *key);
struct JsonObjectEntry *json_object_get(struct JsonObject *json_object, const char *key);
void json_object_print(struct JsonObject *json_object);

#endif // JSON_OBJECT_H
