#include "menu.h"

Window *window_stack[WINDOW_MAX];
Window window_list[WINDOW_MAX];
int window_count = 0;
int window_tag = 0;

#define WIN_TOP window_stack[window_count - 1]

void menu_initialize_system()
{
    for(int i = 0; i < WINDOW_MAX; i++)
    {
        window_stack[i] = NULL;
        if(window_list[i].background != NULL)
        {
            sprite_free(&window_list[i].background);
            memset(&window_stack[i], 0, sizeof(Window));
            window_list[i].handle = -1;
        }
    }

    window_count = 0;
}

void menu_initialize_base_window()
{
    Window *main_menu_window = NULL;
    main_menu_window = menu_push_window();

    Sprite *main_menu_background = sprite_load("images/main_menu_background.png", 768, 441, 1);
    Sprite *main_menu_buttons = sprite_load("images/menu_buttons.png", 128, 64, 3);

    main_menu_window->window_frame.x = 0;
    main_menu_window->window_frame.y = 0;
    main_menu_window->window_frame.w = graphics_screen.w;
    main_menu_window->window_frame.h = graphics_screen.h;
    main_menu_window->background = main_menu_background;
    main_menu_window->draw = menu_draw_window;
    main_menu_window->update = menu_update_base_window;

    float button_width = main_menu_window->window_frame.w / 8;
    float button_height = main_menu_window->window_frame.h / 8;
    float button_x = main_menu_window->window_frame.x + (button_width / 2);
    float button_y = main_menu_window->window_frame.y + (button_height / 1.5);

    //SDL_Log("%d, %d", column_div, row_div);

    menu_set_button(main_menu_window, 0, 0, "PLAY", main_menu_buttons, button_x, button_y, button_width, button_height);
    menu_set_button(main_menu_window, 1, 2, "HELP", main_menu_buttons, button_x, button_y * 3, button_width, button_height);

    //menu_set_button(map_window, 0, 0, "test", buttons, map_window->window_frame.x + TILE_FRAME, map_window->window_frame.y + TILE_FRAME, TILE_FRAME / 1.6, TILE_FRAME / 1.6);
}

void menu_update_base_window(Window *self, int button_id)
{
    switch(button_id)
    {
        case 0:
            menu_initialize_pack_window();
            break;
        case 1:
            break;
        default:
            break;
    }
}

void menu_initialize_pack_window()
{
    Window *pack_menu_window = NULL;
    pack_menu_window = menu_push_window();

    Sprite *pack_menu_background = sprite_load("images/main_menu_background.png", 768, 441, 1);
    Sprite *pack_menu_buttons = sprite_load("images/menu_buttons.png", 128, 64, 3);

    pack_menu_window->window_frame.x = 0;
    pack_menu_window->window_frame.y = 0;
    pack_menu_window->window_frame.w = graphics_screen.w;
    pack_menu_window->window_frame.h = graphics_screen.h;
    pack_menu_window->background = pack_menu_background;
    pack_menu_window->draw = menu_draw_window;
    pack_menu_window->update = menu_update_pack_window;

    float button_width = pack_menu_window->window_frame.w / 8;
    float button_height = pack_menu_window->window_frame.h / 8;
    float button_x = pack_menu_window->window_frame.x + (button_width / 2);
    float button_y = pack_menu_window->window_frame.y + (button_height / 1.5);

    //SDL_Log("%d, %d", column_div, row_div);

    menu_set_button(pack_menu_window, 0, 1, "PACK 1", pack_menu_buttons, button_x, button_y, button_width, button_height);

    menu_set_button(pack_menu_window, 1, 2, "BACK", pack_menu_buttons, button_x, button_y * 9, button_width, button_height);
}

void menu_update_pack_window(Window *self, int button_id)
{
    switch(button_id)
    {
        case 0:
            menu_initialize_map_list_window("files/maps.txt");
            break;
        case 1:
            menu_pop_window(self->handle);
            break;
        default:
            break;
    }
}

