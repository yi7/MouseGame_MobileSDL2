#ifndef MOUSEGAME_MOUSE_H
#define MOUSEGAME_MOUSE_H

#include "graphics.h"
#include "sprite.h"
#include "entity.h"
#include "wall.h"
#include "map.h"

/**
 * @brief initializes mouse entity
 * @param x x position of mouse
 * @param y y position of mouse
 * @param scale scale value for resizing
 * @param state which direction mouse is facing
 * @param angle angle of mouse
 * @param flip SDL_RendererFlip value
 */
void mouse_initialize(int x, int y, int scale, int angle, SDL_RendererFlip flip);

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
 * @brief update function of the mouse entity when colliding with a wall
 * @param self the mouse to update
 */
void mouse_update(Entity *self);

/**
 * @brief think function that runs every frame
 * @param self the thinking entity
 */
void mouse_think(Entity *self);

/**
 * @brief move mouse forward by velocity
 * @param self the mouse to move forward
 */
void mouse_step_forward(Entity *self);

/**
 * @brief move mouse backward by velocity
 * @param self the mouse to move backward
 */
void mouse_step_backward(Entity *self);

/**
 * @brief this function will move the mouse just enough so that it won't be colliding with the wall. Will only be called when colliding with wall
 * @param self the mouse to move off
 * @param other the wall to step off of
 */
void mouse_step_off(Entity *self);


/**
 * @brief checks what is in front of mouse
 * @param self the mouse to check
 * @return NULL if nothing. Otherwise the entity in front
 */
Type mouse_check_front(Entity *self);

#endif //MOUSEGAME_MOUSE_H
