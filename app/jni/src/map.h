#ifndef MOUSEGAME_MAP_H
#define MOUSEGAME_MAP_H

#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

#include "graphics.h"
#include "sprite.h"
#include "entity.h"
#include "vector.h"
#include "wall.h"
#include "mouse.h"
#include "tile.h"
#include "map_parser.h"

extern float TILE_FRAME;
extern int TPL; //Tiles per line
extern float WALL_FRAME_WIDTH;
extern const int MAP_TILE_COLUMNS;
extern const int MAP_TILE_ROWS;
extern Tile *tile_list;
extern int ARROW_LIMIT;

enum Map_State
{
    PLAY,
    PAUSE,
    PLAN,
    INACTIVE
};

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
 * @brief closes the map system
 */
void map_close_system();

void map_free_all();

void map_initialize_window(int map_id);

void map_initialize_base(int map_id);

/**
 * @brief loads the map
 * @param map_id id of the map
 */
void map_load_entities(int map_id);

/**
 * @brief draws the tiles on screen
 * @param map_id
 */
void map_draw_tiles();

/**
 * @brief checks if the entity is fully on the tile
 * @param entity the entity to check
 * @return returns true if it's on a tile, otherwise false
 */
bool map_check_on_tile(Entity *entity);

void map_play();

void map_stop();

void map_reset(int button_id);

void map_update(int tile_location);

bool map_check_plan_state();

int map_get_arrow_count();

void map_increment_arrow_count();

void map_decrement_arrow_count();

#endif //MOUSEGAME_MAP_H
