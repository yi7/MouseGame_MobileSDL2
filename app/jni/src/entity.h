#ifndef MOUSEGAME_ENTITY_H
#define MOUSEGAME_ENTITY_H

#include "graphics.h"
#include "sprite.h"
#include "vector.h"

enum Entity_State
{
    MOVE,
    STOP,
    FREE
};

enum Entity_Type
{
    MOUSE,
    CAT,
    WALL,
    TILE
};

typedef struct Entity_S
{
    bool inuse;
    bool active;
    Point2D position;
    Rectangle2D frame_size;
    int velocity;
    int angle;
    int frame;
    enum Entity_State state;
    enum Entity_Type type;
    Sprite *sprite;


    void (*free)(struct Entity_S *self);
    void (*draw)(struct Entity_S *self);
    void (*touch)(struct Entity_S *self, struct Entity_S *other);
    void (*update)(struct Entity_S *self);
    void (*think)(struct Entity_S *self);
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

/**
 * @brief draws the entity
 * @param entity the entity to be drawn
 * @param draw_x the x position of the entity
 * @param draw_y the y position of the entity
 * @param angle the angle of the entity
 * @param flip SDL_RendererFlip value
 */
void entity_draw(Entity *entity, int draw_x, int draw_y, int angle);

/**
 * @brief calls all draw functions of entities on the entity system
 */
void entity_draw_all();

void entity_draw_all_non_active();

void entity_draw_all_active();

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

float entity_intersect_percentage(Entity *a, Entity *b);

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
