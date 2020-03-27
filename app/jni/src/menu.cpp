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
    atexit(menu_close_system);
}

void menu_close_system()
{
    for(int i = 0; i < WINDOW_MAX; i++)
    {
        if(window_list[i].background)
        {
            SDL_DestroyTexture(window_list[i].background->image);
        }

        for(int j = 0; j < window_list[i].button_count; j++)
        {
            SDL_DestroyTexture(window_list[i].buttons[j].message->message_texture);
        }
    }

    free(window_list);
}

void menu_initialize_base_window()
{
    Window *main_menu_window = NULL;
    Sprite *main_menu_background = NULL;
    Sprite *main_menu_buttons = NULL;

    main_menu_window = menu_push_window();
    if(!main_menu_window)
    {
        SDL_Log("menu_initialize_base_window() can't initialize base window -Error:");
    }

    main_menu_background = sprite_load("images/main_menu_background.png", 768, 441, 1);
    if(!main_menu_background)
    {
        SDL_Log("menu_initialize_base_window() can't initialize window background -Error:");
    }

    main_menu_buttons = sprite_load("images/menu_buttons.png", 128, 64, 3);
    if(!main_menu_buttons)
    {
        SDL_Log("menu_initialize_base_window() can't initialize button sprite -Error:");
    }

    main_menu_window->window_frame.x = 0;
    main_menu_window->window_frame.y = 0;
    main_menu_window->window_frame.w = graphics_reference.screen_width;
    main_menu_window->window_frame.h = graphics_reference.screen_height;
    main_menu_window->background = main_menu_background;
    main_menu_window->draw = menu_draw_window;

    //Moved update function to editor source file
    //main_menu_window->update = menu_update_base_window;

    float button_width = graphics_reference.button_width;
    float button_height = graphics_reference.button_height;
    float button_x = main_menu_window->window_frame.x + (button_width / 2);
    float button_y = main_menu_window->window_frame.y + (button_height / 1.5);
    float button_padding = graphics_reference.button_padding;

    menu_set_button(main_menu_window, 0, 0, "PLAY", SMALL, button_padding, main_menu_buttons, button_x, button_y, button_width, button_height);
    menu_set_button(main_menu_window, 1, 2, "HELP", SMALL, button_padding, main_menu_buttons, button_x, button_y * 3, button_width, button_height);
    menu_set_button(main_menu_window, 2, 2, "EDIT", SMALL, button_padding, main_menu_buttons, button_x, button_y * 5, button_width, button_height);
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
    /*if(this_window->child != NULL)
    {
        menu_pop_window(this_window->handle);
        this_window->child = NULL;
    }*/

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

void menu_draw_window(Window *self)
{
    sprite_draw(self->background, 0, self->window_frame.x, self->window_frame.y, self->window_frame.w, self->window_frame.h, 0, SDL_FLIP_NONE);

    for(int i = 0; i < self->button_count; i++)
    {
        if(self->buttons[i].selected)
        {
            sprite_draw(self->buttons[i].button, self->buttons[i].frame + 1, self->buttons[i].box.x, self->buttons[i].box.y, self->buttons[i].box.w, self->buttons[i].box.h, 0, SDL_FLIP_NONE);
        }
        else
        {
            sprite_draw(self->buttons[i].button, self->buttons[i].frame, self->buttons[i].box.x, self->buttons[i].box.y, self->buttons[i].box.w, self->buttons[i].box.h, 0, SDL_FLIP_NONE);
        }

        if(self->buttons[i].has_text)
        {
            font_draw_text(self->buttons[i].message, self->buttons[i].box.x, self->buttons[i].box.y, self->buttons[i].padding);
        }
    }
}

void menu_set_button(Window *window, int button_id, int frame, const char *text, int size, int padding, Sprite *sprite, int x, int y, int w, int h)
{
    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;

    Button *button = &window->buttons[window->button_count];
    window->button_count++;

    button->button_id = button_id;
    button->frame = frame;
    button->button = sprite;
    button->box.x = x;
    button->box.y = y;
    button->box.w = w;
    button->box.h = h;
    button->padding = padding;
    button->selected = false;

    if(strlen(text) == 0)
    {
        button->has_text = false;
    }
    else
    {
        button->has_text = true;
        Message *message = NULL;
        message = font_load_message(text, r, g, b, size);
        button->message = message;
    }
}

void menu_update_button_selection(Window *window, int button_id)
{
    for(int i = 0; i < window->button_count; i++)
    {
        if(i == button_id)
        {
            window->buttons[i].selected = true;
        }
        else
        {
            window->buttons[i].selected = false;
        }
    }
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
            if(vector_rectangle_intersect(WIN_TOP->buttons[i].box, touch))
            {
                WIN_TOP->update(WIN_TOP, i);
                break;
            }
        }
    }
}

