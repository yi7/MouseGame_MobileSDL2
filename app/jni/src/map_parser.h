#ifndef MOUSEGAME_MAP_PARSER_H
#define MOUSEGAME_MAP_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#define JSMN_HEADER
#include "jsmn.h"

typedef struct
{
    int x;
    int y;
    int angle;
    int type;
} Entity_Placement;

typedef struct
{
    char map[285][3];
    Entity_Placement entities[63];
    int entity_count;
    int arrow_count;
    char *map_name;
    int map_id;
    bool initialized;
} Map_Detail;

extern Map_Detail *map_list;

void map_parser_initialize_system();

void map_parser_close_system();

Map_Detail *map_parser_new();

Map_Detail *map_parser_get_map(int map_id);

void map_parser_test();

void map_parser_parse_file(char *filename);

void map_parser_convert_json(char *json);

bool map_parser_jsoneq(char *json, jsmntok_t *token, char *s);


#endif //MOUSEGAME_MAP_PARSER_H
