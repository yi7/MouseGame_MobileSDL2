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

    menu_set_button(base, 2, 2, "EDIT", SMALL, button_padding, main_menu_buttons, button_x, button_y * 5, button_width, button_height);
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
            editor_initialize_side_window();
            editor_load_empty_map();
            map_initialize_base(0);
            map_load_entities(0);
            break;
        default:
            break;
    }
}

void editor_initialize_side_window()
{
    Window *editor_window = NULL;
    editor_window = menu_push_window();

    Sprite *map_side_menu_background = sprite_load("images/side_menu_background.png", 192, 448, 1);
    Sprite *map_side_menu_buttons = sprite_load("images/small_buttons.png", 64, 64, 8);

    editor_window->window_frame.x = graphics_reference.map_width;
    editor_window->window_frame.y = 0;
    editor_window->window_frame.w = graphics_reference.screen_width - graphics_reference.map_width;
    editor_window->window_frame.h = graphics_reference.screen_height;
    editor_window->background = map_side_menu_background;
    editor_window->draw = menu_draw_window;
    editor_window->update = editor_update_side_window;

    float tile_frame = graphics_reference.tile_padding;
    float menu_padding = graphics_reference.tile_padding / 2;
    float inbetween_padding = menu_padding / 2;
    float button_padding = graphics_reference.wall_padding;

    menu_set_button(editor_window, 0, 0, "TEST", SMALL, button_padding, map_side_menu_buttons, editor_window->window_frame.x + menu_padding, editor_window->window_frame.y + menu_padding, tile_frame, tile_frame);
    menu_set_button(editor_window, 1, 0, "CLEAR ALL", SMALL, button_padding, map_side_menu_buttons, editor_window->window_frame.x + (menu_padding * 4), editor_window->window_frame.y + menu_padding, tile_frame, tile_frame);
    menu_set_button(editor_window, 2, 16, "MOUSE", SMALL, button_padding, map_side_menu_buttons, editor_window->window_frame.x + menu_padding, editor_window->window_frame.y + (menu_padding * 3) + inbetween_padding, tile_frame, tile_frame);
    menu_set_button(editor_window, 3, 32, "CAT", SMALL, button_padding, map_side_menu_buttons, editor_window->window_frame.x + (menu_padding * 4), editor_window->window_frame.y + (menu_padding * 3) + inbetween_padding, tile_frame, tile_frame);
    menu_set_button(editor_window, 4, 48, "TILE", SMALL, button_padding, map_side_menu_buttons, editor_window->window_frame.x + menu_padding, editor_window->window_frame.y + (menu_padding * 6), tile_frame, tile_frame);
    menu_set_button(editor_window, 5, 8, "WALL", SMALL, button_padding, map_side_menu_buttons, editor_window->window_frame.x + (menu_padding * 4), editor_window->window_frame.y + (menu_padding * 6), tile_frame, tile_frame);
    menu_set_button(editor_window, 6, 0, "BACK", SMALL, button_padding, map_side_menu_buttons, editor_window->window_frame.x + (menu_padding * 4), editor_window->window_frame.y + (menu_padding * 11), tile_frame, tile_frame);
}

void editor_update_side_window(Window *self, int button_id)
{
    switch(button_id)
    {
        case 0:
            //menu_initialize_editor_test_window();
            //map_save_edit();
            //map_set_state(PLAN);
            //map_play();
            break;
        case 1:
            //map_free_all();
            //map_initialize_base(0, EDIT);
            //map_load_entities(0);
            break;
        case 2:
            //menu_initialize_mouse_select_window();
            break;
        case 3:
            //menu_initialize_cat_select_window();
            break;
        case 4:
            //menu_initialize_tile_select_window();
            break;
        case 5:
            //menu_initialize_wall_select_window();
            break;
        case 6:
            map_free_all();
            file_free_all();
            menu_pop_window(self->handle);
            break;
        default:
            break;
    }
}

void editor_load_empty_map()
{
    Map_Detail *editor;
    editor = file_new();
    if(!editor)
    {
        SDL_Log("editor_load_empty_map: Could not get Map_Detail struct");
    }

    char map[285][3] = {"13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13",
                        "11", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "11",
                        "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13",
                        "11", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "11",
                        "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13",
                        "11", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "11",
                        "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13",
                        "11", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "11",
                        "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13",
                        "11", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "11",
                        "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13",
                        "11", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "11",
                        "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13", "15", "13",
                        "11", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "14", "00", "11",
                        "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13", "12", "13"};

    for(int i = 0; i < (sizeof(editor->map) / sizeof(editor->map[0])); i++)
    {
        strcpy(editor->map[i], map[i]);
    }
    editor->arrow_count = 0;
    editor->entity_count = 0;
    editor->map_name = (char *)malloc(10);
    strcpy(editor->map_name, "Editor");
}

void editor_initialize_mouse_select_window()
{
    Window *select_window = NULL;
    select_window = menu_push_window();

    Sprite *select_background = sprite_load("images/side_menu_test.png", 192, 448, 1);
    Sprite *select_buttons = sprite_load("images/small_buttons.png", 64, 64, 8);

    select_window->window_frame.x = graphics_reference.map_width;
    select_window->window_frame.y = 0;
    select_window->window_frame.w = graphics_reference.screen_width - graphics_reference.map_width;
    select_window->window_frame.h = graphics_reference.screen_height;
    select_window->background = select_background;
    select_window->draw = menu_draw_window;
    select_window->update = editor_update_mouse_select_window;

    float tile_frame = graphics_reference.tile_padding;
    float menu_padding = graphics_reference.tile_padding / 2;
    float inbetween_padding = menu_padding / 2;
    float button_padding = graphics_reference.wall_padding;

    menu_set_button(select_window, 0, 16, "", SMALL, button_padding, select_buttons, select_window->window_frame.x + menu_padding, select_window->window_frame.y + menu_padding, tile_frame, tile_frame);
    menu_set_button(select_window, 1, 8, "REMOVE", SMALL, button_padding, select_buttons, select_window->window_frame.x + menu_padding, select_window->window_frame.y + (menu_padding * 8) + inbetween_padding, tile_frame, tile_frame);
    menu_set_button(select_window, 2, 0, "BACK", SMALL, button_padding, select_buttons, select_window->window_frame.x + (menu_padding * 4), select_window->window_frame.y + (menu_padding * 11), tile_frame, tile_frame);

}

void editor_update_mouse_select_window(Window *self, int button_id)
{
    switch(button_id)
    {
        case 0:
            //menu_update_button_selection(self, button_id);
            //map_change_edit_type(EMOUSE_NORMAL);
            break;
        case 1:
            //menu_update_button_selection(self, button_id);
            //map_change_edit_type(EMOUSE_REMOVE);
            break;
        case 2:
            //map_change_edit_type(NONE);
            menu_pop_window(self->handle);
            break;
        default:
            break;
    }
}

void editor_initialize_cat_select_window();

void editor_update_cat_select_window(Window *self, int button_id);

void editor_initialize_tile_select_window();

void editor_update_tile_select_window(Window *self, int button_id);

void editor_initialize_wall_select_window();

void editor_update_wall_select_window(Window *self, int button_id);
