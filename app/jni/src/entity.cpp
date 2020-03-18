#include "entity.h"

static Entity *entity_list = NULL;
const int ENTITY_MAX = 1000;

void entity_initialize_system()
{
    entity_list = (Entity *)malloc(sizeof(Entity) * ENTITY_MAX);
    if(!entity_list)
    {
        SDL_Log("entity_initialize_system() failed to allocate entity system -Error:");
        return;
    }

    memset(entity_list, 0, sizeof(Entity) * ENTITY_MAX);
    atexit(entity_close_system);
}

void entity_close_system()
{
    entity_free_all();
    free(&entity_list);
}

Entity *entity_new()
{
    for(int i = 0; i < ENTITY_MAX; i++)
    {
        if(entity_list[i].inuse)
        {
            continue;
        }

        memset(&entity_list[i], 0, sizeof(Entity));
        entity_list[i].inuse = true;
        return &entity_list[i];
    }

    return NULL;
}

void entity_free(Entity **entity)
{
    Entity *self;

    if(!entity)
    {
        return;
    }

    if(!*entity)
    {
        return;
    }

    self = *entity;
    self->inuse = false;
    sprite_free(&self->sprite);
    *entity = NULL;
}

void entity_free_all()
{
    Entity *entity;
    for(int i = 0; i < ENTITY_MAX; i++)
    {
        entity = &entity_list[i];
        if(entity->inuse)
        {
            entity_free(&entity);
        }
    }
}

void entity_draw(Entity *entity, int draw_x, int draw_y, int angle)
{
    if(!entity)
    {
        SDL_Log("entity_draw() no entity to draw -Error:");
    }

    if(!entity->sprite)
    {
        SDL_Log("entity_draw() entity has no sprite -Error:");
    }

    sprite_draw(entity->sprite, entity->frame, draw_x, draw_y, entity->frame_size.w, entity->frame_size.h, angle, SDL_FLIP_NONE);
}

void entity_draw_all()
{
    entity_draw_all_non_active();
    entity_draw_all_active();
}

void entity_draw_all_non_active()
{
    for(int i = 0; i < ENTITY_MAX; i++)
    {
        if(!entity_list[i].inuse)
        {
            continue;
        }

        if(!entity_list[i].draw)
        {
            continue;
        }

        if(entity_list[i].active)
        {
            continue;
        }

        entity_list[i].draw(&entity_list[i]);
    }
}

void entity_draw_all_active()
{
    for(int i = 0; i < ENTITY_MAX; i++)
    {
        if(!entity_list[i].inuse)
        {
            continue;
        }

        if(!entity_list[i].draw)
        {
            continue;
        }

        if(!entity_list[i].active)
        {
            continue;
        }

        entity_list[i].draw(&entity_list[i]);
    }
}

bool entity_intersect(Entity *a, Entity *b)
{
    SDL_Rect a_box;
    SDL_Rect b_box;

    a_box.x = a->position.x;
    a_box.y = a->position.y;
    a_box.w = a->frame_size.w;
    a_box.h = a->frame_size.h;

    b_box.x = b->position.x;
    b_box.y = b->position.y;
    b_box.w = b->frame_size.w;
    b_box.h = b->frame_size.h;

    return vector_rectangle_intersect(a_box, b_box);
}

bool entity_intersect_all_filter_by_type(Entity *self, Entity_Type type)
{
    if(!self)
    {
        return false;
    }

    for(int i = 0; i < ENTITY_MAX; i++)
    {
        if(!entity_list[i].inuse)
        {
            continue;
        }

        if(self == &entity_list[i])
        {
            continue;
        }

        if(entity_list[i].type != type)
        {
            continue;
        }

        if(entity_intersect(self, &entity_list[i]))
        {
            return true;
        }
    }

    return false;
}

void entity_touch_all(Entity *self)
{
    if(!self->touch)
    {
        return;
    }

    for(int i = 0; i < ENTITY_MAX; i++)
    {
        if(!entity_list[i].inuse)
        {
            continue;
        }

        if(self == &entity_list[i])
        {
            continue;
        }

        if(entity_intersect(self, &entity_list[i]))
        {
            self->touch(self, &entity_list[i]);
            //entity_list[i].touch(self, &entity_list[i]);
        }
    }
}

float entity_intersect_percentage(Entity *a, Entity *b)
{
    return 0;
}

void entity_think_all()
{
    for(int i = 0; i < ENTITY_MAX; i++)
    {
        if(!entity_list[i].inuse)
        {
            continue;
        }

        if(entity_list[i].state == STOP)
        {
            continue;
        }

        if(!entity_list[i].think)
        {
            continue;
        }

        entity_list[i].think(&entity_list[i]);
    }
}

void entity_update_all_active_state(Entity_State state)
{
    for(int i = 0; i < ENTITY_MAX; i++)
    {
        if(!entity_list[i].inuse)
        {
            continue;
        }

        if(entity_list[i].active)
        {
            entity_list[i].state = state;
        }
    }
}
