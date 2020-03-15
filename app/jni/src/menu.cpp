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
    main_menu_window->draw = menu_draw_base_window;
    main_menu_window->update = menu_update_base_window;

    float button_width = main_menu_window->window_frame.w / 8;
    float button_height = main_menu_window->window_frame.h / 8;
    float button_x = main_menu_window->window_frame.x + (button_width / 2);
    float button_y = main_menu_window->window_frame.y + (button_height / 1.5);

    //SDL_Log("%d, %d", column_div, row_div);

    menu_set_button(main_menu_window, 0, 0, "Play", main_menu_buttons, button_x, button_y, button_width, button_height);
    menu_set_button(main_menu_window, 1, 2, "How to Play", main_menu_buttons, button_x, button_y * 3, button_width, button_height);

    //menu_set_button(map_window, 0, 0, "test", buttons, map_window->window_frame.x + TILE_FRAME, map_window->window_frame.y + TILE_FRAME, TILE_FRAME / 1.6, TILE_FRAME / 1.6);
}

void menu_draw_base_window(Window *self)
{
    sprite_draw(self->background, 0, self->window_frame.x, self->window_frame.y, self->window_frame.w, self->window_frame.h, 0, SDL_FLIP_NONE);
    for(int i = 0; i < self->button_count; i++)
    {
        sprite_draw(self->buttons[i].button, self->buttons[i].frame, self->buttons[i].box.x, self->buttons[i].box.y, self->buttons[i].box.w, self->buttons[i].box.h, 0, SDL_FLIP_NONE);
    }
}

void menu_update_base_window(Window *self, int button_id)
{

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
        SDL_Log("hud_push_window() no more space for window -Error:");
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


