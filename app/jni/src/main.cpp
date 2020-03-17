#include <SDL.h>
#include <stdio.h>
#include <string>
#include <string.h>

#include "graphics.h"
#include "font.h"
#include "sprite.h"
#include "menu.h"
//#include "entity.h"
//#include "menu.h"
//#include "map.h"
//#include "map_parser.h"

void main_initialize_system();
void main_close_system();

void main_initialize_system()
{
    graphics_initialize_system("Game");
    font_initialize_system();
    sprite_initialize_system();
    menu_initialize_system();
    //entity_initialize_system();
    //map_parser_initialize_system();
    //map_initialize_system();
    atexit(main_close_system);
}

void main_close_system()
{
    //Quit SDL subsystems
    //map_close_system();
    //map_parser_close_system();
    //entity_close_system();
    menu_close_system();
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


                    /*if(map_check_plan_state() && touch_location.x < TILE_FRAME * MAP_TILE_COLUMNS && touch_location.y < TILE_FRAME * MAP_TILE_ROWS)
                    {
                        int map_x = touch_location.x / TILE_FRAME;
                        int map_y = touch_location.y / TILE_FRAME;
                        int tile_position = (TPL * map_y) + map_x;

                        if(!tile_list[tile_position].occupied)
                        {
                            if(map_get_arrow_count() < ARROW_LIMIT)
                            {
                                map_increment_arrow_count();
                                tile_list[tile_position].occupied = true;
                                if(abs(touch_location.x - untouch_location.x) > abs(touch_location.y - untouch_location.y))
                                {
                                    if(touch_location.x < untouch_location.x)
                                    {
                                        tile_new_entity(tile_list[tile_position].point.x, tile_list[tile_position].point.y, TILE_FRAME, 0, SDL_FLIP_NONE);
                                    }
                                    else
                                    {
                                        tile_new_entity(tile_list[tile_position].point.x, tile_list[tile_position].point.y, TILE_FRAME, 180, SDL_FLIP_NONE);
                                    }
                                }
                                else
                                {
                                    if(touch_location.y < untouch_location.y)
                                    {
                                        tile_new_entity(tile_list[tile_position].point.x, tile_list[tile_position].point.y, TILE_FRAME, 90, SDL_FLIP_NONE);
                                    }
                                    else
                                    {
                                        tile_new_entity(tile_list[tile_position].point.x, tile_list[tile_position].point.y, TILE_FRAME, -90, SDL_FLIP_NONE);
                                    }
                                }
                            }
                        }
                        else
                        {
                            map_decrement_arrow_count();
                            tile_list[tile_position].occupied = false;
                            entity_free_specific(tile_list[tile_position].point.x, tile_list[tile_position].point.y, TILE);
                        }
                    }*/
                    menu_update_top_window(untouch_location.x, untouch_location.y);
                    break;
                default:
                    break;
            }
        }

        SDL_RenderClear(graphics_renderer);

        menu_draw_all_window();
        /*map_draw_tiles();
        entity_draw_all();
        entity_think_all();*/

        SDL_RenderPresent(graphics_renderer);
    }

    //Free resources and close SDL
    main_close_system();

    return 0;
}
