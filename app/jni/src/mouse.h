#ifndef MOUSEGAME_MOUSE_H
#define MOUSEGAME_MOUSE_H

#include <math.h>

#include "graphics.h"
#include "sprite.h"
#include "entity.h"

/**
 * @brief initializes the mouse entity depending on type
 * @param x x position of mouse
 * @param y y position of mouse
 * @param frame_size frame size of mouse
 * @param angle angle of mouse
 * @param type type of mouse
 */
void mouse_initialize(int x, int y, int frame_size, int angle, Entity_Type type);

/**
 * @brief free function of mouse
 * @param entity entity to free
 */
void mouse_free(Entity *entity);

/**
 * @brief draw function of mouse
 * @param entity entity to draw
 */
void mouse_draw(Entity *entity);

/**
 * @brief touch function of mouse
 * @param self mouse entity
 * @param other some entity it touched
 */
void mouse_touch(Entity *self, Entity *other);

void mouse_drill_touch(Entity *self, Entity *other);

/**
 * @brief update function of mouse
 * @param self entity to update
 */
void mouse_update(Entity *self);

/**
 * @brief think function of mouse
 * @param self mouse entity
 */
void mouse_think(Entity *self);

/**
 * @brief called when mouse touches wall, places mouse off the wall
 * @param self mouse entity
 * @param other wall entity
 */
void mouse_step_off(Entity *self, Entity *other);

/**
 * @brief determines the direction the mouse can take and updates the angle
 * @param self mouse entity
 */
void mouse_find_path(Entity *self);

void mouse_find_path_2(Entity *self);

#endif //MOUSEGAME_MOUSE_H
