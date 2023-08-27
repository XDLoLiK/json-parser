#include "json_array.h"
#include "json_object.h"
#include "json_parser.h"
#include "json_value.h"

struct JsonParser *json_parser_new(const char *json_file_name) {
    struct JsonParser *json_parser = calloc(1, sizeof(struct JsonParser));

    if (!json_parser) {
        return NULL;
    }

    json_parser->file_name = strdup(json_file_name);

    if (!json_parser->file_name) {
        json_parser_delete(json_parser);
        return NULL;
    }

    json_parser->file = fopen(json_parser->file_name, "r");

    if (!json_parser->file) {
        json_parser_delete(json_parser);
        return NULL;
    }

    bump_line(json_parser);

    if (!json_parser->line_start) {
        json_parser_delete(json_parser);
        return NULL;
    }

    return json_parser;
}

void json_parser_delete(struct JsonParser *json_parser) {
    free(json_parser->file_name);
    json_parser->file_name = NULL;

    if (json_parser->file) {
        fclose(json_parser->file);
    }

    json_parser->file = NULL;
    free(json_parser->line_start);
    json_parser->line_start = NULL;
    json_parser->line_current = NULL;
    json_parser->line = 0;
    json_parser->column = 0;
    free(json_parser);
}

struct JsonValue *json_parser_get_value(struct JsonParser *json_parser) {
    return parse_value(json_parser);
}

static void bump_line(struct JsonParser *json_parser) {
    char *line = NULL;
    fscanf(json_parser->file, "%m[^\n]\n", &line);
    free(json_parser->line_start);
    free(json_parser->line_current);
    json_parser->line_start = line;
    json_parser->line_current = strdup(line);
    json_parser->line++;
    json_parser->column = 0;
}

static void report_error(struct JsonParser *json_parser) {
    void(json_parser);
}

static struct JsonValue *parse_value(struct JsonParser *json_parser) {
    struct JsonValue json_value = json_value_new();
    json_value->type = ValueTypeMax;

    if (isdigit(json_parser->line_current)) {
        json_value->value_type = Number;
        json_value->number = parse_number(json_parser);
        return json_value;
    }

    switch (*json_parser->line_current) {
        case '{': {
            json_value->value_type = Object;
            json_value->object = parse_object(json_parser);
            break;
        }
        case '[': {
            json_value->value_type = Array;
            json_value->object = parse_array(json_parser);
            break;
        }
        case '\"': {
            json_value->value_type = String;
            json_value->string = parse_string(json_parser);
            break;
        }
        default: {
            json_value->value_type = LiteralName;
            json_value->literal_name = parse_literal_name(json_parser);
            break;
        }
    }

    return json_value;
}

static double parse_number(struct JsonParser *json_parser) {
    double json_number = 0;
    sscanf(json_parser->buffer, "%lf", &json_number);
    return json_number;
}

static struct JsonObject *parse_object(struct JsonParser *json_parser) {
    struct JsonObject json_object = json_object_new();
    return json_object;
}

static struct JsonArray *parse_array(struct JsonParser *json_parser) {
    struct JsonArray json_array = json_array_new();
    return json_array;
}

static const char *parse_string(struct JsonParser *json_parser) {
    const char *json_string = NULL;
    size_t length = 0;

    while (*json != '\"' && *json != '\0') {
        json_string = realloc(json_string, length + 1);
        json_string[length] = *json;
        length++;
    }

    if (*json == '\0') {
        free(json_string);
        json_string = NULL;
    }

    return json_string;
}

static enum JsonLiteralNames parse_literal_name(
    struct JsonParser *json_parser
) {
    size_t json_length = strlen(json);

    for (int i = 0; i < LiteralNameMax; i++) {
        size_t literal_name_length = strlen(LITERAL_NAMES[i]);
        size_t max_length = MAX(json_length, literal_name_length);

        if (strncmp(json, LITERAL_NAMES[i], max_length) == 0) {
            json += max_length;
            return i;
        }
    }

    return LiteralNameMax;
}
