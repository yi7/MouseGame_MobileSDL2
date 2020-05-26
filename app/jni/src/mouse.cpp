#include "mouse.h"

Uint32 mouse_now;

void mouse_initialize(int x, int y, int frame_size, int angle, Entity_Type type)
{
    Sprite *animals = sprite_load("images/si_mouse.png", 64, 64, 8);
    mouse_now = SDL_GetTicks();
    Entity *mouse = entity_new();
    mouse->active = true;
    mouse->stuck = false;
    mouse->position.x = x + (graphics_reference.wall_padding / 2);
    mouse->position.y = y + (graphics_reference.wall_padding / 2);
    mouse->frame_size.w = frame_size - graphics_reference.wall_padding;
    mouse->frame_size.h = frame_size - graphics_reference.wall_padding;
    mouse->state = STOP;
    mouse->frame = 0;
    mouse->life = 1;
    mouse->angle = angle;
    mouse->type = type;
    mouse->sprite = animals;
    mouse->free = mouse_free;
    mouse->draw = mouse_draw;
    mouse->update = mouse_update;
    mouse->think = mouse_think;

    switch(type)
    {
        case MOUSE:
            mouse->skip_frame = 0;
            mouse->velocity = 420;
            mouse->touch = mouse_touch;
            break;
        case MOUSE_DRILL:
            mouse->skip_frame = 24;
            mouse->velocity = 420;
            mouse->touch = mouse_touch;
            break;
        default:
            mouse->skip_frame = 0;
            mouse->velocity = 0;
            mouse->think = NULL;
            break;
    }
}

void mouse_free(Entity *entity)
{
    entity_free(&entity);
}

void mouse_draw(Entity *entity)
{
    int frame = ((SDL_GetTicks() - mouse_now) * 7 / 1000) % 8;
    entity->frame = frame + entity->skip_frame;
    entity_draw(entity, entity->position.x, entity->position.y, entity->angle - 90);
}

void mouse_touch(Entity *self, Entity *other)
{
    switch(other->type)
    {
        case BOULDER:
            if(self->type == MOUSE_DRILL)
            {
                other->state = FREE;
                break;
            }
        case WALL:
            mouse_step_off(self, other);
            mouse_find_path(self);
            break;
        case TILE_ARROW:
            //SDL_Log("%f", entity_intersect_percentage(self, other));
            if(other->angle != self->angle)
            {
                if(!self->stuck)
                {
                    if(entity_intersect_percentage(self, other) >= 95)
                    {
                        self->stuck = true;
                        self->position.x = other->position.x + (graphics_reference.wall_padding / 2);
                        self->position.y = other->position.y + (graphics_reference.wall_padding / 2);
                        self->angle = other->angle;
                    }
                }
                else if(entity_intersect_percentage(self, other) < 95)
                {
                    self->stuck = false;
                }
            }
            break;
        case TILE_HOME:
            if(entity_intersect_percentage(self, other) > 85)
            {
                other->update(other);
                entity_free(&self);
            }
            break;
        case TILE_HOLE:
            if(entity_intersect_percentage(self, other) > 85)
            {
                entity_update_all_active_state(FREEZE);
                entity_free(&self);
            }
            break;
        default:
            return;
    }
}

void mouse_update(Entity *self)
{
    return;
}

void mouse_think(Entity *self)
{
    entity_touch_all(self);

    switch(self->angle)
    {
        case UP:
            self->position.y -= self->velocity * graphics_delta;
            //SDL_Log("%f", self->velocity * graphics_delta);
            break;
        case RIGHT:
            self->position.x += self->velocity * graphics_delta;
            break;
        case DOWN:
            self->position.y += self->velocity * graphics_delta;
            break;
        case LEFT:
            self->position.x -= self->velocity * graphics_delta;
            break;
        default:
            return;
    }
}

void mouse_step_off(Entity *self, Entity *other)
{
    int wall_padding = 0;
    if(other->type == BOULDER)
    {
        wall_padding = graphics_reference.wall_padding;
    }

    switch(self->angle)
    {
        case UP:
            self->position.y = other->position.y + other->frame_size.h + wall_padding;
            break;
        case RIGHT:
            self->position.x = other->position.x - self->frame_size.w - wall_padding;
            break;
        case DOWN:
            self->position.y = other->position.y - self->frame_size.h - wall_padding;
            break;
        case LEFT:
            self->position.x = other->position.x + other->frame_size.w + wall_padding;
            break;
        default:
            return;
    }
}

