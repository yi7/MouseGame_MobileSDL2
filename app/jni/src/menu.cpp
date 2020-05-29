#include "menu.h"

Window *window_stack[WINDOW_MAX];
Window window_list[WINDOW_MAX];
int window_count = 0;
int window_tag = 0;
int active_button_id;

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
    Sprite *main_menu_play_button = NULL;
    Sprite *main_menu_help_button = NULL;

    main_menu_window = menu_push_window();
    if(!main_menu_window)
    {
        SDL_Log("menu_initialize_base_window() can't initialize base window -Error:");
    }

    main_menu_background = sprite_load("images/si_main_menu.jpg", 768, 448, 1);
    if(!main_menu_background)
    {
        SDL_Log("menu_initialize_base_window() can't initialize window background -Error:");
    }

    main_menu_play_button = sprite_load("images/si_play_button.png", 192, 192, 1);
    if(!main_menu_play_button)
    {
        SDL_Log("menu_initialize_base_window() can't initialize button sprite -Error:");
    }

    main_menu_help_button = sprite_load("images/si_help_button.png", 128, 128, 1);

    main_menu_window->window_frame.x = 0;
    main_menu_window->window_frame.y = 0;
    main_menu_window->window_frame.w = graphics_reference.screen_width;
    main_menu_window->window_frame.h = graphics_reference.screen_height;
    main_menu_window->background = main_menu_background;
    main_menu_window->draw = menu_draw_window;
    main_menu_window->frame = 0;

    //Moved update function to editor source file
    //main_menu_window->update = menu_update_base_window;

    int play_w = graphics_reference.tile_padding * 3;
    int play_x = (graphics_reference.screen_width / 2) - (play_w / 2);
    int play_y = (graphics_reference.screen_height / 2) - (play_w / 2);

    int help_x = (graphics_reference.screen_width / 2) - (graphics_reference.tile_padding);
    int help_y = play_y + play_w - graphics_reference.tile_padding_4;
    int help_w = graphics_reference.tile_padding * 2;

    //int play_x = (graphics_reference.screen_width / 2) -

    menu_set_button(main_menu_window, 0, 0, "", SMALL, 0, main_menu_play_button, play_x, play_y, play_w, play_w);
    menu_set_button(main_menu_window, 1, 0, "", SMALL, 0, main_menu_help_button, help_x, help_y, help_w, help_w);
    //menu_set_button(main_menu_window, 2, 2, "EDIT", SMALL, button_padding, main_menu_buttons, button_x, button_y * 5, button_width, button_height);
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
    WIN_TOP->map_open = false;
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
    sprite_draw(self->background, self->frame, self->window_frame.x, self->window_frame.y, self->window_frame.w, self->window_frame.h, 0, SDL_FLIP_NONE);

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

void menu_draw_top_window()
{
    sprite_draw(WIN_TOP->background, WIN_TOP->frame, WIN_TOP->window_frame.x, WIN_TOP->window_frame.y, WIN_TOP->window_frame.w, WIN_TOP->window_frame.h, 0, SDL_FLIP_NONE);

    for(int i = 0; i < WIN_TOP->button_count; i++)
    {
        if(WIN_TOP->buttons[i].selected)
        {
            sprite_draw(WIN_TOP->buttons[i].button, WIN_TOP->buttons[i].frame + 1, WIN_TOP->buttons[i].box.x, WIN_TOP->buttons[i].box.y, WIN_TOP->buttons[i].box.w, WIN_TOP->buttons[i].box.h, 0, SDL_FLIP_NONE);
        }
        else
        {
            sprite_draw(WIN_TOP->buttons[i].button, WIN_TOP->buttons[i].frame, WIN_TOP->buttons[i].box.x, WIN_TOP->buttons[i].box.y, WIN_TOP->buttons[i].box.w, WIN_TOP->buttons[i].box.h, 0, SDL_FLIP_NONE);
        }

        if(WIN_TOP->buttons[i].has_text)
        {
            font_draw_text(WIN_TOP->buttons[i].message, WIN_TOP->buttons[i].box.x, WIN_TOP->buttons[i].box.y, WIN_TOP->buttons[i].padding);
        }
    }
}

