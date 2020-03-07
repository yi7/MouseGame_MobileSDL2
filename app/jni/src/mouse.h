#ifndef MOUSEGAME_MOUSE_H
#define MOUSEGAME_MOUSE_H

#include "graphics.h"
#include "sprite.h"
#include "entity.h"

/**
 * @brief initializes mouse entity
 * @param x x position of mouse
 * @param y y position of mouse
 * @param scale scale value for resizing
 * @param angle angle of mouse
 * @param flip SDL_RendererFlip value
 */
void mouse_initialize(int x, int y, int scale, enum State state, int angle, SDL_RendererFlip flip);

/**
 * @brief frees the entity
 * @param entity the entity to be freed
 */
void mouse_free(Entity *entity);

/**
 * @brief draws the wall entity
 * @param entity the entity to be drawn
 */
void mouse_draw(Entity *entity);

/**
 * @brief touch function of the entity
 * @param self the entity to check whether it touches anything
 * @param other the entity touched
 */
void mouse_touch(Entity *self, Entity *other);

/**
 * @brief think function that runs every frame
 * @param entity the thinking entity
 */
void mouse_think(Entity *entity);

#endif //MOUSEGAME_MOUSE_H
