#include "vector.h"

bool vector_rectangle_intersect(SDL_Rect a, SDL_Rect b)
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

double vector_square_intersect_percentage(SDL_Rect a, SDL_Rect b)
{
    int x_overlap = MAX(0, MIN(a.x + a.w, b.x + b.w)) - MAX(a.x, b.x);
    int y_overlap = MAX(0, MIN(a.y + a.h, b.y + b.h)) - MAX(a.y, b.y);
    float overlap_area = x_overlap * y_overlap;
    float overlap_reference = MAX((a.w * a.h), (b.w * b.h));
    float percentage = overlap_area / overlap_reference * 100;

    return percentage;
}
