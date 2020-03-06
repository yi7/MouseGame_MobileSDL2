#include "vector.h"

bool vector_rects_intersect(SDL_Rect a, SDL_Rect b)
{
    if( (a.x + a.w > b.x) &&
        (b.x + b.w > a.x) &&
        (a.y + a.h > b.y) &&
        (b.y + b.h > a.y) )
    {
        return true;
    }
    return false;
}

bool vector_circs_intersect(Circle2D a, Circle2D b)
{
    return false;
}

bool vector_circ_rect_intersect(Circle2D a, SDL_Rect b)
{
    return false;
}