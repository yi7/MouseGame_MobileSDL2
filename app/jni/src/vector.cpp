#include "vector.h"

bool vector_rect_intersect(SDL_Rect a, SDL_Rect b)
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