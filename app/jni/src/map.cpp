#include "map.h"

Tile *tile_list = NULL;
const int TILE_MAX = 63;
int arrow_max;
int arrow_count;
Map_State map_state;
int map_active;
Sprite *tiles;

void map_initialize_system()
{
    map_state = INACTIVE;
    tile_list = (Tile *)malloc(sizeof(Tile) * TILE_MAX);
    if(!tile_list)
    {
        SDL_Log("map_initialize_system() failed to allocate entity system -Error:");
        return;
    }
    memset(tile_list, 0, sizeof(Tile) * TILE_MAX);

    tiles = sprite_load("images/tiles.png", 64, 64, 6);

    atexit(map_close_system);
}

void map_close_system()
{
    entity_free_all();
    free(tile_list);
}

void map_free_all()
{
    map_state = INACTIVE;
    arrow_count = 0;
    entity_free_all();
    memset(tile_list, 0, sizeof(Tile) * TILE_MAX);
}

void map_initialize_base(int map_id)
{

    Map_Detail *map_detail = NULL;
    map_detail = file_get_map(map_id);

    arrow_max = map_detail->arrow_count;
    arrow_count = 0;
    map_state = PLAN;
    map_active = map_id;

    int tile_x = 0;
    int tile_y = 0;
    int next_row_count = 0;
    int frame = 2;
    int tile_index = 0;

    for(int i = 0; i < (sizeof(map_detail->map) / sizeof(map_detail->map[0])); i++)
    {
        if(strcmp(map_detail->map[i], "00") == 0) {
            tile_list[tile_index].frame = frame;
            tile_list[tile_index].point.x = tile_x;
            tile_list[tile_index].point.y = tile_y;
            tile_list[tile_index].frame_size.w = graphics_reference.tile_padding;
            tile_list[tile_index].frame_size.h = graphics_reference.tile_padding;
            tile_list[tile_index].occupied = false;
            tile_index++;

            if(frame == 2)
            {
                frame = 3;
            }
            else
            {
                frame = 2;
            }
            tile_x += graphics_reference.tile_padding;
            next_row_count++;
        }

        if(next_row_count >= 9)
        {
            tile_x = 0;
            tile_y += graphics_reference.tile_padding;
            next_row_count = 0;
        }
    }
}

void map_load_entities(int map_id)
{
    Map_Detail *map_detail = NULL;
    map_detail = file_get_map(map_id);

    int wall_x = 0;
    int wall_y = 0;
    int next_row_count = 0;

    //This loop adds all horizontal walls
    for(int i = 0; i < (sizeof(map_detail->map) / sizeof(map_detail->map[0])); i++)
    {
        if(strcmp(map_detail->map[i], "11") == 0) {
            wall_initialize(wall_x, wall_y, 0, graphics_reference.tile_padding, 90, WALL);

            wall_x += graphics_reference.tile_padding;
            next_row_count++;
        }
        else if(strcmp(map_detail->map[i], "14") == 0) {
            wall_x += graphics_reference.tile_padding;
            next_row_count++;
        }

        if(next_row_count >= 10)
        {
            wall_x = 0;
            wall_y += graphics_reference.tile_padding;
            next_row_count = 0;
        }
    }

    wall_x = 0;
    wall_y = 0;
    next_row_count = 0;

    //This loop adds all vertical walls
    for(int i = 0; i < (sizeof(map_detail->map) / sizeof(map_detail->map[0])); i++)
    {
        if(strcmp(map_detail->map[i], "12") == 0) {
            //SDL_Log("%d, %d", wall_x, wall_y);
            wall_initialize(wall_x, wall_y, 0, graphics_reference.tile_padding, 0, WALL);
            wall_x += graphics_reference.tile_padding;
            next_row_count++;
        }
        else if(strcmp(map_detail->map[i], "15") == 0) {
            wall_x += graphics_reference.tile_padding;
            next_row_count++;
        }

        if(next_row_count >= 9)
        {
            wall_x = 0;
            wall_y += graphics_reference.tile_padding;
            next_row_count = 0;
        }

    }

    //This loop adds all animal entities
    //0-9 Mouse, 10-19 Cat
    /*for(int i = 0; i < map_detail->entity_count; i++)
    {
        switch(map_detail->entities[i].type)
        {
            case 0:
                mouse_initialize(TILE_FRAME * map_detail->entities[i].x, TILE_FRAME * map_detail->entities[i].y, TILE_FRAME, map_detail->entities[i].angle, SDL_FLIP_NONE);
                break;
            default:
                break;
        }
    }*/
}

void map_draw_tiles()
{
    for(int i = 0; i < TILE_MAX; i++)
    {
        sprite_draw(tiles, tile_list[i].frame, tile_list[i].point.x, tile_list[i].point.y, tile_list[i].frame_size.w, tile_list[i].frame_size.h, 0, SDL_FLIP_NONE);
    }
}

