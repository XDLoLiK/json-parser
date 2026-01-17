/**
 * @file json_object.h
 * @author Goriainov Stanislav (stangoryainov@gmail.com)
 * @brief This file implements JSON object data structure
 * @version 0.1
 * @date 2023-09-02
 * 
 * @copyright Copyright (c) 2023
 */

#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

#include <stddef.h>
#include <stdio.h>

/**
 * @brief 
 */
struct JsonObjectEntry {
    const char* key;
    struct JsonValue* value;
};

/**
 * @brief 
 */
struct JsonObject {
    struct JsonObjectEntry **data;
    size_t size;
    size_t capacity;
};

/**
 * @brief 
 * 
 * @return struct JsonObject* 
 */
struct JsonObject* json_object_new();

/**
 * @brief 
 * 
 * @param json_object 
 */
void json_object_delete(struct JsonObject* json_object);

/**
 * @brief 
 * 
 * @param json_object 
 * @param key 
 * @param value 
 */
void json_object_insert(struct JsonObject* json_object, const char* key, struct JsonValue* value);

/**
 * @brief 
 * 
 * @param json_object 
 * @param key 
 */
void json_object_erase(struct JsonObject* json_object, const char* key);

/**
 * @brief 
 * 
 * @param json_object 
 * @param key 
 * @return struct JsonObjectEntry* 
 */
struct JsonObjectEntry* json_object_get(struct JsonObject* json_object, const char* key);

/**
 * @brief 
 * 
 * @param json_object 
 * @return size_t 
 */
size_t json_object_size(struct JsonObject* json_object);

/**
 * @brief 
 * 
 * @param json_object 
 * @param file 
 */
void json_object_print(struct JsonObject* json_object, FILE* file);

#endif // JSON_OBJECT_H
