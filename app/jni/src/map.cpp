#include "map.h"

Tile *tile_list = NULL;
const int TILE_MAX = 63;
int arrow_max;
int arrow_count;
int mouse_max;
int mouse_count;
Map_State map_state;
Edit_Type map_edit_type;
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
    mouse_count = 0;
    entity_free_all();
    memset(tile_list, 0, sizeof(Tile) * TILE_MAX);
}

void map_free_entity_tile(Entity *self)
{
    entity_free(&self);
}

void map_initialize_base(int map_id, Map_State state)
{

    Map_Detail *map_detail = NULL;
    map_detail = file_get_map(map_id);

    arrow_max = map_detail->arrow_count;
    arrow_count = 0;
    map_state = state;
    map_edit_type = NONE;
    map_active = map_id;
    mouse_count = 0;

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

    //This loop adds all vertical walls
    for(int i = 0; i < (sizeof(map_detail->map) / sizeof(map_detail->map[0])); i++)
    {
        if(strcmp(map_detail->map[i], "11") == 0) {
            wall_initialize(wall_x, wall_y, 0, tile_frame_size, -90, WALL);
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

    //This loop adds all horizontal walls
    for(int i = 0; i < (sizeof(map_detail->map) / sizeof(map_detail->map[0])); i++)
    {
        if(strcmp(map_detail->map[i], "12") == 0) {
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
    //0-9 Mouse, 10-19 Cat, 20-29 Tiles
    for(int i = 0; i < map_detail->entity_count; i++)
    {
        switch(map_detail->entities[i].type)
        {
            case 0:
                mouse_max++;
                mouse_initialize(map_detail->entities[i].x * tile_frame_size, map_detail->entities[i].y * tile_frame_size, tile_frame_size, map_detail->entities[i].angle, MOUSE);
                break;
            case 10:
                cat_initialize(map_detail->entities[i].x * tile_frame_size, map_detail->entities[i].y * tile_frame_size, tile_frame_size, map_detail->entities[i].angle, CAT);
                break;
            case 20:
                map_initialize_home_tile(map_detail->entities[i].x * tile_frame_size, map_detail->entities[i].y * tile_frame_size, map_detail->entities[i].angle);
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
    if(map_state != PLAN && map_state != EDIT)
    {
        return;
    }

    if(!(touch_x < graphics_reference.map_width && touch_y < graphics_reference.map_height))
    {
        return;
    }

    int map_x = touch_x / graphics_reference.tile_padding;
    int map_y = touch_y / graphics_reference.tile_padding;
    int tile_position = (graphics_reference.tpl * map_y) + map_x;
    int angle;

    if(abs(touch_x - untouch_x) > abs(touch_y - untouch_y))
    {
        if(touch_x < untouch_x)
        {
            angle = RIGHT;
        }
        else
        {
            angle = LEFT;
        }
    }
    else
    {
        if(touch_y < untouch_y)
        {
            angle = DOWN;
        }
        else
        {
            angle = UP;
        }
    }

    if(map_state == PLAN)
    {
        if(!tile_list[tile_position].occupied)
        {
            if(arrow_count < arrow_max)
            {
                arrow_count++;
                tile_list[tile_position].occupied = true;
                map_place_tile(tile_list[tile_position].point.x, tile_list[tile_position].point.y, angle, ARROW_TILE);
            }
        }
        else
        {
            arrow_count--;
            tile_list[tile_position].occupied = false;
            map_remove_tile(touch_x, touch_y);
        }
    }
    else if(map_state == EDIT)
    {
        switch(map_edit_type)
        {
            case ETILE_HOLE:
                map_place_tile(tile_list[tile_position].point.x, tile_list[tile_position].point.y, angle, BLACK_TILE);
                break;
            case ETILE_HOME:
                map_place_tile(tile_list[tile_position].point.x, tile_list[tile_position].point.y, angle, HOME_TILE);
                break;
            case ETILE_REMOVE:
                map_remove_tile(touch_x, touch_y);
            default:
                return;
        }
    }
}

void map_place_tile(int x, int y, int angle, int frame)
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
    tile->frame = frame;
    tile->life = 1;
    tile->state = STOP;
    switch(frame)
    {
        case ARROW_TILE:
            tile->type = TILE_ARROW;
            break;
        case BLACK_TILE:
            tile->type = TILE_HOLE;
            break;
        case HOME_TILE:
            tile->type = TILE_HOME;
            break;
        default:
            return;
    }
    tile->sprite = arrow_tile;
    tile->free = map_free_entity_tile;
    tile->draw = map_draw_entity_tile;
    tile->touch = NULL;
    tile->update = NULL;
    tile->think = NULL;

    if(entity_intersect_all_filter_by_type(tile, TILE_HOME) ||
       entity_intersect_all_filter_by_type(tile, TILE_HOLE))
    {
        entity_free(&tile);
    }
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
    switch(map_state)
    {
        case PLAN:
            if(other->type == TILE_ARROW)
            {
                entity_free(&other);
            }
            break;
        case EDIT:
            switch(map_edit_type)
            {
                case ETILE_REMOVE:
                    if(other->type == TILE_HOLE ||
                       other->type == TILE_HOME)
                    {
                        entity_free(&other);
                    }
                    break;
                default:
                    return;
            }
            break;
        default:
            return;
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
    map_initialize_base(map_active, PLAN);
    map_load_entities(map_active);
}

void map_change_edit_type(Edit_Type type)
{
    map_edit_type = type;
}

void map_initialize_home_tile(int x, int y, int angle)
{
    Entity *home_tile;
    home_tile = entity_new();
    Sprite* temp_tiles = sprite_load("images/tiles.png", 64, 64, 6);

    home_tile->active = false;
    home_tile->stuck = false;
    home_tile->position.x = x;
    home_tile->position.y = y;
    home_tile->frame_size.w = graphics_reference.tile_padding;
    home_tile->frame_size.h = graphics_reference.tile_padding;
    home_tile->velocity = 0;
    home_tile->angle = angle;
    home_tile->frame = 5;
    home_tile->life = 1;
    home_tile->state = STOP;
    home_tile->type = TILE_HOME;
    home_tile->sprite = temp_tiles;

    home_tile->free = map_free_entity_tile;
    home_tile->draw = map_draw_entity_tile;
    home_tile->touch = NULL;
    home_tile->update = map_update_home_tile;
    home_tile->think = NULL;
}

void map_update_home_tile(Entity *self)
{
    if(mouse_count - 1 >= 0)
    {
        mouse_count--;
    }

    if(mouse_count == 0)
    {
        map_state = PAUSE;
        entity_update_all_active_state(STOP);
    }
}