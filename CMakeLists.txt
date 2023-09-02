cmake_minimum_required(VERSION 3.14)

project(JsonParser)

add_compile_options(-Wall -Wextra -ggdb3)

set(SOURCES
    json_array.c
    json_object.c
    json_parser.c
    json_value.c
    main.c
)

set(HEADERS
    json_array.h
    json_colours.h
    json_maths.h
    json_object.h
    json_parser.h
    json_value.h
)

add_executable(JsonParser ${SOURCES} ${HEADERS})
