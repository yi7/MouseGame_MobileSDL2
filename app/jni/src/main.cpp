#include <SDL.h>
#include <stdio.h>
#include <string>
#include <string.h>

#include "graphics.h"
#include "sprite.h"
#include "entity.h"
#include "map.h"

void main_initialize_system();
void main_close_system();

void main_initialize_system()
{
    graphics_initialize_system("MouseGame");
    sprite_initialize_system();
    entity_initialize_system();
    map_initialize_system();
    atexit(main_close_system);
}

void main_close_system()
{
    //Quit SDL subsystems
    map_close_system();
    entity_close_system();
    sprite_close_system();
    graphics_close_system();
    SDL_Quit();
}

int SDL_main( int argc, char* args[] )
{
    main_initialize_system();
    SDL_Event e;
    bool quit = false;

    //Sprite *test = sprite_load("images/loaded.png", 640, 480, 1);

    SDL_Log("test");

    map_load_entities("test");

    while(!quit)
    {
        graphics_next_frame();
        //SDL_PumpEvents();

        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        SDL_RenderClear(graphics_renderer);
        //entity_touch_all();
        entity_think_all();


        //sprite_draw(test, 0, 0, 0, 640, 480, 0, SDL_FLIP_NONE);
        map_draw_tiles(0);
        entity_draw_all();

        SDL_RenderPresent(graphics_renderer);
    }

    //Free resources and close SDL
    main_close_system();

    return 0;
}
