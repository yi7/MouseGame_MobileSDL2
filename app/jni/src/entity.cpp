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
    Entity *entity;
    for(int i = 0; i < ENTITY_MAX; i++)
    {
        entity = &entity_list[i];
        if(entity->inuse)
        {
            entity_free(&entity);
        }
    }
    free(entity_list);
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

void entity_draw(Entity *entity, int draw_x, int draw_y, int angle, SDL_RendererFlip flip)
{
    if(!entity)
    {
        SDL_Log("entity_draw() no entity to draw -Error:");
    }

    if(!entity->sprite)
    {
        SDL_Log("entity_draw() entity has no sprite -Error:");
    }

    sprite_draw(entity->sprite, entity->frame, draw_x, draw_y, entity->frame_size.w, entity->frame_size.h, angle, flip);
}

void entity_draw_all()
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

        entity_list[i].draw(&entity_list[i]);
    }
}

bool entity_intersect(Entity *a, Entity *b)
{
    return false;
}

Entity *entity_intersect_all(Entity *self)
{
    return NULL;
}