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

    Sprite *map_menu_background = sprite_load("images/si_pack_menu.jpg", 768, 448, 1);
    Sprite *map_menu_buttons = sprite_load("images/si_buttons.png", 128, 64, 5);

    file_parse(filename);

    map_menu_window->window_frame.x = 0;
    map_menu_window->window_frame.y = 0;
    map_menu_window->window_frame.w = graphics_reference.screen_width;
    map_menu_window->window_frame.h = graphics_reference.screen_height;
    map_menu_window->background = map_menu_background;
    map_menu_window->draw = menu_draw_window;
    map_menu_window->update = menu_update_map_list_window;

    int button_width = graphics_reference.button_width;
    int button_height = graphics_reference.button_height;
    int rbutton_width = button_width * 1.5;
    int rbutton_height = button_height * 1.5;
    int ref_col = graphics_reference.screen_width / 6;
    int ref_row = graphics_reference.screen_height / 8;
    int button_col_1 = ref_col - (button_width / 2) + (button_width / 8);
    int button_col_2 = button_col_1 + button_width + (button_width / 4);
    int button_col_3 = button_col_2 + button_width + (button_width / 4);
    int button_col_4 = button_col_3 + button_width + (button_width / 4);
    int button_col_5 = button_col_4 + button_width + (button_width / 4);
    int button_row_1 = ref_row;
    int button_row_2 = button_row_1 + button_height + (button_height / 6);
    int button_row_3 = button_row_2 + button_height + (button_height / 6);
    int button_row_4 = button_row_3 + button_height + (button_height / 6);
    int button_row_5 = button_row_4 + button_height + (button_height / 2);
    int rbutton_col_3 = (graphics_reference.screen_width / 2) - (rbutton_width / 2);

    menu_set_button(map_menu_window, 0, 0, "", SMALL, 0, map_menu_buttons, button_col_1, button_row_1, button_width, button_height);
    menu_set_button(map_menu_window, 1, 1, "", SMALL, 0, map_menu_buttons, button_col_2, button_row_1, button_width, button_height);
    menu_set_button(map_menu_window, 2, 2, "", SMALL, 0, map_menu_buttons, button_col_3, button_row_1, button_width, button_height);
    menu_set_button(map_menu_window, 3, 3, "", SMALL, 0, map_menu_buttons, button_col_4, button_row_1, button_width, button_height);
    menu_set_button(map_menu_window, 4, 4, "", SMALL, 0, map_menu_buttons, button_col_5, button_row_1, button_width, button_height);

    menu_set_button(map_menu_window, 5, 5, "", SMALL, 0, map_menu_buttons, button_col_1, button_row_2, button_width, button_height);
    menu_set_button(map_menu_window, 6, 6, "", SMALL, 0, map_menu_buttons, button_col_2, button_row_2, button_width, button_height);
    menu_set_button(map_menu_window, 7, 7, "", SMALL, 0, map_menu_buttons, button_col_3, button_row_2, button_width, button_height);
    menu_set_button(map_menu_window, 8, 8, "", SMALL, 0, map_menu_buttons, button_col_4, button_row_2, button_width, button_height);
    menu_set_button(map_menu_window, 9, 9, "", SMALL, 0, map_menu_buttons, button_col_5, button_row_2, button_width, button_height);

    menu_set_button(map_menu_window, 10, 10, "", SMALL, 0, map_menu_buttons, button_col_1, button_row_3, button_width, button_height);
    menu_set_button(map_menu_window, 11, 11, "", SMALL, 0, map_menu_buttons, button_col_2, button_row_3, button_width, button_height);
    menu_set_button(map_menu_window, 12, 12, "", SMALL, 0, map_menu_buttons, button_col_3, button_row_3, button_width, button_height);
    menu_set_button(map_menu_window, 13, 13, "", SMALL, 0, map_menu_buttons, button_col_4, button_row_3, button_width, button_height);
    menu_set_button(map_menu_window, 14, 14, "", SMALL, 0, map_menu_buttons, button_col_5, button_row_3, button_width, button_height);

    menu_set_button(map_menu_window, 15, 15, "", SMALL, 0, map_menu_buttons, button_col_1, button_row_4, button_width, button_height);
    menu_set_button(map_menu_window, 16, 16, "", SMALL, 0, map_menu_buttons, button_col_2, button_row_4, button_width, button_height);
    menu_set_button(map_menu_window, 17, 17, "", SMALL, 0, map_menu_buttons, button_col_3, button_row_4, button_width, button_height);
    menu_set_button(map_menu_window, 18, 18, "", SMALL, 0, map_menu_buttons, button_col_4, button_row_4, button_width, button_height);
    menu_set_button(map_menu_window, 19, 19, "", SMALL, 0, map_menu_buttons, button_col_5, button_row_4, button_width, button_height);

    menu_set_button(map_menu_window, 20, 20, "", SMALL, 0, map_menu_buttons, rbutton_col_3, button_row_5, rbutton_width, rbutton_height);
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
        menu_initialize_map_side_window(button_id);
        map_set_properties(PLAN, button_id);
        map_initialize_base(button_id);
        map_load_entities(button_id);
    }
}

