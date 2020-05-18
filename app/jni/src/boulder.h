#ifndef MOUSEGAME_BOULDER_H
#define MOUSEGAME_BOULDER_H

#include "graphics.h"
#include "sprite.h"
#include "entity.h"

void boulder_initialize(int x, int y);

void boulder_free(Entity *entity);

void boulder_draw(Entity *entity);

#endif //MOUSEGAME_BOULDER_H
