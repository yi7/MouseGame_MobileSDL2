#include "mouse.h"

void mouse_initialize(int x, int y, int scale, enum State state, int angle, SDL_RendererFlip flip)
{
    Sprite *animals = sprite_load("images/animals.png", 64, 64, 8);

    Rectangle2D sprite_frame;
    sprite_frame.h = scale;
    sprite_frame.w = scale;

    Entity *mouse = entity_new();
    mouse->position.x = x;
    mouse->position.y = y;
    mouse->frame_size.w = sprite_frame.w / 2;
    mouse->frame_size.h = sprite_frame.h / 2;
    mouse->circ_hitbox.x = sprite_frame.w / 2;
    mouse->circ_hitbox.y = sprite_frame.h / 2;
    mouse->circ_hitbox.r = sprite_frame.w / 4;
    mouse->velocity = 2;
    mouse->angle = angle;
    mouse->flip = flip;
    mouse->state = state;
    mouse->type = MOUSE;
    mouse->shape = CIRCLE;
    mouse->sprite = animals;
    mouse->frame = 8;
    mouse->free = mouse_free;
    mouse->draw = mouse_draw;
    mouse->touch = mouse_touch;
    mouse->update = NULL;
    mouse->think = mouse_think;
}

void mouse_free(Entity *entity)
{
    entity_free(&entity);
}

void mouse_draw(Entity *entity)
{
    entity_draw(entity, entity->position.x, entity->position.y, entity->angle, entity->flip);
}

void mouse_touch(Entity *self, Entity *other)
{
    //When mouse collides with something, it won't do anything to it.
    //However other entities will change the state of the mouse.
    if(entity_intersect(self, other))
    {
        switch(other->type) {
            case WALL:
                break;
            case CAT:
            case MOUSE:
            default:
                break;
        }
    }
}

void mouse_think(Entity *entity)
{
    switch(entity->state)
    {
        case UP:
            entity->position.y -= entity->velocity;
            break;
        case RIGHT:
            entity->position.x += entity->velocity;
            break;
        case DOWN:
            entity->position.y += entity->velocity;
            break;
        case LEFT:
            entity->position.x -= entity->velocity;
            break;
        case FREE:
            entity_free(&entity);
            break;
        case STOP:
        default:
            break;
    }
}