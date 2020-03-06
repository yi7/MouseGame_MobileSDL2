#include "wall.h"

void wall_initialize(int x, int y, int scale, int angle, SDL_RendererFlip flip)
{
    Sprite *walls = sprite_load("images/walls.bmp", 4, 64, 4);
    int sprite_frame_width = 4;
    int sprite_frame_height = 64;

    Rectangle2D wall_frame;
    wall_frame.h = scale;
    wall_frame.w = scale * sprite_frame_width / sprite_frame_height;

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
    wall->sprite = walls;
    wall->frame = 0;
    wall->free = wall_free;
    wall->draw = wall_draw;
    wall->touch = wall_touch;
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

}
