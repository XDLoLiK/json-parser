#include "json_parser.h"
#include "json_value.h"

const char *DEFAULT_JSON = "json_test.txt";

int main(int argc, const char **argv) {
    const char *file_name = DEFAULT_JSON;

    if (argc >= 2) {
        file_name = argv[1];
    }

    struct JsonParser *json_parser = json_parser_new(file_name);
    struct JsonValue *json_value = json_parser_get_value(json_parser);
    json_value_print(json_value, stdout);
    json_parser_delete(json_parser);
    json_value_delete(json_value);
    return 0;
}
