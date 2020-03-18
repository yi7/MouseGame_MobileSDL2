#ifndef MOUSEGAME_MAP_H
#define MOUSEGAME_MAP_H

#include "graphics.h"
#include "vector.h"
#include "sprite.h"
#include "entity.h"
#include "file.h"
#include "wall.h"

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

void map_initialize_base(int map_id);

void map_load_entities(int map_id);

void map_draw_tiles();

#endif //MOUSEGAME_MAP_H
