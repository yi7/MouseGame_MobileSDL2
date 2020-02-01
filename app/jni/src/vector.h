#ifndef MOUSEGAME_VECTOR_H
#define MOUSEGAME_VECTOR_H

#include <SDL.h>

typedef struct
{
    float x; /**<the x component of the vector*/
    float y; /**<the y component of the vector*/
} Vector2D;

/**
 * @brief check if two rectangles intersect
 */
bool vector_rect_intersect(SDL_Rect a, SDL_Rect b);

#define ZERO 0
#define MAX(a,b) (a > b ? a:b)
#define MIN(a,b) (a < b ? a:b)

#endif //MOUSEGAME_VECTOR_H
