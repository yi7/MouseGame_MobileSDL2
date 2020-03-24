#include "editor.h"

void editor_initialize_button_to_base()
{
    Window *base;
    base = menu_get_window(1);
    if(!base)
    {
        SDL_Log("editor_initialize_button_link() unable to get base window");
    }

    Sprite *main_menu_buttons;
    main_menu_buttons = sprite_load("images/menu_buttons.png", 128, 64, 3);
    if(!main_menu_buttons)
    {
        SDL_Log("menu_initialize_base_window() can't initialize button sprite -Error:");
    }

    float button_width = graphics_reference.button_width;
    float button_height = graphics_reference.button_height;
    float button_x = base->window_frame.x + (button_width / 2);
    float button_y = base->window_frame.y + (button_height / 1.5);
    float button_padding = graphics_reference.button_padding;

    menu_set_button(base, 2, 2, "EDIT", 0, button_padding, main_menu_buttons, button_x, button_y * 5, button_width, button_height);
    base->update = editor_update_base_window;
}

void editor_update_base_window(Window *self, int button_id)
{
    switch(button_id)
    {
        case 0:
            menu_initialize_pack_list_window();
            break;
        case 1:
            SDL_Log("Help");
            break;
        case 2:
            SDL_Log("Edit");
            break;
        default:
            break;
    }
}

void editor_initialize_side_window()
{

}

void editor_update_side_window(Window *self, int button_id)
{
    
}

