#include "json_array.h"
#include "json_object.h"
#include "json_value.h"

struct JsonValue *json_value_new(enum JsonValueTypes json_value_type) {
    struct JsonValue *json_value = calloc(1, sizeof(struct JsonValue));
    return json_value;
}

void json_value_delete(struct JsonValue *json_value) {
    switch (json_value.value_type) {
        case Object: {
            json_object_delete(json_value->object);
            break;
        }
        case Array: {
            json_array_delete(json_value->array);
            break;
        }
        case String: {
            free(json_value->string);
            break;
        }
    }

    free(json_value);
}
