#ifndef MOUSEGAME_ENTITY_H
#define MOUSEGAME_ENTITY_H

#include <stdlib.h>

#include "graphics.h"
#include "sprite.h"
#include "vector.h"

enum State
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    FREE,
    STOP
};

enum Type
{
    MOUSE,
    CAT,
    WALL,
    TILE,
    STUB
};

enum Shape
{
    CIRCLE,
    RECTANGLE
};

typedef struct Entity_S
{
    bool inuse; /**<flag for tracking resource*/
    Point2D position; /**<position of entity*/
    Rectangle2D frame_size; /**<frame of entity*/
    SDL_Rect rect_hitbox; /**<rectangle hitbox of entity*/
    Circle2D circ_hitbox; /**<circle hitbox of entity*/
    int velocity; /**<velocity of the entity*/
    int angle; /**<angle of the entity*/
    SDL_RendererFlip flip; /**<SDL_RendererFlip value*/
    enum State state; /**<state of the entity*/
    enum Type type; /**<type of entity*/
    enum Shape shape; /**<shape of entity hitbox*/
    Sprite *sprite; /**<sprite associated with entity*/
    int frame; /**<current frame of entity*/

    void (*free)(struct Entity_S *self); /**<cleanup function of entity*/
    void (*draw)(struct Entity_S *self); /**<draw function of entity*/
    void (*touch)(struct Entity_S *self, struct Entity_S *other); /**<touch function of entity*/
    void (*update)(struct Entity_S *self); /**<update function of entity*/
    void (*think)(struct Entity_S *self); /**<think function of entity*/
} Entity;

/**
 * @brief initializes the entity system
 */
void entity_initialize_system();

/**
 * @brief closes the entity system
 */
void entity_close_system();

/**
 * @brief returns a pointer to an empty entity structure
 * @return NULL on error or no more space for entities or a valid entity pointer otherwise
 */
Entity *entity_new();

/**
 * @brief frees an entity
 * @param entity the entity to be freed
 */
void entity_free(Entity **entity);

/**
 * @brief frees all entity on the entity system
 */
void entity_free_all();


void entity_free_specific(int x, int y, Type type);

/**
 * @brief draws the entity
 * @param entity the entity to be drawn
 * @param draw_x the x position of the entity
 * @param draw_y the y position of the entity
 * @param angle the angle of the entity
 * @param flip SDL_RendererFlip value
 */
void entity_draw(Entity *entity, int draw_x, int draw_y, int angle, SDL_RendererFlip flip);

/**
 * @brief calls all draw functions of entities on the entity system
 */
void entity_draw_all();

/**
 * @brief checks whether two entities intersect with each other
 * @param a first entity to check
 * @param b second entity to check
 * @return true or false depending on whether there is a collision or not
 */
bool entity_intersect(Entity *a, Entity *b);

/**
 * @brief checks whether the passed entity intersects with any entity on the entity system
 * @param self the entity to check
 * @return the entity that it collided with
 */
Entity *entity_intersect_all(Entity *self);

/**
 * @brief checks whether there is an entity in front
 * @param self the entity to check
 * @return the entity that is in front, NULL if nothing
 */
Entity *entity_check_front(Entity *self);

/**
 * @brief calls all touch function of entities
 */
void entity_touch_all();

/**
 * @brief updates the entity
 * @param self the entity to update
 */
void entity_update(Entity *self);

/**
 * @brief calls all update functions of entities on the entity system
 */
void entity_update_all();

/**
 * @brief think function that runs every frame
 * @param self the thinking entity
 */
void entity_think(Entity *self);

/**
 * @brief calls all think functions of entities on the entity system
 */
void entity_think_all();

#endif //MOUSEGAME_ENTITY_H