void menu_initialize_map_list_window(char *filename)
{
    Window *map_menu_window = NULL;
    map_menu_window = menu_push_window();

    Sprite *map_menu_background = sprite_load("images/main_menu_background.png", 768, 441, 1);
    Sprite *map_menu_buttons = sprite_load("images/menu_buttons.png", 128, 64, 3);

    map_parser_parse_file(filename);

    map_menu_window->window_frame.x = 0;
    map_menu_window->window_frame.y = 0;
    map_menu_window->window_frame.w = graphics_screen.w;
    map_menu_window->window_frame.h = graphics_screen.h;
    map_menu_window->background = map_menu_background;
    map_menu_window->draw = menu_draw_window;
    map_menu_window->update = menu_update_map_list_window;

    float button_width = map_menu_window->window_frame.w / 8;
    float button_height = map_menu_window->window_frame.h / 8;
    float button_x = map_menu_window->window_frame.x + (button_width / 2);
    float button_y = map_menu_window->window_frame.y + (button_height / 1.5);

    //SDL_Log("%d, %d", column_div, row_div);

    menu_set_button(map_menu_window, 0, 1, "MAP 1", map_menu_buttons, button_x, button_y, button_width, button_height);
    menu_set_button(map_menu_window, 1, 1, "MAP 2", map_menu_buttons, button_x * 4, button_y, button_width, button_height);
    menu_set_button(map_menu_window, 2, 1, "MAP 3", map_menu_buttons, button_x * 7, button_y, button_width, button_height);
    menu_set_button(map_menu_window, 3, 1, "MAP 4", map_menu_buttons, button_x * 10, button_y, button_width, button_height);
    menu_set_button(map_menu_window, 4, 1, "MAP 5", map_menu_buttons, button_x * 13, button_y, button_width, button_height);

    menu_set_button(map_menu_window, 5, 1, "MAP 6", map_menu_buttons, button_x, button_y * 3, button_width, button_height);
    menu_set_button(map_menu_window, 6, 1, "MAP 7", map_menu_buttons, button_x * 4, button_y * 3, button_width, button_height);
    menu_set_button(map_menu_window, 7, 1, "MAP 8", map_menu_buttons, button_x * 7, button_y * 3, button_width, button_height);
    menu_set_button(map_menu_window, 8, 1, "MAP 9", map_menu_buttons, button_x * 10, button_y * 3, button_width, button_height);
    menu_set_button(map_menu_window, 9, 1, "MAP 10", map_menu_buttons, button_x * 13, button_y * 3, button_width, button_height);

    menu_set_button(map_menu_window, 10, 1, "MAP 11", map_menu_buttons, button_x, button_y * 5, button_width, button_height);
    menu_set_button(map_menu_window, 11, 1, "MAP 12", map_menu_buttons, button_x * 4, button_y * 5, button_width, button_height);
    menu_set_button(map_menu_window, 12, 1, "MAP 13", map_menu_buttons, button_x * 7, button_y * 5, button_width, button_height);
    menu_set_button(map_menu_window, 13, 1, "MAP 14", map_menu_buttons, button_x * 10, button_y * 5, button_width, button_height);
    menu_set_button(map_menu_window, 14, 1, "MAP 15", map_menu_buttons, button_x * 13, button_y * 5, button_width, button_height);

    menu_set_button(map_menu_window, 15, 1, "MAP 16", map_menu_buttons, button_x, button_y * 7, button_width, button_height);
    menu_set_button(map_menu_window, 16, 1, "MAP 17", map_menu_buttons, button_x * 4, button_y * 7, button_width, button_height);
    menu_set_button(map_menu_window, 17, 1, "MAP 18", map_menu_buttons, button_x * 7, button_y * 7, button_width, button_height);
    menu_set_button(map_menu_window, 18, 1, "MAP 19", map_menu_buttons, button_x * 10, button_y * 7, button_width, button_height);
    menu_set_button(map_menu_window, 19, 1, "MAP 20", map_menu_buttons, button_x * 13, button_y * 7, button_width, button_height);

    menu_set_button(map_menu_window, 20, 2, "BACK", map_menu_buttons, button_x, button_y * 9, button_width, button_height);
}

void menu_update_map_list_window(Window *self, int button_id)
{
    switch(button_id)
    {
        case 0:
            menu_initialize_map_window(button_id);
            map_initialize_base(button_id);
            map_load_entities(button_id);
            break;
        case 1:
            menu_initialize_map_window(button_id);
            map_initialize_base(button_id);
            map_load_entities(button_id);
            break;
        case 20:
            menu_pop_window(self->handle);
            break;
        default:
            break;
    }
}

void menu_initialize_map_window(int button_id)
{
    Map_Detail *level = NULL;
    level = map_parser_get_map(button_id);

    Window *map_window = NULL;
    map_window = menu_push_window();

    Sprite *map_side_menu_background = sprite_load("images/side_menu_background.png", 192, 448, 1);
    Sprite *map_side_menu_buttons = sprite_load("images/map_buttons.png", 64, 64, 3);

    //The tile dimension. 7 is how many rows there will be.
    int tile_rows = 7;
    int tile_cols = 9;
    float tile_frame = graphics_screen.h / tile_rows;

    map_window->window_frame.x = tile_frame * tile_cols;
    map_window->window_frame.y = 0;
    map_window->window_frame.w = graphics_screen.w - (tile_frame * tile_cols);
    map_window->window_frame.h = graphics_screen.h;
    map_window->background = map_side_menu_background;
    map_window->draw = menu_draw_window;
    map_window->update = menu_update_map_window;

    float padding = tile_frame / 2;

    menu_set_button(map_window, 0, 0, "Test", map_side_menu_buttons, map_window->window_frame.x + padding, map_window->window_frame.y + padding, tile_frame, tile_frame);
    menu_set_button(map_window, 1, 0, "Test", map_side_menu_buttons, map_window->window_frame.x + (padding * 4), map_window->window_frame.y + padding, tile_frame, tile_frame);
    menu_set_button(map_window, 2, 0, "Test", map_side_menu_buttons, map_window->window_frame.x + padding, map_window->window_frame.y + (padding * 11), tile_frame, tile_frame);

    //SDL_Color text_color = { 0, 0, 0 };
    //font_draw_text("X", map_window->window_frame.x + (padding * 3), map_window->window_frame.y + (padding * 4), text_color, 2);
}