void menu_initialize_pack_list_window()
{
    Window *pack_menu_window = NULL;
    pack_menu_window = menu_push_window();

    Sprite *pack_menu_background = sprite_load("images/si_screen_select.png", 768, 448, 1);
    Sprite *pack_menu_buttons = sprite_load("images/si_pack_buttons.png", 128, 256, 5);
    Sprite *return_button = sprite_load("images/si_buttons.png", 128, 64, 5);

    pack_menu_window->window_frame.x = 0;
    pack_menu_window->window_frame.y = 0;
    pack_menu_window->window_frame.w = graphics_reference.screen_width;
    pack_menu_window->window_frame.h = graphics_reference.screen_height;
    pack_menu_window->background = pack_menu_background;
    pack_menu_window->draw = menu_draw_window;
    pack_menu_window->update = menu_update_pack_list_window;
    pack_menu_window->frame = 0;

    int button_width = graphics_reference.tile_padding * 2;
    int button_height = graphics_reference.tile_padding * 4;
    int pack1_x = graphics_reference.tile_padding + graphics_reference.tile_padding_8;
    int pack2_x = pack1_x + button_width + graphics_reference.tile_padding_8;
    //int pack3_x = pack2_x + button_width + graphics_reference.tile_padding_8;
    //int pack4_x = pack3_x + button_width + graphics_reference.tile_padding_8;
    //int pack5_x = pack4_x + button_width + graphics_reference.tile_padding_8;
    int button_y = graphics_reference.screen_height / 6;

    float rbutton_width = graphics_reference.button_width;
    float rbutton_height = graphics_reference.button_height;
    float rbutton_x = (graphics_reference.screen_width / 2) - (rbutton_width / 2);
    float rbutton_y = (graphics_reference.tile_padding * 5) + (graphics_reference.tile_padding_4 * 2);

    menu_set_button(pack_menu_window, 0, 0, "", SMALL, 0, pack_menu_buttons, pack1_x, button_y, button_width, button_height);
    menu_set_button(pack_menu_window, 1, 1, "", SMALL, 0, pack_menu_buttons, pack2_x, button_y, button_width, button_height);
    //menu_set_button(pack_menu_window, 0, 2, "", SMALL, 0, pack_menu_buttons, pack3_x, button_y, button_width, button_height);
    //menu_set_button(pack_menu_window, 0, 3, "", SMALL, 0, pack_menu_buttons, pack4_x, button_y, button_width, button_height);
    //menu_set_button(pack_menu_window, 0, 4, "", SMALL, 0, pack_menu_buttons, pack5_x, button_y, button_width, button_height);
    menu_set_button(pack_menu_window, 2, 20, "", SMALL, 0, return_button, rbutton_x, rbutton_y, rbutton_width, rbutton_height);
}