void mouse_find_path(Entity *self)
{
    Entity *temp_up_hitbox;
    Entity *temp_right_hitbox;
    Entity *temp_down_hitbox;
    Entity *temp_left_hitbox;

    temp_up_hitbox = entity_new();
    temp_right_hitbox = entity_new();
    temp_down_hitbox = entity_new();
    temp_left_hitbox = entity_new();

    temp_up_hitbox->position.x = self->position.x;
    temp_up_hitbox->position.y = self->position.y - graphics_reference.tile_padding_2;
    temp_up_hitbox->frame_size.w = self->frame_size.w;
    temp_up_hitbox->frame_size.h = self->frame_size.h;

    temp_right_hitbox->position.x = self->position.x + graphics_reference.tile_padding_2;
    temp_right_hitbox->position.y = self->position.y;
    temp_right_hitbox->frame_size.w = self->frame_size.w;
    temp_right_hitbox->frame_size.h = self->frame_size.h;

    temp_down_hitbox->position.x = self->position.x;
    temp_down_hitbox->position.y = self->position.y + graphics_reference.tile_padding_2;
    temp_down_hitbox->frame_size.w = self->frame_size.w;
    temp_down_hitbox->frame_size.h = self->frame_size.h;

    temp_left_hitbox->position.x = self->position.x - graphics_reference.tile_padding_2;
    temp_left_hitbox->position.y = self->position.y;
    temp_left_hitbox->frame_size.w = self->frame_size.w;
    temp_left_hitbox->frame_size.h = self->frame_size.h;

    switch(self->angle)
    {
        case UP:
            if( !(entity_intersect_all_filter_by_type(temp_right_hitbox, WALL) ||
                  entity_intersect_all_filter_by_type(temp_right_hitbox, BOULDER)))
            {
                self->angle = RIGHT;
            }
            else if(!(entity_intersect_all_filter_by_type(temp_left_hitbox, WALL) ||
                      entity_intersect_all_filter_by_type(temp_left_hitbox, BOULDER)))
            {
                self->angle = LEFT;
            }
            else if(!(entity_intersect_all_filter_by_type(temp_down_hitbox, WALL) ||
                      entity_intersect_all_filter_by_type(temp_down_hitbox, BOULDER)))
            {
                self->angle = DOWN;
            }
            break;
        case RIGHT:
            if(!(entity_intersect_all_filter_by_type(temp_down_hitbox, WALL) ||
                 entity_intersect_all_filter_by_type(temp_down_hitbox, BOULDER)))
            {
                self->angle = DOWN;
            }
            else if(!(entity_intersect_all_filter_by_type(temp_up_hitbox, WALL) ||
                      entity_intersect_all_filter_by_type(temp_up_hitbox, BOULDER)))
            {
                self->angle = UP;
            }
            else if(!(entity_intersect_all_filter_by_type(temp_left_hitbox, WALL) ||
                      entity_intersect_all_filter_by_type(temp_left_hitbox, BOULDER)))
            {
                self->angle = LEFT;
            }
            break;
        case DOWN:
            if(!(entity_intersect_all_filter_by_type(temp_left_hitbox, WALL) ||
                 entity_intersect_all_filter_by_type(temp_left_hitbox, BOULDER)))
            {
                self->angle = LEFT;
            }
            else if(!(entity_intersect_all_filter_by_type(temp_right_hitbox, WALL) ||
                      entity_intersect_all_filter_by_type(temp_right_hitbox, BOULDER)))
            {
                self->angle = RIGHT;
            }
            else if(!(entity_intersect_all_filter_by_type(temp_up_hitbox, WALL) ||
                      entity_intersect_all_filter_by_type(temp_up_hitbox, BOULDER)))
            {
                self->angle = UP;
            }
            break;
        case LEFT:
            if( !(entity_intersect_all_filter_by_type(temp_up_hitbox, WALL) ||
                  entity_intersect_all_filter_by_type(temp_up_hitbox, BOULDER)))
            {
                self->angle = UP;
            }
            else if(!(entity_intersect_all_filter_by_type(temp_down_hitbox, WALL) ||
                      entity_intersect_all_filter_by_type(temp_down_hitbox, BOULDER)))
            {
                self->angle = DOWN;
            }
            else if(!(entity_intersect_all_filter_by_type(temp_right_hitbox, WALL) ||
                      entity_intersect_all_filter_by_type(temp_right_hitbox, BOULDER)))
            {
                self->angle = RIGHT;
            }
            break;
        default:
            return;
    }

    entity_free(&temp_up_hitbox);
    entity_free(&temp_right_hitbox);
    entity_free(&temp_down_hitbox);
    entity_free(&temp_left_hitbox);
}

