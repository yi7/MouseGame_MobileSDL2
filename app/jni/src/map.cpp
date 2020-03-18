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

void map_free_entity_tile(Entity *self)
{
    entity_free(&self);
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
    float tile_frame_size = graphics_reference.tile_padding;

    int wall_x = 0;
    int wall_y = 0;
    int next_row_count = 0;

    //This loop adds all horizontal walls
    for(int i = 0; i < (sizeof(map_detail->map) / sizeof(map_detail->map[0])); i++)
    {
        if(strcmp(map_detail->map[i], "11") == 0) {
            wall_initialize(wall_x, wall_y, 0, tile_frame_size, 90, WALL);

            wall_x += tile_frame_size;
            next_row_count++;
        }
        else if(strcmp(map_detail->map[i], "14") == 0) {
            wall_x += tile_frame_size;
            next_row_count++;
        }

        if(next_row_count >= 10)
        {
            wall_x = 0;
            wall_y += tile_frame_size;
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
            wall_initialize(wall_x, wall_y, 0, tile_frame_size, 0, WALL);
            wall_x += tile_frame_size;
            next_row_count++;
        }
        else if(strcmp(map_detail->map[i], "15") == 0) {
            wall_x += tile_frame_size;
            next_row_count++;
        }

        if(next_row_count >= 9)
        {
            wall_x = 0;
            wall_y += tile_frame_size;
            next_row_count = 0;
        }

    }

    //This loop adds all animal entities
    //0-9 Mouse, 10-19 Cat
    for(int i = 0; i < map_detail->entity_count; i++)
    {
        switch(map_detail->entities[i].type)
        {
            case 0:
                mouse_initialize(map_detail->entities[i].x * tile_frame_size, map_detail->entities[i].y * tile_frame_size, tile_frame_size, map_detail->entities[i].angle, MOUSE);
                break;
            default:
                break;
        }
    }
}

void map_draw_base_tile()
{
    for(int i = 0; i < TILE_MAX; i++)
    {
        sprite_draw(tiles, tile_list[i].frame, tile_list[i].point.x, tile_list[i].point.y, tile_list[i].frame_size.w, tile_list[i].frame_size.h, 0, SDL_FLIP_NONE);
    }
}

void map_draw_entity_tile(Entity *self)
{
    entity_draw(self, self->position.x, self->position.y, self->angle);
}

void map_update(float touch_x, float touch_y, float untouch_x, float untouch_y)
{
    if(map_state != PLAN)
    {
        return;
    }

    if(touch_x < graphics_reference.map_width && touch_y < graphics_reference.map_height)
    {
        int map_x = touch_x / graphics_reference.tile_padding;
        int map_y = touch_y / graphics_reference.tile_padding;
        int tile_position = (graphics_reference.tpl * map_y) + map_x;

        if(!tile_list[tile_position].occupied)
        {
            if(arrow_count < arrow_max)
            {
                arrow_count++;
                tile_list[tile_position].occupied = true;
                if(abs(touch_x - untouch_x) > abs(touch_y - untouch_y))
                {
                    if(touch_x < untouch_x)
                    {
                        map_place_tile(tile_list[tile_position].point.x, tile_list[tile_position].point.y, 0);
                    }
                    else
                    {
                        map_place_tile(tile_list[tile_position].point.x, tile_list[tile_position].point.y, 180);
                    }
                }
                else
                {
                    if(touch_y < untouch_y)
                    {
                        map_place_tile(tile_list[tile_position].point.x, tile_list[tile_position].point.y, 90);
                    }
                    else
                    {
                        map_place_tile(tile_list[tile_position].point.x, tile_list[tile_position].point.y, -90);
                    }
                }
            }
        }
        else
        {
            arrow_count--;
            tile_list[tile_position].occupied = false;
            map_remove_tile(touch_x, touch_y);
        }
    }
}

void map_place_tile(int x, int y, int angle)
{
    Entity *tile;
    tile = entity_new();
    Sprite* arrow_tile = sprite_load("images/tiles.png", 64, 64, 6);

    tile->active = false;
    tile->stuck = false;
    tile->position.x = x;
    tile->position.y = y;
    tile->frame_size.w = graphics_reference.tile_padding;
    tile->frame_size.h = graphics_reference.tile_padding;
    tile->velocity = 0;
    tile->angle = angle;
    tile->frame = 7;
    tile->state = STOP;
    tile->type = TILE_ARROW;
    tile->sprite = arrow_tile;

    tile->free = map_free_entity_tile;
    tile->draw = map_draw_entity_tile;
    tile->touch = NULL;
    tile->update = NULL;
    tile->think = NULL;
}

void map_remove_tile(float x, float y)
{
    //Create a temporary entity that stores touch location
    Entity *touch;
    touch = entity_new();

    memset(touch, 0, sizeof(Entity));
    touch->position.x = x;
    touch->position.y = y;
    touch->frame_size.w = 0;
    touch->frame_size.h = 0;
    touch->touch = map_touch_tile;

    //Call touch function of entity touched
    entity_touch_all(touch);

    //Free temp entity, don't need it anymore
    entity_free(&touch);
}

void map_touch_tile(Entity *self, Entity *other)
{
    if(other->type == TILE_ARROW)
    {
        entity_free(&other);
    }
}

void map_play()
{
    map_state = PLAY;
    entity_update_all_active_state(MOVE);
}

void map_stop()
{
    map_state = PAUSE;
    entity_update_all_active_state(STOP);
}

void map_reset()
{
    map_free_all();
    map_state = PLAN;
    map_initialize_base(map_active);
    map_load_entities(map_active);
}