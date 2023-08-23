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

#endif // JSON_OBJECT_H
