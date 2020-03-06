#ifndef MOUSEGAME_MAP_H
#define MOUSEGAME_MAP_H

#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

#include "graphics.h"
#include "sprite.h"
#include "vector.h"

#define JSMN_HEADER
#include "jsmn.h"

typedef struct
{
    int columns; /**<number of columns for tilemap*/
    int rows; /**<number of rows for tilemap*/
    char map[512][3]; /**<positioning of all entities on map*/
    char name[128]; /**<name name of the map*/
} Map;

/**
 * @brief initializes the map system
 */
void map_initialize_system();

/**
 * @brief loads the map
 * @param map_id id of the map
 */
void map_load(char *filename);

void map_draw(int map_id);

#endif //MOUSEGAME_MAP_H