void menu_initialize_map_side_window(int button_id)
{
    Window *map_window = NULL;
    map_window = menu_push_window();

    Map_Detail *map_detail = file_get_map(button_id);

    Sprite *map_side_menu_background = sprite_load("images/si_side_menu.png", 192, 448, 1);
    Sprite *map_side_menu_buttons = sprite_load("images/si_tiles.png", 64, 64, 8);
    Sprite *map_return_button = sprite_load("images/si_buttons.png", 128, 64, 5);
    //Sprite *count = sprite_load("images/tiles.png", 64, 64, 6);

    map_window->window_frame.x = graphics_reference.map_width;
    map_window->window_frame.y = 0;
    map_window->window_frame.w = graphics_reference.screen_width - graphics_reference.map_width;
    map_window->window_frame.h = graphics_reference.screen_height;
    map_window->background = map_side_menu_background;
    map_window->draw = menu_draw_window;
    map_window->update = menu_update_map_side_window;

    int small_button_width = graphics_reference.tile_padding;
    int small_button_height = graphics_reference.tile_padding;
    int rbutton_width = graphics_reference.button_width * 1.3;
    int rbutton_height = graphics_reference.button_height * 1.3;
    int button_col_1 = map_window->window_frame.x + (map_window->window_frame.w / 2) - (small_button_width / 2);
    int rbutton_col_1 = map_window->window_frame.x + (map_window->window_frame.w / 2) - (rbutton_width / 2);
    int button_row_1 = map_window->window_frame.y + graphics_reference.tile_padding + graphics_reference.tile_padding_4 - graphics_reference.wall_padding;
    int button_row_2 = button_row_1 + graphics_reference.tile_padding + graphics_reference.tile_padding_8;
    int button_row_3 = button_row_2 + graphics_reference.tile_padding + graphics_reference.tile_padding_8;
    int button_row_4 = button_row_3 + graphics_reference.tile_padding + graphics_reference.tile_padding_8;
    int button_row_5 = button_row_4 + graphics_reference.tile_padding + graphics_reference.tile_padding_4 + graphics_reference.tile_padding_8;

    menu_set_button(map_window, 0, 48, "", SMALL, 0, map_side_menu_buttons, button_col_1, button_row_1, small_button_width, small_button_height);
    menu_set_button(map_window, 1, 49, "", SMALL, 0, map_side_menu_buttons, button_col_1, button_row_2, small_button_width, small_button_height);
    menu_set_button(map_window, 2, 50, "", SMALL, 0, map_side_menu_buttons, button_col_1, button_row_3, small_button_width, small_button_height);
    menu_set_button(map_window, 3, 40, "", SMALL, 0, map_side_menu_buttons, button_col_1, button_row_4, small_button_width, small_button_height);
    menu_set_button(map_window, 4, 20, "", SMALL, 0, map_return_button, rbutton_col_1, button_row_5, rbutton_width, rbutton_height);
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
        case 4:
            map_free_all();
            menu_pop_window(self->handle);
            break;
        default:
            break;
    }
}

void menu_think()
{
    if(map_get_state() == WIN)
    {
        map_set_properties(PAUSE, 0);
        menu_initialize_win_window();
    }
}

void menu_initialize_win_window()
{
    Window *win_window = NULL;
    win_window = menu_push_window();

    Sprite *map_side_menu_background = sprite_load("images/si_pack_menu.jpg", 768, 448, 1);
    Sprite *map_side_menu_buttons = sprite_load("images/map_buttons.png", 64, 64, 3);

    win_window->window_frame.x = 0;
    win_window->window_frame.y = 0;
    win_window->window_frame.w = graphics_reference.screen_width;
    win_window->window_frame.h = graphics_reference.screen_height;
    win_window->background = map_side_menu_background;
    win_window->draw = menu_draw_window;
    win_window->update = menu_update_win_window;
}

void menu_update_win_window(Window *self, int button_id)
{
    //SDL_Log("%d", self->handle);
    return;
}

