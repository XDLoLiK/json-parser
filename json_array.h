#ifndef JSON_ARRAY_H
#define JSON_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json_math.h"

#define JSON_ARRAY_GROW_RATE 2

/**
 * @brief Represents JSON array
 * 
 */
struct JsonArray {
    struct JsonValue *data;
    size_t size;
    size_t capacity;
};

/**
 * @brief Allocates memory for a new JSON array
 * 
 * @return struct JsonArray* Pointer to the new JSON array
 */
struct JsonArray *json_array_new();

/**
 * @brief Deallocates memory for a given JSON array
 * 
 * @param json_array JSON array to delete
 */
void json_array_delete(struct JsonArray *json_array);

/**
 * @brief Inserts a JSON value into a given JSON array
 * 
 * @param json_array JSON array to insert the value into
 * @param value      JSON value to insert
 * @param position   Position to insert to
 */
void json_array_insert(
    struct JsonArray *json_array,
    struct JsonValue *value,
    size_t position
);

/**
 * @brief Erases a JSON value from a given JSON array
 * 
 * @param json_array JSON array to erase the value from
 * @param position   Position to erase from
 */
void json_array_erase(struct JsonArray *json_array, size_t position);

/**
 * @brief Provides a JSON value at a given position
 * 
 * @param json_array JSON array to get the value from
 * @param position   Position to get the value from
 * @return struct JsonValue* Pointer to a corresponding JSON value
 * or NULL in case of failure
 */
struct JsonValue *json_array_at(struct JsonArray *json_array, size_t position);

/**
 * @brief Provides the size of a given JSON array
 * 
 * @param json_array JSON array to get the size of
 * @return size_t    The size of a given JSON array
 */
size_t json_array_size(struct JsonArray *json_array);

/**
 * @brief Provides the capacity of a given JSON array
 * 
 * @param json_array JSON array to get the capacity of
 * @return size_t    The capacity of a given JSON array
 */
size_t json_array_capacity(struct JsonArray *json_array);

/**
 * @brief Prints a JSON array in a JSON format
 * 
 * @param json_array JSON array to print
 * @param file       File to print the JSON array into
 */
void json_array_print(struct JsonArray *json_array, FILE *file);

#endif // JSON_ARRAY_H
