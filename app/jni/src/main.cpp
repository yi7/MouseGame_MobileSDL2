#include <SDL.h>
#include <stdio.h>
#include <string>
#include <string.h>

#include "graphics.h"
#include "sprite.h"

void main_initialize_system();
void main_close_system();

void main_initialize_system()
{
    graphics_initialize_system("MouseGame");
    sprite_initialize_system();
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
    Sprite *test;

    test = sprite_load("images/hello.bmp", 128, 128);

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

        SDL_RenderClear(graphics_renderer);

        sprite_draw(test, 0, ((graphics_screen.w - 128) / 2), ((graphics_screen.h - 128) / 2));

        SDL_RenderPresent(graphics_renderer);
    }

    //Free resources and close SDL
    main_close_system();

    return 0;
}
