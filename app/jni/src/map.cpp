#include "map.h"

Sprite *tiles;
Sprite *buttons;
float TILE_FRAME = 0;
int TPL = 0;
float WALL_FRAME_WIDTH;
const int MAP_TILE_COLUMNS = 9;
const int MAP_TILE_ROWS = 7;
Tile *tile_list = NULL;
int ARROW_LIMIT = 0;
int arrow_count = 0;
Map_State map_state;

void map_initialize_system()
{
    tiles = sprite_load("images/tiles.png", 64, 64, 6);
    buttons = sprite_load("images/map_buttons.png", 64, 64, 3);
    TILE_FRAME = graphics_screen.h / MAP_TILE_ROWS;
    TPL = MAP_TILE_COLUMNS;

    tile_list = (Tile *)malloc(sizeof(Tile) * (MAP_TILE_COLUMNS * MAP_TILE_ROWS));
    if(!tile_list)
    {
        SDL_Log("map_initialize_system() failed to allocate entity system -Error:");
        return;
    }
    memset(tile_list, 0, sizeof(Tile) * (MAP_TILE_COLUMNS * MAP_TILE_ROWS));

    //Didn't know where I can put the wall frame to adjust hitbox for animal entities so I placed it here.
    //4 is the pixel width of wall. 64 is the pixel height of the wall.
    WALL_FRAME_WIDTH = TILE_FRAME * 4 / 64;

    atexit(map_close_system);
}

void map_close_system()
{
    entity_free_all();
    free(tile_list);
    sprite_free(&tiles);
    sprite_free(&buttons);
}

void map_free_all()
{
    entity_free_all();
    memset(tile_list, 0, sizeof(Tile) * (MAP_TILE_COLUMNS * MAP_TILE_ROWS));
}

void map_initialize_base(int map_id)
{
    Map_Detail *level = NULL;
    level = map_parser_get_map(map_id);

    ARROW_LIMIT = level->arrow_count;
    arrow_count = 0;
    map_state = PLAN;

    int tile_x = 0;
    int tile_y = 0;
    int next_row_count = 0;
    int frame = 2;
    int tile_index = 0;

    for(int i = 0; i < (sizeof(level->map) / sizeof(level->map[0])); i++)
    {
        if(strcmp(level->map[i], "00") == 0) {
            tile_new(&tile_list[tile_index], tile_x, tile_y, TILE_FRAME, frame, false);
            tile_index++;
            if(frame == 2)
            {
                frame = 3;
            }
            else
            {
                frame = 2;
            }
            tile_x += TILE_FRAME;
            next_row_count++;
        }

        if(next_row_count >= 9)
        {
            tile_x = 0;
            tile_y += TILE_FRAME;
            next_row_count = 0;
        }
    }
}

void map_load_entities(int map_id)
{
    Map_Detail *level = NULL;
    level = map_parser_get_map(map_id);

    int wall_x = 0;
    int wall_y = 0;
    int next_row_count = 0;

    //This loop adds all vertical walls
    for(int i = 0; i < (sizeof(level->map) / sizeof(level->map[0])); i++)
    {
        if(strcmp(level->map[i], "11") == 0) {
            wall_initialize(wall_x, wall_y, TILE_FRAME, 0, SDL_FLIP_NONE);

            wall_x += TILE_FRAME;
            next_row_count++;
        }
        else if(strcmp(level->map[i], "14") == 0) {
            wall_x += TILE_FRAME;
            next_row_count++;
        }

        if(next_row_count >= 10)
        {
            wall_x = 0;
            wall_y += TILE_FRAME;
            next_row_count = 0;
        }
    }

    wall_x = 0;
    wall_y = 0;
    next_row_count = 0;

    //This loop adds all horizontal walls
    for(int i = 0; i < (sizeof(level->map) / sizeof(level->map[0])); i++)
    {
        if(strcmp(level->map[i], "12") == 0) {
            //SDL_Log("%d, %d", wall_x, wall_y);
            wall_initialize(wall_x, wall_y, TILE_FRAME, 90, SDL_FLIP_NONE);
            wall_x += TILE_FRAME;
            next_row_count++;
        }
        else if(strcmp(level->map[i], "15") == 0) {
            wall_x += TILE_FRAME;
            next_row_count++;
        }

        if(next_row_count >= 9)
        {
            wall_x = 0;
            wall_y += TILE_FRAME;
            next_row_count = 0;
        }

    }

    //This loop adds all animal entities
    //0-9 Mouse, 10-19 Cat
    for(int i = 0; i < level->entity_count; i++)
    {
        switch(level->entities[i].type)
        {
            case 0:
                mouse_initialize(TILE_FRAME * level->entities[i].x, TILE_FRAME * level->entities[i].y, TILE_FRAME, level->entities[i].angle, SDL_FLIP_NONE);
                break;
            default:
                break;
        }
    }
}

void map_draw_tiles()
{
    for(int i = 0; i < (MAP_TILE_COLUMNS * MAP_TILE_ROWS); i++)
    {
        sprite_draw(tiles, tile_list[i].frame, tile_list[i].point.x, tile_list[i].point.y, TILE_FRAME, TILE_FRAME, 0, SDL_FLIP_NONE);
    }
}

bool map_check_on_tile(Entity *entity)
{
    for(int i = 0; i < (MAP_TILE_COLUMNS * MAP_TILE_ROWS); i++)
    {
        if(entity->position.x == tile_list[i].point.x &&
           entity->position.y == tile_list[i].point.y)
        {
            return true;
        }
    }

    return false;
}

void map_play()
{
    map_state = PLAY;
    entity_change_active_entity_state(MOVE);
}

void map_stop()
{
    map_state = PAUSE;
    entity_change_active_entity_state(STOP);
}

void map_reset(int button_id)
{
    map_free_all();
    map_initialize_base(button_id);
    map_load_entities(button_id);
}

void map_update(float touch_x, float touch_y, float untouch_x, float untouch_y)
{
    if(map_state == PLAY || map_state == PAUSE)
    {
        return;
    }

    int map_x = touch_x / TILE_FRAME;
    int map_y = touch_x / TILE_FRAME;
    int tile_position = (TPL * map_y) + map_x;

    /*SDL_Log("touch: %d, %d", touch_x, touch_y);
    SDL_Log("untouch: %d, %d", untouch_x, untouch_y);
    SDL_Log("pos: %d", tile_position);*/

    if(!tile_list[tile_position].occupied)
    {
        if(arrow_count < ARROW_LIMIT)
        {
            arrow_count++;
            tile_list[tile_position].occupied = true;
            if(abs(touch_x - untouch_x) > abs(touch_y - untouch_y))
            {
                if(touch_x < untouch_x)
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
                if(touch_y < untouch_y)
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
        arrow_count--;
        tile_list[tile_position].occupied = false;
        entity_free_specific(tile_list[tile_position].point.x, tile_list[tile_position].point.y, TILE);
    }
}