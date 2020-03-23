#include "cat.h"

void cat_initialize(int x, int y, int frame_size, int angle, Entity_Type type)
{
    Sprite *animals = sprite_load("images/animals_test.png", 64, 64, 8);

    Entity *cat = entity_new();
    cat->active = true;
    cat->stuck = false;
    cat->position.x = x + (graphics_reference.wall_padding / 2);
    cat->position.y = y + (graphics_reference.wall_padding / 2);
    cat->frame_size.w = frame_size - graphics_reference.wall_padding;
    cat->frame_size.h = frame_size - graphics_reference.wall_padding;
    cat->velocity = 16;
    cat->angle = angle;
    cat->frame = 0;
    cat->life = 1;
    cat->state = STOP;
    cat->type = type;
    cat->sprite = animals;

    cat->free = cat_free;
    cat->draw = cat_draw;
    cat->touch = cat_touch;
    cat->update = cat_update;
    cat->think = cat_think;
}

void cat_free(Entity *entity)
{
    entity_free(&entity);
}

void cat_draw(Entity *entity)
{
    entity_draw(entity, entity->position.x, entity->position.y, entity->angle);
}

void cat_touch(Entity *self, Entity *other)
{
    switch(other->type)
    {
        case WALL:
            cat_step_off(self, other);
            cat_find_path(self);
            break;
        case TILE_ARROW:
            if(entity_intersect_percentage(self, other) > 95)
            {
                self->position.x = other->position.x + (graphics_reference.wall_padding / 2);
                self->position.y = other->position.y + (graphics_reference.wall_padding / 2);
                self->angle = other->angle;

                if(other->life > 0)
                {
                    other->frame = 8;
                    other->life--;
                }
                else
                {
                    entity_free(&other);
                }
            }

            break;
        case MOUSE:
            if(entity_intersect_percentage(self, other) > 50)
            {
                entity_free(&other);
                entity_update_all_active_state(STOP);
            }
            break;
        case TILE_HOME:
            if(entity_intersect_percentage(self, other) > 85)
            {
                entity_update_all_active_state(STOP);
            }
            break;
        case TILE_HOLE:
            if(entity_intersect_percentage(self, other) > 85)
            {
                entity_free(&self);
            }
            break;
        default:
            return;
    }
}

void cat_update(Entity *self)
{
    return;
}

void cat_think(Entity *self)
{
    entity_touch_all(self);

    switch(self->angle)
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
            return;
    }
}

void cat_step_off(Entity *self, Entity *other)
{
    switch(self->angle)
    {
        case UP:
            self->position.y = other->position.y + other->frame_size.h;
            break;
        case RIGHT:
            self->position.x = other->position.x - self->frame_size.w;
            break;
        case DOWN:
            self->position.y = other->position.y - self->frame_size.h;
            break;
        case LEFT:
            self->position.x = other->position.x + other->frame_size.w;
            break;
        default:
            return;
    }
}

void cat_find_path(Entity *self)
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
    temp_up_hitbox->position.y = self->position.y - graphics_reference.wall_padding;
    temp_up_hitbox->frame_size.w = self->frame_size.w;
    temp_up_hitbox->frame_size.h = self->frame_size.h;

    temp_right_hitbox->position.x = self->position.x + graphics_reference.wall_padding;
    temp_right_hitbox->position.y = self->position.y;
    temp_right_hitbox->frame_size.w = self->frame_size.w;
    temp_right_hitbox->frame_size.h = self->frame_size.h;

    temp_down_hitbox->position.x = self->position.x;
    temp_down_hitbox->position.y = self->position.y + graphics_reference.wall_padding;
    temp_down_hitbox->frame_size.w = self->frame_size.w;
    temp_down_hitbox->frame_size.h = self->frame_size.h;

    temp_left_hitbox->position.x = self->position.x - graphics_reference.wall_padding;
    temp_left_hitbox->position.y = self->position.y;
    temp_left_hitbox->frame_size.w = self->frame_size.w;
    temp_left_hitbox->frame_size.h = self->frame_size.h;

    switch(self->angle)
    {
        case UP:
            if(!entity_intersect_all_filter_by_type(temp_right_hitbox, WALL))
            {
                self->angle = RIGHT;
            }
            else if(!entity_intersect_all_filter_by_type(temp_left_hitbox, WALL))
            {
                self->angle = LEFT;
            }
            else if(!entity_intersect_all_filter_by_type(temp_down_hitbox, WALL))
            {
                self->angle = DOWN;
            }
            break;
        case RIGHT:
            if(!entity_intersect_all_filter_by_type(temp_down_hitbox, WALL))
            {
                self->angle = DOWN;
            }
            else if(!entity_intersect_all_filter_by_type(temp_up_hitbox, WALL))
            {
                self->angle = UP;
            }
            else if(!entity_intersect_all_filter_by_type(temp_left_hitbox, WALL))
            {
                self->angle = LEFT;
            }
            break;
        case DOWN:
            if(!entity_intersect_all_filter_by_type(temp_left_hitbox, WALL))
            {
                self->angle = LEFT;
            }
            else if(!entity_intersect_all_filter_by_type(temp_right_hitbox, WALL))
            {
                self->angle = RIGHT;
            }
            else if(!entity_intersect_all_filter_by_type(temp_up_hitbox, WALL))
            {
                self->angle = UP;
            }
            break;
        case LEFT:
            if(!entity_intersect_all_filter_by_type(temp_up_hitbox, WALL))
            {
                self->angle = UP;
            }
            else if(!entity_intersect_all_filter_by_type(temp_down_hitbox, WALL))
            {
                self->angle = DOWN;
            }
            else if(!entity_intersect_all_filter_by_type(temp_right_hitbox, WALL))
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

