#ifndef MOUSEGAME_ENTITY_H
#define MOUSEGAME_ENTITY_H

#include "graphics.h"
#include "sprite.h"
#include "vector.h"

#define RIGHT 0
#define UP -90
#define LEFT 180
#define DOWN 90

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
    TILE_ARROW,
    TILE_HOME,
    TILE_HOLE
};

typedef struct Entity_S
{
    bool inuse; /**<flag for tracking resource*/
    bool active; /**<flag to determine if entity moves or not*/
    bool stuck; /**<flag to determine if entity is stuck on map*/
    Point2D position; /**<position of entity*/
    Rectangle2D frame_size; /**<frame size of entity*/
    int velocity; /**<velocity of entity*/
    int angle; /**<angle of entity*/
    int frame; /**<frame of entity*/
    int skip_frame;
    int life;
    enum Entity_State state; /**<state of entity*/
    enum Entity_Type type; /**<type of entity*/
    Sprite *sprite; /**<sprite associated with entity*/

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

/**
 * @brief draws the entity
 * @param entity the entity to be drawn
 * @param draw_x the x position of the entity
 * @param draw_y the y position of the entity
 * @param angle the angle of the entity
 */
void entity_draw(Entity *entity, int draw_x, int draw_y, int angle);

/**
 * @brief calls all draw functions of entities
 */
void entity_draw_all();

/**
 * @brief calls all draw function of non active entities
 */
void entity_draw_all_non_active();

/**
 * @brief calls all draw function of active entities
 */
void entity_draw_all_active();

/**
 * @brief checks whether two entities intersect with each other
 * @param a first entity to check
 * @param b second entity to check
 * @return true or false depending on whether there is a collision or not
 */
bool entity_intersect(Entity *a, Entity *b);

/**
 * @brief checks whether entity intersects with a certain type
 * @param self entity to check
 * @param type type to filter by
 * @return true or false depending on whether there is a collision or not
 */
bool entity_intersect_all_filter_by_type(Entity *self, Entity_Type type);

Entity *entity_intersect_all_find_one(Entity *self);

/**
 * @brief runs touch function against all entities collided with
 * @param self entity to check collision
 */
void entity_touch_all(Entity *self);

/**
 * @brief determines percentage of entity collision
 * @param a first entity to check
 * @param b second entity to check
 * @return collision percentage
 */
float entity_intersect_percentage(Entity *a, Entity *b);

/**
 * @brief calls all think functions of entities on the entity system
 */
void entity_think_all();

/**
 * @brief updates all active entities state
 * @param state the state to change to
 */
void entity_update_all_active_state(Entity_State state);

#endif //MOUSEGAME_ENTITY_H