void menu_update_map_window(Window *self, int button_id)
{
    switch(button_id)
    {
        case 2:
            map_free_all();
            menu_pop_window(self->handle);
            break;
        default:
            break;
    }
}

void menu_draw_window(Window *self)
{
    sprite_draw(self->background, 0, self->window_frame.x, self->window_frame.y, self->window_frame.w, self->window_frame.h, 0, SDL_FLIP_NONE);

    for(int i = 0; i < self->button_count; i++)
    {
        sprite_draw(self->buttons[i].button, self->buttons[i].frame, self->buttons[i].box.x, self->buttons[i].box.y, self->buttons[i].box.w, self->buttons[i].box.h, 0, SDL_FLIP_NONE);
        font_draw_text(self->buttons[i].message, self->buttons[i].box.x, self->buttons[i].box.y);

        //font_draw_text(self->buttons[i].button_text, self->buttons[i].box.x, self->buttons[i].box.y, text_color, 1);
    }
}

void menu_pop_window(int handle)
{
    Window *window = NULL;
    Window *this_window = NULL;

    if(handle == -1)
    {
        //Can't kill base window
        return;
    }

    this_window = menu_get_window(handle);
    if(this_window->child != NULL)
    {
        menu_pop_window(this_window->handle);
        this_window->child = NULL;
    }

    window = WIN_TOP;
    if(window->handle != handle)
    {
        menu_bubble_window(handle);
        window = WIN_TOP;
    }

    WIN_TOP = NULL;
    window->handle = 0;
    window->inuse = false;
    memset(window, 0, sizeof(window));
    window_count--;
}

Window *menu_push_window()
{
    int i;
    if((window_count + 1) > WINDOW_MAX)
    {
        SDL_Log("menu_push_window() no more space for window -Error:");
        return NULL;
    }

    for(i = 0; i < WINDOW_MAX; i++)
    {
        if(!window_list[i].inuse)
        {
            break;
        }
    }

    window_count++;
    WIN_TOP = &window_list[i];
    memset(&window_list[i], 0, sizeof(Window));
    WIN_TOP->inuse = true;
    WIN_TOP->handle = ++window_tag;
    return WIN_TOP;
}

Window *menu_get_window(int handle)
{
    for(int i = 0; i < WINDOW_MAX; i++)
    {
        if(window_list[i].handle == handle)
        {
            return &window_list[i];
        }
    }

    return NULL;
}

void menu_bubble_window(int handle)
{
    Window *window_temp = NULL;
    int i;
    bool found = false;

    for(i = 0; i < WINDOW_MAX; i++)
    {
        if(window_stack[i]->handle == handle)
        {
            found = true;
            break;
        }
    }

    if(!found)
    {
        return;
    }

    window_temp = window_stack[i];
    for(; i < (WINDOW_MAX - 1); i++)
    {
        window_stack[i] = window_stack[i + 1];
    }
    WIN_TOP = window_temp;
}

void menu_set_button(Window *window, int button_id, int frame, char *text, Sprite *sprite, int x, int y, int w, int h)
{
    Button *button = &window->buttons[window->button_count];
    window->button_count++;

    button->button_id = button_id;
    button->frame = frame;
    strcpy(button->button_text, text);
    button->button = sprite;
    button->box.x = x;
    button->box.y = y;
    button->box.w = w;
    button->box.h = h;

    Message *message = NULL;
    message = font_load_message(text, 0, 0, 0, 1);
    button->message = message;
}

void menu_draw_all_window()
{
    for(int i = 0; i < WINDOW_MAX; i++)
    {
        if(window_stack[i] != NULL)
        {
            if(window_stack[i]->draw != NULL)
            {
                window_stack[i]->draw(window_stack[i]);
            }
        }
    }
}

void menu_update_top_window(float touch_x, float touch_y)
{
    SDL_Rect touch;
    touch.x = touch_x;
    touch.y = touch_y;
    touch.w = 0;
    touch.h = 0;

    if(WIN_TOP)
    {
        for(int i = 0; i < WIN_TOP->button_count; i++)
        {
            if(vector_rects_intersect(WIN_TOP->buttons[i].box, touch))
            {
                WIN_TOP->update(WIN_TOP, i);
            }
        }
    }
}