void menu_initialize_pack_list_window()
{
    Window *pack_menu_window = NULL;
    pack_menu_window = menu_push_window();

    Sprite *pack_menu_background = sprite_load("images/main_menu_background.png", 768, 441, 1);
    Sprite *pack_menu_buttons = sprite_load("images/menu_buttons.png", 128, 64, 3);

    pack_menu_window->window_frame.x = 0;
    pack_menu_window->window_frame.y = 0;
    pack_menu_window->window_frame.w = graphics_reference.screen_width;
    pack_menu_window->window_frame.h = graphics_reference.screen_height;
    pack_menu_window->background = pack_menu_background;
    pack_menu_window->draw = menu_draw_window;
    pack_menu_window->update = menu_update_pack_list_window;

    float button_width = graphics_reference.button_width;
    float button_height = graphics_reference.button_height;
    float button_x = pack_menu_window->window_frame.x + (button_width / 2);
    float button_y = pack_menu_window->window_frame.y + (button_height / 1.5);
    float button_padding = graphics_reference.button_padding;

    menu_set_button(pack_menu_window, 0, 1, "PACK 1", SMALL, button_padding, pack_menu_buttons, button_x, button_y, button_width, button_height);
    menu_set_button(pack_menu_window, 1, 2, "BACK", SMALL, button_padding, pack_menu_buttons, button_x, button_y * 9, button_width, button_height);
}

void menu_update_pack_list_window(Window *self, int button_id)
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

    file_parse(filename);

    map_menu_window->window_frame.x = 0;
    map_menu_window->window_frame.y = 0;
    map_menu_window->window_frame.w = graphics_reference.screen_width;
    map_menu_window->window_frame.h = graphics_reference.screen_height;
    map_menu_window->background = map_menu_background;
    map_menu_window->draw = menu_draw_window;
    map_menu_window->update = menu_update_map_list_window;

    float button_width = graphics_reference.button_width;
    float button_height = graphics_reference.button_height;
    float button_x = map_menu_window->window_frame.x + (button_width / 2);
    float button_y = map_menu_window->window_frame.y + (button_height / 1.5);
    float button_padding = graphics_reference.button_padding;

    menu_set_button(map_menu_window, 0, 1, "MAP 1", SMALL, button_padding, map_menu_buttons, button_x, button_y, button_width, button_height);
    menu_set_button(map_menu_window, 1, 1, "MAP 2", SMALL, button_padding, map_menu_buttons, button_x * 4, button_y, button_width, button_height);
    menu_set_button(map_menu_window, 2, 1, "MAP 3", SMALL, button_padding, map_menu_buttons, button_x * 7, button_y, button_width, button_height);
    menu_set_button(map_menu_window, 3, 1, "MAP 4", SMALL, button_padding, map_menu_buttons, button_x * 10, button_y, button_width, button_height);
    menu_set_button(map_menu_window, 4, 1, "MAP 5", SMALL, button_padding, map_menu_buttons, button_x * 13, button_y, button_width, button_height);

    menu_set_button(map_menu_window, 5, 1, "MAP 6", SMALL, button_padding, map_menu_buttons, button_x, button_y * 3, button_width, button_height);
    menu_set_button(map_menu_window, 6, 1, "MAP 7", SMALL, button_padding, map_menu_buttons, button_x * 4, button_y * 3, button_width, button_height);
    menu_set_button(map_menu_window, 7, 1, "MAP 8", SMALL, button_padding, map_menu_buttons, button_x * 7, button_y * 3, button_width, button_height);
    menu_set_button(map_menu_window, 8, 1, "MAP 9", SMALL, button_padding, map_menu_buttons, button_x * 10, button_y * 3, button_width, button_height);
    menu_set_button(map_menu_window, 9, 1, "MAP 10", SMALL, button_padding, map_menu_buttons, button_x * 13, button_y * 3, button_width, button_height);

    menu_set_button(map_menu_window, 10, 1, "MAP 11", SMALL, button_padding, map_menu_buttons, button_x, button_y * 5, button_width, button_height);
    menu_set_button(map_menu_window, 11, 1, "MAP 12", SMALL, button_padding, map_menu_buttons, button_x * 4, button_y * 5, button_width, button_height);
    menu_set_button(map_menu_window, 12, 1, "MAP 13", SMALL, button_padding, map_menu_buttons, button_x * 7, button_y * 5, button_width, button_height);
    menu_set_button(map_menu_window, 13, 1, "MAP 14", SMALL, button_padding, map_menu_buttons, button_x * 10, button_y * 5, button_width, button_height);
    menu_set_button(map_menu_window, 14, 1, "MAP 15", SMALL, button_padding, map_menu_buttons, button_x * 13, button_y * 5, button_width, button_height);

    menu_set_button(map_menu_window, 15, 1, "MAP 16", SMALL, button_padding, map_menu_buttons, button_x, button_y * 7, button_width, button_height);
    menu_set_button(map_menu_window, 16, 1, "MAP 17", SMALL, button_padding, map_menu_buttons, button_x * 4, button_y * 7, button_width, button_height);
    menu_set_button(map_menu_window, 17, 1, "MAP 18", SMALL, button_padding, map_menu_buttons, button_x * 7, button_y * 7, button_width, button_height);
    menu_set_button(map_menu_window, 18, 1, "MAP 19", SMALL, button_padding, map_menu_buttons, button_x * 10, button_y * 7, button_width, button_height);
    menu_set_button(map_menu_window, 19, 1, "MAP 20", SMALL, button_padding, map_menu_buttons, button_x * 13, button_y * 7, button_width, button_height);

    menu_set_button(map_menu_window, 20, 2, "BACK", SMALL, button_padding, map_menu_buttons, button_x, button_y * 9, button_width, button_height);
}

