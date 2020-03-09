#include "mouse.h"

void mouse_initialize(int x, int y, int scale, enum State state, int angle, SDL_RendererFlip flip)
{
    Sprite *animals = sprite_load("images/animals_test.png", 64, 64, 8);

    Rectangle2D sprite_frame;
    sprite_frame.h = scale;
    sprite_frame.w = scale;

    Entity *mouse = entity_new();
    mouse->position.x = x;
    mouse->position.y = y;
    mouse->frame_size.w = sprite_frame.w / 2;
    mouse->frame_size.h = sprite_frame.h / 2;
    mouse->rect_hitbox.w = sprite_frame.w / 2;
    mouse->rect_hitbox.h = sprite_frame.h / 2;
    mouse->velocity = 4;
    mouse->angle = angle;
    mouse->flip = flip;
    mouse->state = state;
    mouse->type = MOUSE;
    mouse->shape = RECTANGLE;
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
                //mouse_update_wall(self);
                break;
            case CAT:
            case MOUSE:
            default:
                break;
        }
    }
}

void mouse_upadte_wall(Entity *entity)
{

}

void mouse_think(Entity *self)
{
    mouse_step_forward(self);

    switch(mouse_check_front_type(self)) {
        case WALL:
            mouse_step_backward(self);
            switch (self->state) {
                case UP:
                    self->state = RIGHT;
                    mouse_step_forward(self);
                    if(mouse_check_front_type(self) == WALL)
                    {
                        mouse_step_backward(self);
                        self->state = LEFT;
                        mouse_step_forward(self);
                        if(mouse_check_front_type(self) == WALL)
                        {
                            mouse_step_backward(self);
                            self->state = DOWN;
                            mouse_step_forward(self);
                        }
                    }
                    break;
                case RIGHT:
                    self->state = DOWN;
                    mouse_step_forward(self);
                    if(mouse_check_front_type(self) == WALL)
                    {
                        mouse_step_backward(self);
                        self->state = UP;
                        mouse_step_forward(self);
                        if(mouse_check_front_type(self) == WALL)
                        {
                            mouse_step_backward(self);
                            self->state = LEFT;
                            mouse_step_forward(self);
                        }
                    }
                    break;
                case DOWN:
                    self->state = LEFT;
                    mouse_step_forward(self);
                    if(mouse_check_front_type(self) == WALL)
                    {
                        mouse_step_backward(self);
                        self->state = RIGHT;
                        mouse_step_forward(self);
                        if(mouse_check_front_type(self) == WALL)
                        {
                            mouse_step_backward(self);
                            self->state = UP;
                            mouse_step_forward(self);
                        }
                    }
                    break;
                case LEFT:
                    self->state = UP;
                    mouse_step_forward(self);
                    if(mouse_check_front_type(self) == WALL)
                    {
                        mouse_step_backward(self);
                        self->state = DOWN;
                        mouse_step_forward(self);
                        if(mouse_check_front_type(self) == WALL)
                        {
                            mouse_step_backward(self);
                            self->state = RIGHT;
                            mouse_step_forward(self);
                        }
                    }
                    break;
                default:
                    break;
            }
            break;
        case CAT:
            entity_free(&self);
            break;
        default:
            break;
    }

    //SDL_Log("pos: %d, %d", entity->position.x, entity->position.y);
    //SDL_Log("fra: %d, %d", entity->rect_hitbox.w, entity->rect_hitbox.h);
}

void mouse_step_forward(Entity *self)
{
    switch(self->state)
    {
        case UP:
            self->position.y -= self->velocity;
            break;
        case RIGHT:
            self->position.x += self->velocity;
            break;
        case DOWN:
            self->position.y += self->velocity;
            break;
        case LEFT:
            self->position.x -= self->velocity;
            break;
        default:
            break;
    }
}

void mouse_step_backward(Entity *self)
{
    switch(self->state)
    {
        case UP:
            self->position.y += self->velocity;
            break;
        case RIGHT:
            self->position.x -= self->velocity;
            break;
        case DOWN:
            self->position.y -= self->velocity;
            break;
        case LEFT:
            self->position.x += self->velocity;
            break;
        default:
            break;
    }
}

Type mouse_check_front_type(Entity *self)
{
    Entity *entity;

    entity = entity_intersect_all(self);
    if(entity)
    {
        return entity->type;
    }

    return TILE;
}