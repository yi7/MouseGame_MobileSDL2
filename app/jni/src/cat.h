#ifndef MOUSEGAME_CAT_H
#define MOUSEGAME_CAT_H

#include "graphics.h"
#include "sprite.h"
#include "entity.h"

/**
 * @brief initializes the cat entity depending on type
 * @param x x position of cat
 * @param y y position of cat
 * @param frame_size frame size of cat
 * @param angle angle of cat
 * @param type type of cat
 */
void cat_initialize(int x, int y, int frame_size, int angle, Entity_Type type);

/**
 * @brief free function of cat
 * @param entity entity to free
 */
void cat_free(Entity *entity);

/**
 * @brief draw function of cat
 * @param entity entity to draw
 */
void cat_draw(Entity *entity);

/**
 * @brief touch function of cat
 * @param self cat entity
 * @param other some entity it touched
 */
void cat_touch(Entity *self, Entity *other);

/**
 * @brief update function of cat
 * @param self entity to update
 */
void cat_update(Entity *self);

/**
 * @brief think function of cat
 * @param self cat entity
 */
void cat_think(Entity *self);

/**
 * @brief called when cat touches wall, places cat off the wall
 * @param self cat entity
 * @param other wall entity
 */
void cat_step_off(Entity *self, Entity *other);

/**
 * @brief determines the direction the cat can take and updates the angle
 * @param self cat entity
 */
void cat_find_path(Entity *self);

#endif //MOUSEGAME_CAT_H
