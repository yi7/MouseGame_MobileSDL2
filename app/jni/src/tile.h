#ifndef MOUSEGAME_TILE_H
#define MOUSEGAME_TILE_H

#include "vector.h"
#include "sprite.h"

typedef struct
{
    Point2D point; /**<tile position*/
    int frame_size; /**<frame size of tile*/
    int frame; /**<frame of tile*/
} Tile;

/**
 * @brief returns a pointer to an empty tile structure
 * @return NULL on error or a valid tile pointer
 */
Tile *tile_new(int x, int y, int frame_size, int frame);

#endif //MOUSEGAME_TILE_H
