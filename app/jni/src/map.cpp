#include "map.h"

Sprite *tiles;
int map_width;
int map_height;
const int MAP_TILE_COLUMNS = 9;
const int MAP_TILE_ROWS = 7;

char map[221][3] = {"00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00",
                    "15", "13", "12", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15",
                    "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "11", "00",
                    "15", "13", "15", "13", "12", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15",
                    "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "11", "00", "14", "00",
                    "15", "13", "15", "13", "15", "13", "12", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15",
                    "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "11", "00", "14", "00", "14", "00",
                    "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15",
                    "00", "14", "00", "14", "00", "11", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00",
                    "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "12", "13", "15", "13", "15",
                    "00", "14", "00", "11", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00",
                    "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "12", "13", "15",
                    "00", "11", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00"};

void map_initialize_system()
{
    tiles = sprite_load("images/tiles.png", 64, 64, 6);
    int map_tile_dimension = graphics_screen.h / MAP_TILE_ROWS;
    map_height = map_tile_dimension * MAP_TILE_ROWS;
    map_width = map_tile_dimension * MAP_TILE_COLUMNS;
}

void map_load_entities(char *filename)
{
    int tile_frame = graphics_screen.h / MAP_TILE_ROWS;
    int wall_x = tile_frame;
    int wall_y = 0;
    int next_row_count = 0;

    for(int i = 0; i < (sizeof(map) / sizeof(map[0])); i++)
    {
        if(strcmp(map[i], "11") == 0) {
            wall_initialize(wall_x, wall_y, tile_frame, 0, SDL_FLIP_NONE);

            wall_x += tile_frame;
            next_row_count++;
        }
        else if(strcmp(map[i], "14") == 0) {
            wall_x += tile_frame;
            next_row_count++;
        }

        if(next_row_count >= 8)
        {
            wall_x = tile_frame;
            wall_y += tile_frame;
            next_row_count = 0;
        }
    }

    wall_x = 0;
    wall_y = tile_frame;
    next_row_count = 0;

    for(int i = 0; i < (sizeof(map) / sizeof(map[0])); i++)
    {
        if(strcmp(map[i], "12") == 0) {
            wall_initialize(wall_x + (tile_frame / 2) , wall_y - (tile_frame / 2), tile_frame, 90, SDL_FLIP_NONE);

            wall_x += tile_frame;
            next_row_count++;
        }
        else if(strcmp(map[i], "15") == 0) {
            wall_x += tile_frame;
            next_row_count++;
        }

        if(next_row_count >= 9)
        {
            wall_x = 0;
            wall_y += tile_frame;
            next_row_count = 0;
        }
    }

    mouse_initialize(tile_frame * 2 + (tile_frame / 4), tile_frame / 4, tile_frame, RIGHT, 0, SDL_FLIP_NONE);
}

void map_draw_tiles(int map_id)
{
    int tile_length = graphics_screen.h / MAP_TILE_ROWS;

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
}