#ifndef MOUSEGAME_MOUSE_H
#define MOUSEGAME_MOUSE_H

#include "graphics.h"
#include "sprite.h"
#include "entity.h"

void mouse_initialize(int x, int y, int frame_size, int angle, Entity_Type type);

void mouse_free(Entity *entity);

void mouse_draw(Entity *entity);

void mouse_touch(Entity *self, Entity *other);

void mouse_update(Entity *self);

void mouse_think(Entity *self);

void mouse_rotate(Entity *self);

void mouse_step_off(Entity *self, Entity *other);

void mouse_find_path(Entity *self);

#endif //MOUSEGAME_MOUSE_H
