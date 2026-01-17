#include "json_array.h"
#include "json_object.h"
#include "json_value.h"

const char* LITERAL_NAMES[] = {
    "false",
    "null",
    "true",
};

struct JsonValue* json_value_new() {
    struct JsonValue* json_value = calloc(1, sizeof(struct JsonValue));
    return json_value;
}

void json_value_delete(struct JsonValue* json_value) {
    switch (json_value->value_type) {
        case Object: {
            json_object_delete(json_value->object);
            break;
        }
        case Array: {
            json_array_delete(json_value->array);
            break;
        }
        case String: {
            free((void *)json_value->string);
            break;
        }
        default: {
            break;
        }
    }

    free(json_value);
}

enum JsonValueTypes json_value_type(struct JsonValue* json_value) {
    return json_value->value_type;
}

void json_value_print(struct JsonValue* json_value, FILE* file) {
    switch (json_value->value_type) {
        case Object: {
            json_object_print(json_value->object, file);
            break;
        }
        case Array: {
            json_array_print(json_value->array, file);
            break;
        }
        case String: {
            fprintf(file, "\"%s\"\n", json_value->string);
            break;
        }
        case Number: {
            fprintf(file, "%lf\n", json_value->number);
            break;
        }
        case LiteralName: {
            fprintf(file, "%s\n", LITERAL_NAMES[json_value->literal_name]);
            break;
        }
        default: {
            break;
        }
    }
}
