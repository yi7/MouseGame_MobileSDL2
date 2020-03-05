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
    sprite_close_system();
    graphics_close_system();
    SDL_Quit();
}

int SDL_main( int argc, char* args[] )
{
    main_initialize_system();
    SDL_Event e;
    bool quit = false;

    Sprite *test;
    test = sprite_load("images/hello.bmp", 128, 128, 1);

    SDL_Log("test");

    Sprite *tiles, *walls, *wall_joint;
    tiles = sprite_load("images/tiles.bmp", 64, 64, 6);
    walls = sprite_load("images/walls.bmp", 16, 64, 4);
    wall_joint = sprite_load("images/wall_joint.bmp", 4, 4, 1);

    char map[221][3] = {"00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00",
                        "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11",
                        "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00",
                        "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11",
                        "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00",
                        "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11",
                        "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00",
                        "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11",
                        "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00",
                        "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11",
                        "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00",
                        "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11", "12", "11",
                        "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00"};
    int tile_dim = graphics_screen.h / 7;

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

        int x = 0;
        int y = 0;
        int j = 2;
        for(int i = 0; i < (sizeof(map) / sizeof(map[0])); i++)
        {
            if(strcmp(map[i], "11") == 0 || strcmp(map[i], "12") == 0)
            {
                continue;
            }
            if(strcmp(map[i], "00") == 0) {
                sprite_draw(tiles, j, x, y, tile_dim, tile_dim, 0, SDL_FLIP_NONE);
                if(j == 0)
                {
                    j = 2;
                }
                else
                {
                    j = 0;
                }
                x += tile_dim;
            }

            if(x >= tile_dim * 9)
            {
                x = 0;
                y += tile_dim;
            }
        }

        SDL_RenderPresent(graphics_renderer);
    }

    //Free resources and close SDL
    main_close_system();

    return 0;
}
