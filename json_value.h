#ifndef JSON_VALUE_H
#define JSON_VALUE_H

#include <stddef.h>
#include <stdio.h>

/**
 * @brief 
 * 
 */
enum JsonLiteralNames {
    False,
    Null,
    True,
    LiteralNameMax,
};

/**
 * @brief 
 * 
 */
enum JsonValueTypes {
    Object,
    Array,
    Number,
    String,
    LiteralName,
    ValueTypeMax,
};

/**
 * @brief 
 * 
 */
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

/**
 * @brief 
 * 
 * @return struct JsonValue* 
 */
struct JsonValue *json_value_new();

/**
 * @brief 
 * 
 * @param json_value 
 */
void json_value_delete(struct JsonValue *json_value);

/**
 * @brief 
 * 
 * @param json_value 
 * @return enum JsonValueTypes 
 */
enum JsonValueTypes json_value_type(struct JsonValue *json_value);

/**
 * @brief 
 * 
 * @param json_value 
 * @param file 
 */
void json_value_print(struct JsonValue *json_value, FILE *file);

#endif // JSON_VALUE_H