void mouse_find_path_2(Entity *self)
{
    Entity *temp_up_hitbox;
    Entity *temp_right_hitbox;
    Entity *temp_down_hitbox;
    Entity *temp_left_hitbox;

    temp_up_hitbox = entity_new();
    temp_right_hitbox = entity_new();
    temp_down_hitbox = entity_new();
    temp_left_hitbox = entity_new();

    temp_up_hitbox->position.x = self->position.x;
    temp_up_hitbox->position.y = self->position.y - graphics_reference.tile_padding_2;
    temp_up_hitbox->frame_size.w = self->frame_size.w;
    temp_up_hitbox->frame_size.h = self->frame_size.h;

    temp_right_hitbox->position.x = self->position.x + graphics_reference.tile_padding_2;
    temp_right_hitbox->position.y = self->position.y;
    temp_right_hitbox->frame_size.w = self->frame_size.w;
    temp_right_hitbox->frame_size.h = self->frame_size.h;

    temp_down_hitbox->position.x = self->position.x;
    temp_down_hitbox->position.y = self->position.y + graphics_reference.tile_padding_2;
    temp_down_hitbox->frame_size.w = self->frame_size.w;
    temp_down_hitbox->frame_size.h = self->frame_size.h;

    temp_left_hitbox->position.x = self->position.x - graphics_reference.tile_padding_2;
    temp_left_hitbox->position.y = self->position.y;
    temp_left_hitbox->frame_size.w = self->frame_size.w;
    temp_left_hitbox->frame_size.h = self->frame_size.h;

    switch(self->angle)
    {
        case UP:
            if( !entity_intersect_all_filter_by_type(temp_right_hitbox, BOULDER))
            {
                self->angle = RIGHT;
            }
            else if(!entity_intersect_all_filter_by_type(temp_left_hitbox, BOULDER))
            {
                self->angle = LEFT;
            }
            else if(!entity_intersect_all_filter_by_type(temp_down_hitbox, BOULDER))
            {
                self->angle = DOWN;
            }
            break;
        case RIGHT:
            if(!entity_intersect_all_filter_by_type(temp_down_hitbox, BOULDER))
            {
                self->angle = DOWN;
            }
            else if(!entity_intersect_all_filter_by_type(temp_up_hitbox, BOULDER))
            {
                self->angle = UP;
            }
            else if(!entity_intersect_all_filter_by_type(temp_left_hitbox, BOULDER))
            {
                self->angle = LEFT;
            }
            break;
        case DOWN:
            if(!entity_intersect_all_filter_by_type(temp_left_hitbox, BOULDER))
            {
                self->angle = LEFT;
            }
            else if(!entity_intersect_all_filter_by_type(temp_right_hitbox, BOULDER))
            {
                self->angle = RIGHT;
            }
            else if(!entity_intersect_all_filter_by_type(temp_up_hitbox, BOULDER))
            {
                self->angle = UP;
            }
            break;
        case LEFT:
            if( !entity_intersect_all_filter_by_type(temp_up_hitbox, BOULDER))
            {
                self->angle = UP;
            }
            else if(!entity_intersect_all_filter_by_type(temp_down_hitbox, BOULDER))
            {
                self->angle = DOWN;
            }
            else if(!entity_intersect_all_filter_by_type(temp_right_hitbox, BOULDER))
            {
                self->angle = RIGHT;
            }
            break;
        default:
            return;
    }

    entity_free(&temp_up_hitbox);
    entity_free(&temp_right_hitbox);
    entity_free(&temp_down_hitbox);
    entity_free(&temp_left_hitbox);
}