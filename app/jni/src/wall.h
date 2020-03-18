#ifndef MOUSEGAME_WALL_H
#define MOUSEGAME_WALL_H

#include "graphics.h"
#include "sprite.h"
#include "entity.h"

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

void wall_touch(Entity *self, Entity *other);

void wall_think(Entity *self);

void wall_get_off(Entity *self, Entity *other);

#endif //MOUSEGAME_WALL_H