void menu_update_pack_list_window(Window *self, int button_id)
{
    switch(button_id)
    {
        case 0:
            menu_initialize_map_list_window("files/pack1.txt");
            break;
        case 1:
            menu_initialize_map_list_window("files/pack2.txt");
            break;
        case 2:
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

    Sprite *map_menu_background = sprite_load("images/si_screen_select2.png", 768, 448, 1);
    Sprite *map_menu_buttons = sprite_load("images/si_buttons.png", 128, 64, 5);

    file_parse(filename);

    map_menu_window->window_frame.x = 0;
    map_menu_window->window_frame.y = 0;
    map_menu_window->window_frame.w = graphics_reference.screen_width;
    map_menu_window->window_frame.h = graphics_reference.screen_height;
    map_menu_window->background = map_menu_background;
    map_menu_window->draw = menu_draw_window;
    map_menu_window->update = menu_update_map_list_window;
    map_menu_window->frame = 0;

    int button_width = graphics_reference.button_width;
    int button_height = graphics_reference.button_height;
    int rbutton_width = button_width;
    int rbutton_height = button_height;
    int ref_col_mid = graphics_reference.screen_width / 2;
    int ref_row = graphics_reference.tile_padding + graphics_reference.tile_padding_4;

    int button_col_3 = ref_col_mid - (button_width / 2);
    int button_col_2 = button_col_3 - button_width;
    int button_col_1 = button_col_2 - button_width;
    int button_col_4 = button_col_3 + button_width;
    int button_col_5 = button_col_4 + button_width;

    int button_row_1 = ref_row;
    int button_row_2 = button_row_1 + button_height;
    int button_row_3 = button_row_2 + button_height;
    int button_row_4 = button_row_3 + button_height;
    int button_row_5 = button_row_4 + button_height + graphics_reference.tile_padding_4;

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

    menu_set_button(map_menu_window, 20, 20, "", SMALL, 0, map_menu_buttons, button_col_3, button_row_5, rbutton_width, rbutton_height);
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
        active_button_id = button_id;
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

    map_window->window_frame.x = graphics_reference.map_width;
    map_window->window_frame.y = 0;
    map_window->window_frame.w = graphics_reference.screen_width - graphics_reference.map_width;
    map_window->window_frame.h = graphics_reference.screen_height;
    map_window->background = map_side_menu_background;
    map_window->draw = menu_draw_window;
    map_window->update = menu_update_map_side_window;
    map_window->map_open = true;
    map_window->frame = 0;

    int small_button_width = graphics_reference.tile_padding;
    int small_button_height = graphics_reference.tile_padding;
    int rbutton_width = graphics_reference.button_width;
    int rbutton_height = graphics_reference.button_height;
    int button_col_1 = map_window->window_frame.x + (map_window->window_frame.w / 2) - (small_button_width / 2);
    int rbutton_col_1 = map_window->window_frame.x + (map_window->window_frame.w / 2) - (rbutton_width / 2);
    int button_row_1 = map_window->window_frame.y + graphics_reference.tile_padding + graphics_reference.tile_padding_4 - graphics_reference.wall_padding;
    int button_row_2 = button_row_1 + graphics_reference.tile_padding + graphics_reference.tile_padding_8;
    int button_row_3 = button_row_2 + graphics_reference.tile_padding + graphics_reference.tile_padding_8;
    int button_row_4 = button_row_3 + graphics_reference.tile_padding + graphics_reference.tile_padding_8;
    int button_row_5 = button_row_4 + graphics_reference.tile_padding + graphics_reference.tile_padding_4 + graphics_reference.tile_padding_8;

    menu_set_button(map_window, 0, 56, "", SMALL, 0, map_side_menu_buttons, button_col_1, button_row_1, small_button_width, small_button_height);
    menu_set_button(map_window, 1, 57, "", SMALL, 0, map_side_menu_buttons, button_col_1, button_row_2, small_button_width, small_button_height);
    menu_set_button(map_window, 2, 58, "", SMALL, 0, map_side_menu_buttons, button_col_1, button_row_3, small_button_width, small_button_height);
    menu_set_button(map_window, 3, 48, "", SMALL, 0, map_side_menu_buttons, button_col_1, button_row_4, small_button_width, small_button_height);
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
        menu_initialize_win_window();
    }

    //SDL_Log("%d", WIN_TOP->handle);

    if(WIN_TOP->map_open)
    {
        int arrow_placed = map_get_arrow_max() - map_get_arrow_count();
        WIN_TOP->buttons[3].frame = arrow_placed + 48;
    }
}

void menu_initialize_win_window()
{
    Window *win_window = NULL;
    win_window = menu_push_window();

    Sprite *map_side_menu_background = sprite_load("images/si_level_complete.png", 768, 448, 1);
    Sprite *map_menu_buttons = sprite_load("images/si_tiles.png", 64, 64, 8);
    Sprite *map_return_button = sprite_load("images/si_buttons.png", 128, 64, 5);

    win_window->window_frame.x = 0;
    win_window->window_frame.y = 0;
    win_window->window_frame.w = graphics_reference.screen_width;
    win_window->window_frame.h = graphics_reference.screen_height;
    win_window->background = map_side_menu_background;
    win_window->draw = menu_draw_window;
    win_window->update = menu_update_win_window;
    win_window->frame = 0;

    float button_width = graphics_reference.tile_padding;
    int button_col_1 = (graphics_reference.screen_width / 2) - (graphics_reference.tile_padding / 2);
    int rbutton_col_1 = (graphics_reference.screen_width / 2) - (graphics_reference.button_width / 2);
    int button_row_1 = (graphics_reference.screen_height / 2) - (graphics_reference.tile_padding * 2) + graphics_reference.tile_padding_4;
    int button_row_2 = button_row_1 + graphics_reference.tile_padding + graphics_reference.tile_padding_2;
    int button_row_3 = button_row_2 + graphics_reference.tile_padding + graphics_reference.tile_padding_2;

    menu_set_button(win_window, 0, 65, "", SMALL, 0, map_menu_buttons, button_col_1, button_row_1, button_width, button_width);
    menu_set_button(win_window, 1, 66, "", SMALL, 0, map_menu_buttons, button_col_1, button_row_2, button_width, button_width);
    menu_set_button(win_window, 2, 20, "", SMALL, 0, map_return_button, rbutton_col_1, button_row_3, graphics_reference.button_width, graphics_reference.button_height);
}

