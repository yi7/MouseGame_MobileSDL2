#ifndef MOUSEGAME_MAP_H
#define MOUSEGAME_MAP_H

#include "graphics.h"
#include "vector.h"
#include "sprite.h"
#include "entity.h"
#include "file.h"
#include "wall.h"
#include "mouse.h"

enum Map_State
{
    PLAY,
    PAUSE,
    PLAN,
    INACTIVE
};

typedef struct
{
    Point2D point;
    Rectangle2D frame_size;
    int frame;
    bool occupied;
} Tile;

void map_initialize_system();

void map_close_system();

void map_free_all();

void map_free_entity_tile(Entity *self);

void map_initialize_base(int map_id);

void map_load_entities(int map_id);

void map_draw_base_tile();

void map_draw_entity_tile(Entity *self);

void map_update(float touch_x, float touch_y, float untouch_x, float untouch_y);

void map_place_tile(int x, int y, int angle);

void map_remove_tile(float x, float y);

void map_touch_tile(Entity *self, Entity *other);

void map_play();

void map_stop();

void map_reset();

#endif //MOUSEGAME_MAP_H
