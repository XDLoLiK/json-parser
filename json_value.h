#ifndef JSON_VALUE_H
#define JSON_VALUE_H

enum JsonLiteralNames {
    False, // "false"
    Null, // "null"
    True, // "true"
    LiteralNameMax,
};

enum JsonValueTypes {
    Object,
    Array,
    Number,
    String,
    LiteralName,
    ValueTypeMax,
};

struct JsonValue {
    enum JsonValueTypes value_type;
    union {
        struct JsonObject *object;
        struct JsonArray *array;
        double number;
        const char *string;
        enum JsonLiteralNames literal_name;
    };
};

struct JsonValue *json_value_new();
void json_value_delete(struct JsonValue *json_value);

#endif // JSON_VALUE_H
