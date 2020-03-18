#ifndef MOUSEGAME_WALL_H
#define MOUSEGAME_WALL_H

#include "graphics.h"
#include "sprite.h"
#include "entity.h"

/**
 * @brief initializes the wall entity
 * @param x x position of wall
 * @param y y position of wall
 * @param frame frame of wall
 * @param frame_size frame size of wall
 * @param angle angle of wall
 * @param type type of wall
 */
void wall_initialize(int x, int y, int frame, int frame_size, int angle, Entity_Type type);

/**
 * @brief frees the wall entity
 * @param entity the entity to be freed
 */
void wall_free(Entity *entity);

/**
 * @brief draws the wall entity
 * @param entity the entity to be drawn
 */
void wall_draw(Entity *entity);

#endif //MOUSEGAME_WALL_H
