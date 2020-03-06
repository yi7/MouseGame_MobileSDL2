#include "map.h"

Sprite *tiles, *walls, *wall_joint;

void map_initialize_system()
{
    tiles = sprite_load("images/tiles.bmp", 64, 64, 6);
    walls = sprite_load("images/walls.bmp", 4, 64, 4);
    wall_joint = sprite_load("images/wall_joint.bmp", 4, 4, 1);
}

void map_load(char *filename)
{

}

void map_draw(int map_id)
{
    char map[221][3] = {"00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00",
                        "13", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12",
                        "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00",
                        "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12",
                        "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00",
                        "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12",
                        "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00",
                        "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12",
                        "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00",
                        "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12",
                        "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00",
                        "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12",
                        "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00", "11", "00"};

    int tile_length = graphics_screen.h / 7;

    //Draw Tiles
    int tile_x = 0;
    int tile_y = 0;
    int next_row_count = 0;
    int temp_color = 2;

    for(int i = 0; i < (sizeof(map) / sizeof(map[0])); i++)
    {
        if(strcmp(map[i], "00") == 0) {
            sprite_draw(tiles, temp_color, tile_x, tile_y, tile_length, tile_length, 0, SDL_FLIP_NONE);
            if(temp_color == 0)
            {
                temp_color = 2;
            }
            else
            {
                temp_color = 0;
            }
            tile_x += tile_length;
            next_row_count++;
        }

        if(next_row_count >= 9)
        {
            tile_x = 0;
            tile_y += tile_length;
            next_row_count = 0;
        }
    }

    //Draw Vertical Walls
    tile_x = tile_length;
    tile_y = 0;
    next_row_count = 0;

    for(int i = 0; i < (sizeof(map) / sizeof(map[0])); i++)
    {
        if(strcmp(map[i], "11") == 0) {
            sprite_draw(walls, 0, tile_x - (tile_length / 32), tile_y, tile_length / 16, tile_length, 0, SDL_FLIP_NONE);
            tile_x += tile_length;
            next_row_count++;
        }
        else if(strcmp(map[i], "14") == 0) {
            tile_x += tile_length;
            next_row_count++;
        }

        if(next_row_count >= 8)
        {
            tile_x = tile_length;
            tile_y += tile_length;
            next_row_count = 0;
        }
    }

    //Draw Horizontal Walls
    tile_x = 0;
    tile_y = tile_length;
    next_row_count = 0;

    for(int i = 0; i < (sizeof(map) / sizeof(map[0])); i++)
    {
        if(strcmp(map[i], "12") == 0) {
            sprite_draw(walls, 0, tile_x - - (tile_length / 2) - (tile_length / 32), tile_y - (tile_length / 2), tile_length / 16, tile_length, 90, SDL_FLIP_HORIZONTAL);
            tile_x += tile_length;
            next_row_count++;
        }
        else if(strcmp(map[i], "14") == 0) {
            tile_x += tile_length;
            next_row_count++;
        }

        if(next_row_count >= 9)
        {
            tile_x = 0;
            tile_y += tile_length;
            next_row_count = 0;
        }
    }
}