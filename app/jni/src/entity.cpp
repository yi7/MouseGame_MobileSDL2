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
    entity_free_all();
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

void entity_free_specific(int x, int y, Type type)
{
    Entity *entity;
    for(int i = 0; i < ENTITY_MAX; i++)
    {
        entity = &entity_list[i];
        if(entity->inuse &&
           entity->position.x == x &&
           entity->position.y == y &&
           entity->type == type)
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
    /*SDL_Rect a_box;
    SDL_Rect b_box;

    if(a->shape == RECTANGLE)
    {
        a_box.x = a->position.x;
        a_box.y = a->position.y;
        a_box.w = a->rect_hitbox.w;
        a_box.h = a->rect_hitbox.h;
    }

    if(b->shape == RECTANGLE)
    {
        b_box.x = b->position.x;
        b_box.y = b->position.y;
        b_box.w = b->rect_hitbox.w;
        b_box.h = b->rect_hitbox.h;
    }*/

    if(a->shape == RECTANGLE && b->shape == RECTANGLE)
    {
        return vector_rects_intersect(a->rect_hitbox, b->rect_hitbox);
    }
    else if(a->shape == CIRCLE && b->shape == RECTANGLE)
    {
        return vector_circ_rect_intersect(a->circ_hitbox, b->rect_hitbox);
    }
    else if(a->shape == RECTANGLE && b->shape == CIRCLE)
    {
        return vector_circ_rect_intersect(b->circ_hitbox, a->rect_hitbox);
    }
    else if(a->shape == CIRCLE && b->shape == CIRCLE)
    {
        return vector_circs_intersect(a->circ_hitbox, b->circ_hitbox);
    }

    return false;
}

Entity *entity_intersect_all(Entity *self)
{
    if(!self)
    {
        return NULL;
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
            return &entity_list[i];
        }
    }

    return NULL;
}

Entity *entity_check_front(Entity *self)
{
    //Move a step ahead
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
        case FREE:
        case STOP:
        default:
            break;
    }

    Entity *entity_collided;
    entity_collided = entity_intersect_all(self);

    if(entity_collided)
    {
        //Move a step behind
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
            case FREE:
            case STOP:
            default:
                break;
        }
        return entity_collided;
    }
    else
    {
        return NULL;
    }
}

void entity_touch_all()
{
    Entity *other;

    for(int i = 0; i < ENTITY_MAX; i++)
    {
        if(!entity_list[i].inuse)
        {
            continue;
        }

        if(!entity_list[i].touch)
        {
            continue;
        }

        other = entity_intersect_all(&entity_list[i]);
        if(other)
        {
            entity_list[i].touch(&entity_list[i], other);
        }
    }
}

void entity_update(Entity *self);

void entity_update_all()
{
    for(int i = 0; i < ENTITY_MAX; i++)
    {
        if(!entity_list[i].inuse)
        {
            continue;
        }

        if(!entity_list[i].update)
        {
            continue;
        }

        entity_list[i].update(&entity_list[i]);
    }
}

void entity_think(Entity *self)
{
    SDL_Log("think thik");
}

void entity_think_all()
{
    for(int i = 0; i < ENTITY_MAX; i++)
    {
        if(!entity_list[i].inuse)
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