void menu_update_map_list_window(Window *self, int button_id)
{
    if(button_id == 20)
    {
        file_free_all();
        menu_pop_window(self->handle);
    }
    else
    {
        menu_initialize_map_side_window();
        map_set_properties(PLAN, button_id);
        map_initialize_base(button_id);
        map_load_entities(button_id);
    }
}

void menu_initialize_map_side_window()
{
    Window *map_window = NULL;
    map_window = menu_push_window();

    Sprite *map_side_menu_background = sprite_load("images/side_menu_background.png", 192, 448, 1);
    Sprite *map_side_menu_buttons = sprite_load("images/map_buttons.png", 64, 64, 3);

    map_window->window_frame.x = graphics_reference.map_width;
    map_window->window_frame.y = 0;
    map_window->window_frame.w = graphics_reference.screen_width - graphics_reference.map_width;
    map_window->window_frame.h = graphics_reference.screen_height;
    map_window->background = map_side_menu_background;
    map_window->draw = menu_draw_window;
    map_window->update = menu_update_map_side_window;

    float tile_frame = graphics_reference.tile_padding;
    float menu_padding = graphics_reference.tile_padding / 2;
    float button_padding = graphics_reference.wall_padding;

    menu_set_button(map_window, 0, 0, "PLAY", SMALL, button_padding, map_side_menu_buttons, map_window->window_frame.x + menu_padding, map_window->window_frame.y + menu_padding, tile_frame, tile_frame);
    menu_set_button(map_window, 1, 0, "PAUSE", SMALL, button_padding, map_side_menu_buttons, map_window->window_frame.x + (menu_padding * 4), map_window->window_frame.y + menu_padding, tile_frame, tile_frame);
    menu_set_button(map_window, 2, 0, "RESET", SMALL, button_padding, map_side_menu_buttons, map_window->window_frame.x + menu_padding, map_window->window_frame.y + (menu_padding * 4), tile_frame, tile_frame);
    menu_set_button(map_window, 3, 0, "BACK", SMALL, button_padding, map_side_menu_buttons, map_window->window_frame.x + (menu_padding * 4), map_window->window_frame.y + (menu_padding * 11), tile_frame, tile_frame);
}

void menu_update_map_side_window(Window *self, int button_id)
{
    switch(button_id)
    {
        case 0:
            map_play();
            break;
        case 1:
            map_stop();
            break;
        case 2:
            map_reset();
            break;
        case 3:
            map_free_all();
            menu_pop_window(self->handle);
            break;
        default:
            break;
    }
}



