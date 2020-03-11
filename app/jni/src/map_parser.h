#ifndef MOUSEGAME_MAP_PARSER_H
#define MOUSEGAME_MAP_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#include "jsmn.h"

typedef struct
{
    int x;
    int y;
    int angle;
    char *type;
} Entity_Placement;

typedef struct
{
    char map[285][3];
    Entity_Placement entities[63];
    char *map_name;
    int map_id;
} Map_Detail;

extern Map_Detail *map_list;

void map_parser_initialize_system();

void map_parser_close_system();

void map_parser_parse_file(char *filename);


#endif //MOUSEGAME_MAP_PARSER_H