void menu_update_win_window(Window *self, int button_id)
{

    switch(button_id)
    {
        case 0:
            if(active_button_id + 1 < 20)
            {
                active_button_id++;
                menu_pop_window(self->handle);
                map_free_all();
                menu_pop_window(WIN_TOP->handle);
                menu_initialize_map_side_window(active_button_id);
                map_set_properties(PLAN, active_button_id);
                map_initialize_base(active_button_id);
                map_load_entities(active_button_id);
            }
            else
            {
                menu_pop_window(self->handle);
                map_free_all();
                menu_pop_window(WIN_TOP->handle);
            }

            break;
        case 1:
            menu_pop_window(self->handle);
            map_free_all();
            menu_pop_window(WIN_TOP->handle);
            break;
        case 2:
            menu_pop_window(self->handle);
            break;
        default:
            break;
    }
}

void menu_initialize_help_window()
{
    Window *help_window = NULL;
    help_window = menu_push_window();

    Sprite *help_images = sprite_load("images/si_help.jpg", 768, 448, 3);
    Sprite *next_button = sprite_load("images/si_tiles.png", 64, 64, 8);

    help_window->window_frame.x = 0;
    help_window->window_frame.y = 0;
    help_window->window_frame.w = graphics_reference.screen_width;
    help_window->window_frame.h = graphics_reference.screen_height;
    help_window->background = help_images;
    help_window->draw = menu_draw_window;
    help_window->update = menu_update_help_window;
    help_window->frame = 0;

    int button_row = (graphics_reference.screen_height / 2) - (graphics_reference.tile_padding_2);
    int button_left = 0;
    int button_right = graphics_reference.screen_width - graphics_reference.tile_padding;

    menu_set_button(help_window, 0, 62, "", SMALL, 0, next_button, button_left, button_row, graphics_reference.tile_padding, graphics_reference.tile_padding);
    menu_set_button(help_window, 1, 63, "", SMALL, 0, next_button, button_right, button_row, graphics_reference.tile_padding, graphics_reference.tile_padding);

    /*
    float button_width = graphics_reference.tile_padding;
    int button_col_1 = (graphics_reference.screen_width / 2) - (graphics_reference.tile_padding / 2);
    int rbutton_col_1 = (graphics_reference.screen_width / 2) - (graphics_reference.button_width / 2);
    int button_row_1 = (graphics_reference.screen_height / 2) - (graphics_reference.tile_padding * 2) + graphics_reference.tile_padding_4;
    int button_row_2 = button_row_1 + graphics_reference.tile_padding + graphics_reference.tile_padding_2;
    int button_row_3 = button_row_2 + graphics_reference.tile_padding + graphics_reference.tile_padding_2;

    menu_set_button(win_window, 0, 65, "", SMALL, 0, map_menu_buttons, button_col_1, button_row_1, button_width, button_width);
    menu_set_button(win_window, 1, 66, "", SMALL, 0, map_menu_buttons, button_col_1, button_row_2, button_width, button_width);
    menu_set_button(win_window, 2, 20, "", SMALL, 0, map_return_button, rbutton_col_1, button_row_3, graphics_reference.button_width, graphics_reference.button_height);*/
}

void menu_update_help_window(Window *self, int button_id)
{
    switch(button_id)
    {
        case 0:
            if(self->frame == 0)
            {
                self->frame = 5;
            }
            else
            {
                self->frame--;
            }
            break;
        case 1:
            if(self->frame == 5)
            {
                self->frame = 0;
            }
            else
            {
                self->frame++;
            }
        default:
            break;
    }
}
