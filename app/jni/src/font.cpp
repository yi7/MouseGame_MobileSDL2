#include "font.h"

TTF_Font *small_font = NULL;
TTF_Font *medium_font = NULL;
TTF_Font *large_font = NULL;


void font_initialize_system()
{
    if(TTF_Init() == 0)
    {
        atexit(TTF_Quit);
    }
    else
    {
        SDL_Log("font_initialize_system() failed to initialize font system -Error:");
    }

    small_font = TTF_OpenFont("fonts/zerotwos.ttf", graphics_screen.h * 0.025f);
    if(!small_font)
    {
        SDL_Log("font_initialize_system() failed to initialize small font -Error:");
    }

    medium_font = TTF_OpenFont("fonts/zerotwos.ttf", graphics_screen.h * 0.04f);
    if(!medium_font)
    {
        SDL_Log("font_initialize_system() failed to initialize medium font -Error:");
    }

    large_font = TTF_OpenFont("fonts/zerotwos.ttf", graphics_screen.h * 0.055f);
    if(!large_font)
    {
        SDL_Log("font_initialize_system() failed to initialize large font -Error:");
    }
}

void font_draw_text(char *text, int x, int y, SDL_Color color, int size)
{
    int center_x, center_y, w, h;
    SDL_Surface *temp_surface = NULL;
    SDL_Texture *temp_texture = NULL;

    switch(size)
    {
        case 0:
            temp_surface = TTF_RenderText_Solid(small_font, text, color);
            break;
        case 1:
            temp_surface = TTF_RenderText_Solid(medium_font, text, color);
            break;
        case 2:
            temp_surface = TTF_RenderText_Solid(large_font, text, color);
            break;
        default:
            return;
    }

    if(!temp_surface)
    {
        SDL_Log("font_draw_text() failed to render text surface -Error:");
    }

    temp_texture = SDL_CreateTextureFromSurface(graphics_renderer, temp_surface);
    if(!temp_texture)
    {
        SDL_Log("font_draw_text() failed to render texture from rendered text -Error:");
    }

    SDL_Rect dest;
    dest.x = x + 50;
    dest.y = y + 50;
    dest.w = temp_surface->w;
    dest.h = temp_surface->h;

    SDL_RenderCopyEx(graphics_renderer, temp_texture, NULL, &dest, 0, NULL, SDL_FLIP_NONE);
}
