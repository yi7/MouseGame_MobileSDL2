#ifndef MOUSEGAME_TILE_H
#define MOUSEGAME_TILE_H

#include "vector.h"
#include "sprite.h"
#include "entity.h"

//This tile structure is only for drawing the background tiles.
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

/**
 * @brief initiazlies the tile entity
 * @param x x position of the tile
 * @param y y position of the tile
 * @param scale scale value for resizing
 * @param state state of the tile
 * @param angle angle of the tile
 * @param flip SDL_RendererFlip value
 */
void tile_new_entity(int x, int y, int scale, enum State state, int angle, SDL_RendererFlip flip);

/**
 * @brief frees the tile entity
 * @param entity the entity to be freed
 */
void tile_free_entity(Entity *entity);

/**
 * @brief draws the tile entity
 * @param entity the entity to be drawn
 */
void tile_draw_entity(Entity *entity);

#endif //MOUSEGAME_TILE_H