#include "json_array.h"
#include "json_colours.h"
#include "json_object.h"
#include "json_parser.h"
#include "json_value.h"

extern const char* LITERAL_NAMES[];

void default_handler(struct JsonParser* json_parser, const struct JsonParserError* error);
void internal_handler(struct JsonParser* json_parser, const struct JsonParserError* error);

static const struct JsonParserError PARSER_ERRORS[] = {
    { NoError, "No error.", default_handler },
    { MissingDoubleQuotes, "A '\"' expected.", default_handler },
    { LiteralNameExpected, "Literal name expected.", default_handler },
    { CommentExpected, "Comment expected", default_handler },
    { LibraryInternal, "Library internal error.", internal_handler },
    { MissingCurlyBraces, "", default_handler },
    { MissingSquareBraces, "", default_handler }
};

static struct JsonValue* parse_value(struct JsonParser* json_parser);

void default_handler(struct JsonParser* json_parser, const struct JsonParserError* error) {
    fprintf(
        stderr,
        RED("Error: ") "%s\n"
        "%zd:%zd | %s\n",
        error->message,
        json_parser->line,
        json_parser->column,
        json_parser->line_start
    );
    exit(error->code);
}

void internal_handler(
    __attribute__((unused)) struct JsonParser* json_parser,
    const struct JsonParserError* error
) {
    fprintf(stderr, RED("Error: ") "%s\n", error->message);
    exit(error->code);
}

static void error_report(struct JsonParser* json_parser, enum JsonParserErrorCodes error_code) {
    if (!json_parser) {
        error_code = LibraryInternal;
    }

    const struct JsonParserError* error = &PARSER_ERRORS[error_code];

    if (!error->handler) {
        exit(error->code);
    }

    error->handler(json_parser, error);
}

static void bump_line(struct JsonParser* json_parser) {
    JSON_ASSERT(json_parser);

    char* line = NULL;
    fscanf(json_parser->file, "%m[^\n]\n", &line);
    free((char*)json_parser->line_start);
    json_parser->line_start = line;
    json_parser->line_current = line;
    json_parser->line++;
    json_parser->column = 1;
}

static bool bump_column(struct JsonParser* json_parser, size_t columns) {
    JSON_ASSERT(json_parser);

    json_parser->column += columns;
    json_parser->line_current += columns;
    char symbol = *json_parser->line_current;

    if (symbol == '\n' || symbol == '\0') {
        bump_line(json_parser);
        return true;
    }

    return false;
}

struct JsonParser* json_parser_new(const char* json_file_name) {
    if (!json_file_name) {
        return NULL;
    }

    struct JsonParser* json_parser = calloc(1, sizeof(struct JsonParser));

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

    json_parser->line_start = NULL;
    bump_line(json_parser);
    json_parser->line = 1;

    if (!json_parser->line_start) {
        json_parser_delete(json_parser);
        return NULL;
    }

    return json_parser;
}

void json_parser_delete(struct JsonParser* json_parser) {
    JSON_ASSERT(json_parser);

    free((char *)json_parser->file_name);
    json_parser->file_name = NULL;

    if (json_parser->file) {
        fclose(json_parser->file);
    }

    json_parser->file = NULL;
    free((char *)json_parser->line_start);
    json_parser->line_start = NULL;
    json_parser->line_current = NULL;
    json_parser->line = 0;
    json_parser->column = 0;
    free(json_parser);
}

struct JsonValue* json_parser_get_value(struct JsonParser* json_parser) {
    JSON_ASSERT(json_parser);

    return parse_value(json_parser);
}

static bool require_symbol(
    struct JsonParser* json_parser,
    char symbol,
    enum JsonParserErrorCodes error_code
) {
    JSON_ASSERT(json_parser);
    JSON_ASSERT(json_parser->line_current);

    if (*json_parser->line_current != symbol) {
        error_report(json_parser, error_code);
        return false;
    }

    bump_column(json_parser, 1);
    return true;
}

static bool require_string(
    struct JsonParser* json_parser,
    const char* string,
    enum JsonParserErrorCodes error_code
) {
    JSON_ASSERT(json_parser);
    JSON_ASSERT(json_parser->line_current);
    JSON_ASSERT(string);

    size_t line_length = strlen(json_parser->line_current);
    size_t string_length = strlen(string);

    if (line_length < string_length ||
        strncmp(json_parser->line_current, string, string_length) != 0)
    {
        error_report(json_parser, error_code);
        return false;
    }
    
    bump_column(json_parser, string_length);
    return true;
}

static void skip_spaces(struct JsonParser* json_parser) {
    JSON_ASSERT(json_parser);

    while (isspace(*json_parser->line_current)) {
        bump_column(json_parser, 1);
    }
}

static void skip_comment(struct JsonParser* json_parser) {
    JSON_ASSERT(json_parser);

    require_string(json_parser, "//", CommentExpected);

    while (!bump_column(json_parser, 1)) { }
}

static double parse_number(struct JsonParser* json_parser) {
    JSON_ASSERT(json_parser);

    double json_number = 0;
    int count = 0;
    sscanf(json_parser->line_current, "%lf%n", &json_number, &count);
    bump_column(json_parser, count);
    return json_number;
}

static struct JsonObject* parse_object(struct JsonParser* json_parser) {
    JSON_ASSERT(json_parser);
    
    require_symbol(json_parser, '{', MissingCurlyBraces);
    struct JsonObject* json_object = json_object_new();
    return json_object;
}

static struct JsonArray* parse_array(struct JsonParser* json_parser) {
    JSON_ASSERT(json_parser);

    require_symbol(json_parser, '[', MissingSquareBraces);
    struct JsonArray* json_array = json_array_new();
    return json_array;
}

static const char* parse_string(struct JsonParser* json_parser) {
    JSON_ASSERT(json_parser);

    require_symbol(json_parser, '\"', MissingDoubleQuotes);
    char* json_string = NULL;
    int count = 0;
    int scanned = sscanf(json_parser->line_current, "%m[^\"]%n", &json_string, &count);

    if (scanned < 1) {
        error_report(json_parser, MissingDoubleQuotes);
        return NULL;
    }

    bump_column(json_parser, count);
    require_symbol(json_parser, '\"', MissingDoubleQuotes);
    return json_string;
}

static enum JsonLiteralNames parse_literal_name(struct JsonParser* json_parser) {
    JSON_ASSERT(json_parser);

    size_t line_length = strlen(json_parser->line_current);

    for (int i = 0; i < LiteralNameMax; i++) {
        size_t literal_name_length = strlen(LITERAL_NAMES[i]);

        if (line_length < literal_name_length) {
            continue;
        }

        if (strncmp(json_parser->line_current, LITERAL_NAMES[i], literal_name_length) == 0) {
            bump_column(json_parser, literal_name_length);
            return i;
        }
    }

    error_report(json_parser, LiteralNameExpected);
    return LiteralNameMax;
}

static struct JsonValue* parse_value(struct JsonParser* json_parser) {
    JSON_ASSERT(json_parser);

    struct JsonValue* json_value = json_value_new();
    json_value->value_type = ValueTypeMax;

retry:
    skip_spaces(json_parser);
    char symbol = *json_parser->line_current;

    if (isdigit(symbol) || symbol == '.') {
        json_value->value_type = Number;
        json_value->number = parse_number(json_parser);
        return json_value;
    }

    switch (symbol) {
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
        case '/': {
            skip_comment(json_parser);
            goto retry;
        }
        default: {
            json_value->value_type = LiteralName;
            json_value->literal_name = parse_literal_name(json_parser);
            break;
        }
    }

    return json_value;
}
