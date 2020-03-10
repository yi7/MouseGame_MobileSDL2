#include "tile.h"

Tile *tile_new(int x, int y, int frame_size, int frame)
{
    Tile *tile;

    tile->point.x = x;
    tile->point.y = y;
    tile->frame_size = frame_size;
    tile->frame = frame;

    return tile;
}

