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
    int total_radius = a.r + b.r;
    int total_radius_squared = total_radius * total_radius;

    if(vector_distance_squared(a.x, a.y, b.x, b.y) < total_radius_squared)
    {
        return true;
    }

    return false;
}

bool vector_circ_rect_intersect(Circle2D a, SDL_Rect b)
{
    int close_x;
    int close_y;

    if(a.x < b.x)
    {
        close_x = b.x;
    }
    else if(a.x > b.x + b.w)
    {
        close_x = b.x + b.w;
    }
    else
    {
        close_x = a.x;
    }

    if(a.y < b.y)
    {
        close_y = b.y;
    }
    else if(a.y > b.y + b.h)
    {
        close_y = b.y + b.h;
    }
    else
    {
        close_y = a.y;
    }

    if(vector_distance_squared(a.x, a.y, close_x, close_y) < (a.r * a.r))
    {
        return true;
    }

    return false;
}

double vector_distance_squared(int x1, int y1, int x2, int y2)
{
    int delta_x = x2 - x1;
    int delta_y = y2 - y1;
    return (delta_x * delta_x) + (delta_y * delta_y);
}
