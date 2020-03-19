#include "wall.h"

void wall_initialize(int x, int y, int frame, int frame_size, int angle, Entity_Type type)
{
    Sprite *wall_vertical = sprite_load("images/walls_vertical.png", 4, 64, 4);
    Sprite *wall_horizontal = sprite_load("images/walls_horizontal.png", 64, 4, 1);
    Entity *wall = entity_new();


    switch(angle)
    {
        case 0:
            wall->position.x = x;
            wall->position.y = y - (graphics_reference.wall_padding / 2);
            wall->frame_size.w = frame_size;
            wall->frame_size.h = graphics_reference.wall_padding;
            wall->sprite = wall_horizontal;
            break;
        case -90:
            wall->position.x = x - (graphics_reference.wall_padding / 2);
            wall->position.y = y;
            wall->frame_size.w = graphics_reference.wall_padding;
            wall->frame_size.h = frame_size;
            wall->sprite = wall_vertical;
            break;
        default:
            return;
    }

    wall->active = false;
    wall->stuck = false;
    wall->velocity = 0;
    wall->angle = 0;
    wall->frame = frame;
    wall->life = 1;
    wall->state = STOP;
    wall->type = type;
    wall->free = wall_free;
    wall->draw = wall_draw;
    wall->touch = NULL;
    wall->update = NULL;
    wall->think = NULL;
}

void wall_free(Entity *entity)
{
    entity_free(&entity);
}

void wall_draw(Entity *entity)
{
    entity_draw(entity, entity->position.x, entity->position.y, entity->angle);
}
