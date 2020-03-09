#include "wall.h"

void wall_initialize(int x, int y, int scale, int angle, SDL_RendererFlip flip)
{
    Sprite *wall_vertical = sprite_load("images/walls_vertical.png", 4, 64, 4);
    Sprite *wall_horizontal = sprite_load("images/walls_horizontal.png", 64, 4, 1);

    Rectangle2D wall_frame;
    Entity *wall = entity_new();

    if(angle == 0)
    {
        wall_frame.h = scale;
        wall_frame.w = scale * wall_vertical->frame_size.w / wall_vertical->frame_size.h;
        wall->position.x = x - (wall_frame.w / 2);
        wall->position.y = y;
        //wall->rect_hitbox.x = wall->position.x;
        //wall->rect_hitbox.y = wall->position.y;
        wall->rect_hitbox.w = wall_frame.w;
        wall->rect_hitbox.h = wall_frame.h;
        wall->sprite = wall_vertical;
    }
    else if(angle == 90)
    {
        wall_frame.w = scale;
        wall_frame.h = scale * wall_horizontal->frame_size.h / wall_horizontal->frame_size.w;
        //SDL_Log("%d", scale);
        //SDL_Log("%d, %d", wall_frame.h, wall_frame.w);
        wall->position.x = x;
        wall->position.y = y - (wall_frame.h / 2);
        //wall->rect_hitbox.x = x;
        //wall->rect_hitbox.y = y - (wall_frame.w / 2);
        wall->rect_hitbox.w = wall_frame.w;
        wall->rect_hitbox.h = wall_frame.h;
        wall->sprite = wall_horizontal;

        //SDL_Log("%d, %d", x, y);

    }
    wall->frame_size.w = wall_frame.w;
    wall->frame_size.h = wall_frame.h;
    wall->velocity = 0;
    wall->angle = 0;
    wall->flip = flip;
    wall->state = STOP;
    wall->type = WALL;
    wall->shape = RECTANGLE;
    wall->frame = 0;
    wall->free = wall_free;
    wall->draw = wall_draw;
    wall->touch = wall_touch;
    wall->think = NULL;
}

void wall_free(Entity *entity)
{
    entity_free(&entity);
}

void wall_draw(Entity *entity)
{
    entity_draw(entity, entity->position.x, entity->position.y, entity->angle, entity->flip);
}

void wall_touch(Entity *self, Entity *other)
{
    if(!self)
    {
        SDL_Log("wall_touch() no self entity -Error:");
    }

    if(!other)
    {
        SDL_Log("wall_touch() no other entity -Error:");
    }

    switch(other->type) {
        case MOUSE:
        case CAT:
        case TILE:
        case WALL:
        default:
            break;
    }
}

void wall_update_animal(Entity *entity)
{
    switch(entity->state)
    {
        case UP:
            entity->state = RIGHT;
            entity->position.y += entity->velocity;
            break;
        case RIGHT:
            entity->state = DOWN;
            entity->position.x -= entity->velocity;
            break;
        case DOWN:
            entity->state = LEFT;
            entity->position.y -= entity->velocity;
            break;
        case LEFT:
            entity->state = UP;
            entity->position.x += entity->velocity;
            break;
        default:
            break;
    }
}
