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

enum Game_State
{
    MAIN_MENU,
    PLAY,
    PAUSE,
    QUIT
};

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
    Game_State state = MAIN_MENU;
    Point2D touch_location;
    //Sprite *test = sprite_load("images/loaded.png", 640, 480, 1);

    SDL_Log("test");

    map_load_entities("test");
    touch_location.x = 0;
    touch_location.y = 0;

    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            switch(e.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_FINGERDOWN:
                    touch_location.x = e.tfinger.x * graphics_screen.w;
                    touch_location.y = e.tfinger.y * graphics_screen.h;

                    switch(state)
                    {
                        case MAIN_MENU:
                            state = PLAY;
                            break;
                        case PLAY:
                            if(touch_location.x > TILE_FRAME * MAP_TILE_COLUMNS)
                            {
                                state = PAUSE;
                            }
                            break;
                        case PAUSE:
                            if(touch_location.x > TILE_FRAME * MAP_TILE_COLUMNS)
                            {
                                state = PLAY;
                            }
                            break;
                    }
                    break;
                case SDL_FINGERMOTION:
                    touch_location.x = e.tfinger.x * graphics_screen.w;
                    touch_location.y = e.tfinger.y * graphics_screen.h;
                case SDL_FINGERUP:
                    touch_location.x = e.tfinger.x * graphics_screen.w;
                    touch_location.y = e.tfinger.y * graphics_screen.h;
                default:
                    break;
            }
        }

        SDL_RenderClear(graphics_renderer);

        switch(state)
        {
            case MAIN_MENU:
                break;
            case PLAY:
                entity_think_all();
                map_draw_tiles(0);
                entity_draw_all();
                break;
            case PAUSE:
                map_draw_tiles(0);
                entity_draw_all();
                break;
        }

        SDL_RenderPresent(graphics_renderer);
    }

    //Free resources and close SDL
    main_close_system();

    return 0;
}
