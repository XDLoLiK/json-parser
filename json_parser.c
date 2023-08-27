#include "json_array.h"
#include "json_object.h"
#include "json_parser.h"
#include "json_value.h"

extern const char *LITERAL_NAMES[];

static void error_report(const struct JsonParserError *parser_error) {
    (void)parser_error;
}

static const struct JsonParserError PARSER_ERRORS[] = {
    {StringExpected, "Expected JSON  string", NULL},
    {LiteralNameExpected, "Expected JSON literal name", NULL},
};

static void bump_line(struct JsonParser *json_parser) {
    char *line = NULL;
    fscanf(json_parser->file, "%m[^\n]\n", &line);
    free((void *)json_parser->line_start);
    free((void *)json_parser->line_current);
    json_parser->line_start = line;
    json_parser->line_current = line;
    json_parser->line++;
    json_parser->column = 0;
}

static void bump_column(struct JsonParser *json_parser, size_t columns) {
    json_parser->column += columns;
    json_parser->line_current += columns;
}

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
    free((void *)json_parser->file_name);
    json_parser->file_name = NULL;

    if (json_parser->file) {
        fclose(json_parser->file);
    }

    json_parser->file = NULL;
    free((void *)json_parser->line_start);
    free((void *)json_parser->line_current);
    json_parser->line_start = NULL;
    json_parser->line_current = NULL;
    json_parser->line = 0;
    json_parser->column = 0;
    free(json_parser);
}

static void skip_spaces(struct JsonParser *json_parser) {
    while (isspace(*json_parser->line_current)) {
        bump_column(json_parser, 1);
    }
}

static double parse_number(struct JsonParser *json_parser) {
    double json_number = 0;
    int count = 0;
    sscanf(json_parser->line_current, "%lf%n", &json_number, &count);
    bump_column(json_parser, count);
    return json_number;
}

static struct JsonObject *parse_object(struct JsonParser *json_parser) {
    struct JsonObject *json_object = json_object_new();
    return json_object;
}

static struct JsonArray *parse_array(struct JsonParser *json_parser) {
    struct JsonArray *json_array = json_array_new();
    return json_array;
}

static const char *parse_string(struct JsonParser *json_parser) {
    char *json_string = NULL;
    int count = 0;
    sscanf(json_parser->line_current, "%m[^\"]%n", &json_string, &count);
    bump_column(json_parser, count);

    if (*json_parser->line_current != '\"') {
        free(json_string);
        error_report(&PARSER_ERRORS[StringExpected]);
        return NULL;
    }

    return json_string;
}

static enum JsonLiteralNames parse_literal_name(
    struct JsonParser *json_parser
) {
    size_t line_length = strlen(json_parser->line_current);

    for (int i = 0; i < LiteralNameMax; i++) {
        size_t literal_name_length = strlen(LITERAL_NAMES[i]);

        if (line_length < literal_name_length) {
            error_report(&PARSER_ERRORS[LiteralNameExpected]);
            return LiteralNameMax;
        }

        if (
            strncmp(
                json_parser->line_current,
                LITERAL_NAMES[i],
                literal_name_length
            ) == 0
        ) {
            bump_column(json_parser, literal_name_length);
            return i;
        }
    }

    return LiteralNameMax;
}

static struct JsonValue *parse_value(struct JsonParser *json_parser) {
    struct JsonValue *json_value = json_value_new();
    json_value->value_type = ValueTypeMax;

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
            json_value->array = parse_array(json_parser);
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

struct JsonValue *json_parser_get_value(struct JsonParser *json_parser) {
    return parse_value(json_parser);
}
