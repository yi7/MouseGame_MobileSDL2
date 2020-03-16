#include "tile.h"

void tile_new(Tile *tile, int x, int y, float frame_size, int frame, bool occupied)
{
    tile->point.x = x;
    tile->point.y = y;
    tile->frame_size = frame_size;
    tile->frame = frame;
    tile->occupied = occupied;
}

void tile_new_entity(int x, int y, int scale, enum State state, int angle, SDL_RendererFlip flip)
{
    Sprite *tiles = sprite_load("images/tiles.png", 64, 64, 6);

    Entity *tile = entity_new();
    tile->position.x = x;
    tile->position.y = y;
    tile->frame_size.w = scale;
    tile->frame_size.h = scale;
    tile->rect_hitbox.x = x;
    tile->rect_hitbox.y = y;
    tile->rect_hitbox.w = scale;
    tile->rect_hitbox.h = scale;
    tile->velocity = 16;
    tile->angle = angle;
    tile->flip = flip;
    tile->type = TILE;
    tile->shape = RECTANGLE;
    tile->sprite = tiles;
    tile->state = state;
    switch(state)
    {
        case UP:
            tile->frame = 6;
            break;
        case RIGHT:
            tile->frame = 7;
            break;
        case DOWN:
            tile->frame = 8;
            break;
        case LEFT:
            tile->frame = 9;
            break;
        default:
            tile->frame = 0;
            break;
    }

    tile->free = tile_free_entity;
    tile->draw = tile_draw_entity;
    tile->touch = NULL;
    tile->update = NULL;
    tile->think = NULL;
}

void tile_free_entity(Entity *entity)
{
    entity_free(&entity);
}

void tile_draw_entity(Entity *entity)
{
    entity_draw(entity, entity->position.x, entity->position.y, entity->angle, entity->flip);
}