#include "boulder.h"

void boulder_initialize(int x, int y)
{
    Sprite *tiles = sprite_load("images/si_tiles.png", 64, 64, 8);
    Entity *boulder = entity_new();
    boulder->active = false;
    boulder->stuck = false;
    boulder->position.x = x + (graphics_reference.wall_padding / 2);
    boulder->position.y = y + (graphics_reference.wall_padding / 2);
    boulder->frame_size.w = graphics_reference.tile_padding - graphics_reference.wall_padding;
    boulder->frame_size.h = graphics_reference.tile_padding - graphics_reference.wall_padding;
    boulder->velocity = 0;
    boulder->angle = 0;
    boulder->frame = 32;
    boulder->life = 1;
    boulder->state = STOP;
    boulder->type = BOULDER;
    boulder->sprite = tiles;

    boulder->free = boulder_free;
    boulder->draw = boulder_draw;
    boulder->touch = NULL;
    boulder->update = NULL;
    boulder->think = NULL;
}

void boulder_free(Entity *entity)
{
    entity_free(&entity);
}

void boulder_draw(Entity *entity)
{
    entity_draw(entity, entity->position.x, entity->position.y, entity->angle);
    if(entity->state == FREE)
    {
        entity->frame++;
        if(entity->frame > 47)
        {
            boulder_free(entity);
        }
    }
}