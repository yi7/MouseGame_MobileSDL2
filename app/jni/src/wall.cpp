#include "wall.h"

void wall_initialize(int x, int y, int scale, int angle, SDL_RendererFlip flip)
{
    Sprite *walls = sprite_load("images/walls.png", 4, 64, 4);

    Rectangle2D wall_frame;
    wall_frame.h = scale;
    wall_frame.w = scale * walls->frame_size.w / walls->frame_size.h;

    Entity *wall = entity_new();
    wall->position.x = x - (wall_frame.w / 2);
    wall->position.y = y;
    wall->frame_size.w = wall_frame.w;
    wall->frame_size.h = wall_frame.h;
    wall->rect_hitbox.w = wall_frame.w;
    wall->rect_hitbox.h = wall_frame.h;
    wall->velocity = 0;
    wall->angle = angle;
    wall->flip = flip;
    wall->state = STOP;
    wall->type = WALL;
    wall->shape = RECTANGLE;
    wall->sprite = walls;
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
    if(entity_intersect(self, other))
    {
        switch(other->type) {
            case MOUSE:
            case CAT:
                wall_update_animal(other);
                break;
            case TILE:
            case WALL:
            default:
                break;
        }
    }
}

void wall_update_animal(Entity *entity)
{
    switch(entity->state)
    {
        case UP:
            entity->state = RIGHT;
            break;
        case RIGHT:
            entity->state = DOWN;
            break;
        case DOWN:
            entity->state = LEFT;
            break;
        case LEFT:
            entity->state = UP;
            break;
        default:
            break;
    }
}
