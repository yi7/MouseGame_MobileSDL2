#include <SDL.h>
#include <stdio.h>
#include <string>
#include <string.h>

#include "graphics.h"
#include "sprite.h"
#include "entity.h"
#include "font.h"
#include "menu.h"
#include "map.h"
#include "map_parser.h"

void main_initialize_system();
void main_close_system();

enum Game_State
{
    MAIN_MENU,
    PLAY,
    PAUSE,
    PLAN,
    RESET,
    QUIT
};

void main_initialize_system()
{
    graphics_initialize_system("MouseGame");
    sprite_initialize_system();
    entity_initialize_system();
    font_initialize_system();
    menu_initialize_system();
    map_parser_initialize_system();
    map_initialize_system();
    atexit(main_close_system);
}

void main_close_system()
{
    //Quit SDL subsystems
    map_close_system();
    map_parser_close_system();
    font_close_system();
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
    Point2D untouch_location;
    int arrow_count = 0;
    //Sprite *test = sprite_load("images/loaded.png", 640, 480, 1);

    SDL_Log("start");

    //map_load_entities("test");

    //map_parser_parse_file("files/maps.txt");
    //map_initialize_window(0);
    //map_initialize_base(0);
    //map_load_entities(0);

    menu_initialize_base_window();
    SDL_Color textColor = { 0, 0, 0 };

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

                    /*switch(state)
                    {
                        case MAIN_MENU:
                            state = PAUSE;
                            break;
                        case PLAY:
                            if(touch_location.x > TILE_FRAME * MAP_TILE_COLUMNS)
                            {
                                state = PAUSE;
                            }
                            else
                            {
                                state = PLAN;
                            }

                            break;
                        case PLAN:
                        case PAUSE:
                            if(touch_location.x > TILE_FRAME * MAP_TILE_COLUMNS)
                            {
                                state = PLAY;
                            }
                            break;
                    }*/
                    break;
                case SDL_FINGERUP:
                    untouch_location.x = e.tfinger.x * graphics_screen.w;
                    untouch_location.y = e.tfinger.y * graphics_screen.h;

                    menu_update_top_window(untouch_location.x, untouch_location.y);

                    /*if(state == PLAN)
                    {
                        int map_x = touch_location.x / TILE_FRAME;
                        int map_y = touch_location.y / TILE_FRAME;
                        int tile_position = (TPL * map_y) + map_x;

                        if(!tile_list[tile_position].occupied)
                        {
                            if(arrow_count < ARROW_LIMIT)
                            {
                                arrow_count++;
                                tile_list[tile_position].occupied = true;
                                if(abs(touch_location.x - untouch_location.x) > abs(touch_location.y - untouch_location.y))
                                {
                                    if(touch_location.x < untouch_location.x)
                                    {
                                        tile_new_entity(tile_list[tile_position].point.x, tile_list[tile_position].point.y, TILE_FRAME, RIGHT, 0, SDL_FLIP_NONE);
                                    }
                                    else
                                    {
                                        tile_new_entity(tile_list[tile_position].point.x, tile_list[tile_position].point.y, TILE_FRAME, LEFT, 0, SDL_FLIP_NONE);
                                    }
                                }
                                else
                                {
                                    if(touch_location.y < untouch_location.y)
                                    {
                                        tile_new_entity(tile_list[tile_position].point.x, tile_list[tile_position].point.y, TILE_FRAME, DOWN, 0, SDL_FLIP_NONE);
                                    }
                                    else
                                    {
                                        tile_new_entity(tile_list[tile_position].point.x, tile_list[tile_position].point.y, TILE_FRAME, UP, 0, SDL_FLIP_NONE);
                                    }
                                }
                            }
                        }
                        else
                        {
                            arrow_count--;
                            tile_list[tile_position].occupied = false;
                            entity_free_specific(tile_list[tile_position].point.x, tile_list[tile_position].point.y, TILE);
                        }
                    }*/

                    break;
                default:
                    break;
            }
        }

        SDL_RenderClear(graphics_renderer);

        menu_draw_all_window();
        map_draw_tiles();
        entity_draw_all();
        entity_think_all();
        //font_draw_text("TEST TEST ONE TWO THREE", 0, 0, textColor, 2);

        /*switch(state)
        {
            case MAIN_MENU:
                break;
            case PLAY:
                entity_think_all();
                entity_update_all();
                menu_draw_all_window();
                map_draw_tiles(0);
                entity_draw_all();
                break;
            case PLAN:
            case PAUSE:
                entity_update_all();
                menu_draw_all_window();
                map_draw_tiles(0);
                entity_draw_all();
                break;
            default:
                break;
        }*/

        SDL_RenderPresent(graphics_renderer);
    }

    //Free resources and close SDL
    main_close_system();

    return 0;
}
