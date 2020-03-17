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

typedef struct
{
    int x; /**<the x position of circle*/
    int y; /**<the y position of circle*/
    int r; /**<the radius of circle*/
} Circle2D;

/**
 * @brief check if two rectangles intersect
 * @param a first rectangle to compare
 * @param b second rectangle to compare
 */
bool vector_rects_intersect(SDL_Rect a, SDL_Rect b);

/**
 * @brief check if two circles intersect
 * @param a first circle to compare
 * @param b second circle to compare
 */
bool vector_circs_intersect(Circle2D a, Circle2D b);

/**
 * @brief check if circle and rectangle intersect
 * @param a circle to compare
 * @param b rectangle to compare
 */
bool vector_circ_rect_intersect(Circle2D a, SDL_Rect b);

/**
 * @brief distance squared
 * @param x1 first x point
 * @param y1 first y point
 * @param x2 second x point
 * @param y2 second y point
 * @return distance squared
 */
double vector_distance_squared(int x1, int y1, int x2, int y2);

double vector_square_intersect_percentage(SDL_Rect a, SDL_Rect b);

#define ZERO 0
#define MAX(a,b) (a > b ? a:b)
#define MIN(a,b) (a < b ? a:b)

#endif //MOUSEGAME_VECTOR_H
