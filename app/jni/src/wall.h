#ifndef MOUSEGAME_WALL_H
#define MOUSEGAME_WALL_H

#include "graphics.h"
#include "sprite.h"
#include "entity.h"

/**
 * @brief initializes wall entity
 * @param x x position of wall
 * @param y y position of wall
 * @param scale scale value for resizing
 * @param angle angle of wall
 * @param flip SDL_RendererFlip value
 */
void wall_initialize(int x, int y, int scale, int angle, SDL_RendererFlip flip);

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

/**
 * @brief touch function of the wall entity
 * @param self the wall entity
 * @param other the entity that collided with the wall
 */
void wall_touch(Entity *self, Entity *other);

/**
 * @brief update function of the wall entity
 * @param entity the entity that collided with the wall
 */
void wall_update_animal(Entity *entity);

#endif //MOUSEGAME_WALL_H
