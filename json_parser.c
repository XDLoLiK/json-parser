#include "json_parser.h"

const char *LITERAL_NAMES[] = {
    "false",
    "null",
    "true",
};

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

    json_parser->current_line = calloc(JSON_PARSER_BUFFER_SIZE, sizeof(char));

    if (!json_parser->buffer) {
        json_parser_delete(json_parser);
        return NULL;
    }

    json_parser->line = 0;
    json_parser->column = 0;
    return json_parser;
}

static void json_parser_bump_buffer(struct JsonParser *json_parser) {
    if (*json_parser->current != '\0') {
        return;
    }

    size_t buffer_size = 0;
    size_t line_size = 0;

    while (buffer_size < JSON_PARSER_BUFFER_SIZE) {
        fscanf(json_parser->file, "%`[^\n]%*c%n", buffer, &line_size);
        buffer_size += line_size;
    }
}

void json_parser_delete(struct JsonParser *json_parser) {
    // TODO: poison struct data
    free(json_parser->file_name);

    if (json_parser->file) {
        fclose(json_parser->file);
    }

    free(json_parser->buffer);
    free(json_parser);
}

static struct JsonValue parse_value(struct JsonParser *json_parser) {
    struct JsonValue json_value = {
        .value_type = ValueTypeMax,
    };

    if (isdigit(json_parser->buffer)) {
        json_value.value_type = Number;
        json_value.number = parse_number(json);
        return json_value;
    }

    switch (*json_parser->buffer) {
        case '{': {
            json_value.value_type = Object;
            json_value.object = parse_object(json + 1);
            break;
        }
        case '[': {
            json_value.value_type = Array;
            json_value.object = parse_array(json + 1);
            break;
        }
        case '\"': {
            json_value.value_type = String;
            json_value.string = parse_string(json + 1);
            break;
        }
        default: {
            json_value.value_type = LiteralName;
            json_value.literal_name = parse_literal_name(json);
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

static struct JsonObject parse_object(struct JsonParser *json_parser) {
    struct JsonObject json_object = {};
    return json_object;
}

static struct JsonArray parse_array(struct JsonParser *json_parser) {
    struct JsonArray json_array = {};
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

static enum JsonLiteralNames parse_literal_name(struct JsonParser *json_parser) {
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
