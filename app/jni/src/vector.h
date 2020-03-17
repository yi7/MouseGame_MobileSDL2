#ifndef MOUSEGAME_VECTOR_H
#define MOUSEGAME_VECTOR_H

#include <SDL.h>

typedef struct
{
    float x; /**<the x position of point*/
    float y; /**<the y position of point*/
} Point2D;

typedef struct
{
    float w; /**<the width of rectangle*/
    float h; /**<the height of rectangle*/
} Rectangle2D;

/**
 * @brief check if two rectangles intersect
 * @param a first rectangle to compare
 * @param b second rectangle to compare
 */
bool vector_rectangle_intersect(SDL_Rect a, SDL_Rect b);

/**
 * @brief checks percentage of intersection based on second rectangle
 * @param a first rectangle to compare
 * @param b second rectangle to compare
 * @return percentage of intersection
 */
double vector_square_intersect_percentage(SDL_Rect a, SDL_Rect b);

#define ZERO 0
#define MAX(a,b) (a > b ? a:b)
#define MIN(a,b) (a < b ? a:b)

#endif //MOUSEGAME_VECTOR_H
