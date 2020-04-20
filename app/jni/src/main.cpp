#include <SDL.h>
#include <stdio.h>
#include <string>
#include <string.h>

#include "graphics.h"
#include "font.h"
#include "sprite.h"
#include "menu.h"
#include "file.h"
#include "entity.h"
#include "map.h"
#include "editor.h"

void main_initialize_system();
void main_close_system();

void main_initialize_system()
{
    //Initialize SDL subsystems
    graphics_initialize_system("Game");
    font_initialize_system();
    sprite_initialize_system();
    file_initialize_system();
    menu_initialize_system();
    entity_initialize_system();
    map_initialize_system();
    atexit(main_close_system);
}

void main_close_system()
{
    //Close SDL subsystems
    map_close_system();
    entity_close_system();
    menu_close_system();
    file_close_system();
    sprite_close_system();
    font_close_system();
    graphics_close_system();
    SDL_Quit();
}

int SDL_main( int argc, char* args[] )
{
    main_initialize_system();
    SDL_Event e;
    bool quit = false;
    Point2D touch_location;
    Point2D untouch_location;

    SDL_Log("Start Game!");

    menu_initialize_base_window();
    editor_initialize_button_to_base();

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
                    touch_location.x = e.tfinger.x * graphics_reference.screen_width;
                    touch_location.y = e.tfinger.y * graphics_reference.screen_height;
                    break;
                case SDL_FINGERUP:
                    untouch_location.x = e.tfinger.x * graphics_reference.screen_width;
                    untouch_location.y = e.tfinger.y * graphics_reference.screen_height;
                    //SDL_Log("%d", map_get_state());
                    map_update(touch_location.x, touch_location.y, untouch_location.x, untouch_location.y);
                    menu_update_top_window(untouch_location.x, untouch_location.y);
                    break;
                default:
                    break;
            }
        }

        SDL_RenderClear(graphics_renderer);

        menu_draw_all_window();
        map_draw_base_tile();
        entity_draw_all();
        entity_think_all();
        //menu_think();

        SDL_RenderPresent(graphics_renderer);
    }

    //Free resources and close SDL
    main_close_system();

    return 0;
}
