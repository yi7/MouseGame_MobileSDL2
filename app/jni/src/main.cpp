#include <SDL.h>
#include <stdio.h>
#include <string>
#include <string.h>

#include "graphics.h"

void main_initialize_system();
void main_close_system();

void main_initialize_system()
{
    graphics_initialize_system("MouseGame");
    atexit(main_close_system);
}

void main_close_system()
{
    //Quit SDL subsystems
    SDL_Quit();
}

int SDL_main( int argc, char* args[] )
{
    main_initialize_system();
    SDL_Event e;
    bool quit = false;

    void *main_pixels = NULL;
    int main_pitch = 0;
    int main_width, main_height;
    SDL_Texture *main_texture = NULL;
    SDL_Surface *main_surface = SDL_LoadBMP("images/hello.bmp");
    SDL_Surface *main_formatted_surface = SDL_ConvertSurfaceFormat(main_surface, SDL_PIXELFORMAT_RGBA8888, 0);
    main_texture = SDL_CreateTexture(graphics_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, main_formatted_surface->w, main_formatted_surface->h);
    SDL_SetTextureBlendMode(main_texture, SDL_BLENDMODE_BLEND);
    SDL_LockTexture(main_texture, &main_formatted_surface->clip_rect, &main_pixels, &main_pitch);
    memcpy(main_pixels, main_formatted_surface->pixels, main_formatted_surface->pitch * main_formatted_surface->h);
    main_width = main_formatted_surface->w;
    main_height = main_formatted_surface->h;

    Uint32 *main_temp_pixels = (Uint32*)main_pixels;
    int main_pixel_count = (main_pitch / 4) * main_height;
    Uint32 main_color_key = SDL_MapRGB(main_formatted_surface->format, 0, 0xFF, 0xFF);
    Uint32 main_transparent = SDL_MapRGBA(main_formatted_surface->format, 0x00, 0xFF, 0xFF, 0x00);

    for(int i = 0; i < main_pixel_count; ++i)
    {
        if(main_temp_pixels[i] == main_color_key)
        {
            main_temp_pixels[i] = main_transparent;
        }
    }

    SDL_UnlockTexture(main_texture);
    main_pixels = NULL;
    SDL_FreeSurface(main_formatted_surface);
    SDL_FreeSurface(main_surface);

    while(!quit)
    {
        //graphics_next_frame();
        //SDL_PumpEvents();

        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(graphics_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(graphics_renderer);

        SDL_Rect main_render_quad = {((graphics_screen.w - main_width) / 2), ((graphics_screen.h - main_height) / 2), main_width, main_height};
        SDL_RenderCopyEx(graphics_renderer, main_texture, NULL, &main_render_quad, 0.0, NULL, SDL_FLIP_NONE);

        SDL_RenderPresent(graphics_renderer);
    }

    //Free resources and close SDL
    main_close_system();

    return 0;
}
