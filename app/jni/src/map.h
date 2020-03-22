#ifndef MOUSEGAME_MAP_H
#define MOUSEGAME_MAP_H

#include "graphics.h"
#include "vector.h"
#include "sprite.h"
#include "entity.h"
#include "file.h"
#include "wall.h"
#include "mouse.h"
#include "cat.h"

#define WHITE_TILE 2
#define GREY_TILE 3
#define BLACK_TILE 4
#define HOME_TILE 5
#define ARROW_TILE 7

enum Map_State
{
    PLAY,
    PAUSE,
    PLAN,
    TEST,
    INACTIVE,
    LOSE,
    WIN,
    EDIT
};

enum Edit_Type
{
    NONE,
    ETILE_HOME,
    ETILE_HOLE,
    ETILE_REMOVE,
    EMOUSE_NORMAL,
    EMOUSE_REMOVE,
    ECAT_NORMAL,
    ECAT_REMOVE,
    EWALL_V,
    EWALL_H,
    EWALL_REMOVE_V,
    EWALL_REMOVE_H
};

typedef struct
{
    Point2D point;
    Rectangle2D frame_size;
    int frame;
    bool occupied;
} Tile;

/**
 * @brief initializes map system
 */
void map_initialize_system();

/**
 * @brief closes map system
 */
void map_close_system();

/**
 * @brief frees all entity and tiles on map
 */
void map_free_all();

/**
 * @brief free function of arrow tile entity
 * @param self
 */
void map_free_entity_tile(Entity *self);

/**
 * @brief initializes the base tiles, you cannot interact with these
 * @param map_id id of map
 */
void map_initialize_base(int map_id, Map_State state);

/**
 * @brief loads all entities onto the map
 * @param map_id id of map
 */
void map_load_entities(int map_id);

/**
 * @brief draws the base tiles
 */
void map_draw_base_tile();

/**
 * @brief draw function of arrow tile entity
 * @param self
 */
void map_draw_entity_tile(Entity *self);

/**
 * @brief updates the map based on where player touches
 * @param touch_x x position where finger pressed
 * @param touch_y y position where finger pressed
 * @param untouch_x x position where finger lifted
 * @param untouch_y y position where finger lifted
 */
void map_update(float touch_x, float touch_y, float untouch_x, float untouch_y);

/**
 * @brief places arrow tile entity on specific tile
 * @param x x position of arrow tile
 * @param y y position of arrow tile
 * @param angle angle of arrow tile
 */
void map_place_tile(int x, int y, int angle, int frame);

/**
 * @brief frees arrow tile on specific tile
 * @param x x position where finger pressed
 * @param y y position where finger pressed
 */
void map_remove_tile(float x, float y);

/**
 * @brief touch function called when arrow tile is pressed
 * @param self temporary entity for finger press
 * @param other arrow tile to be removed
 */
void map_touch_tile(Entity *self, Entity *other);

void map_remove_active_entity(float x, float y);

void map_touch_active_entity(Entity *self, Entity *other);

void map_remove_wall(float x, float y, Edit_Type type, int tile_index);

void map_touch_wall(Entity *self, Entity *other);

int map_check_wall_edit_hitbox(float touch_x, float touch_y, Edit_Type type);

/**
 * @brief changes entity state to move
 */
void map_play();

/**
 * @brief changes entity state to stop
 */
void map_stop();

/**
 * @brief frees all entity on map and places them back to their initial placement
 */
void map_reset();

void map_test();

void map_change_edit_type(Edit_Type type);

void map_initialize_home_tile(int x, int y, int angle);

void map_update_home_tile(Entity *self);

void map_change_state(Map_State state);

int map_get_state();

void map_set_state(Map_State state);

void map_save_edit();

void map_reset_edit();

#endif //MOUSEGAME_MAP_H
