#ifndef MOUSEGAME_MENU_H
#define MOUSEGAME_MENU_H

#include "graphics.h"
#include "sprite.h"
#include "vector.h"
#include "map_parser.h"
#include "map.h"
#include "font.h"

#define WINDOW_BUTTONS 64
#define WINDOW_MAX 32

typedef struct
{
    Sprite *button;
    SDL_Rect box;
    char button_text[80];
    int frame;
    int button_id;
} Button;

typedef struct Window_S
{
    bool inuse;
    int handle;
    struct Window_S *parent;
    struct Window_S *child;
    SDL_Rect window_frame;
    Button buttons[WINDOW_BUTTONS];
    int button_count;
    Sprite *background;

    void (*draw)(struct Window_S *self);
    void (*update)(struct Window_S *self, int button_id);
} Window;

void menu_initialize_system();

void menu_initialize_base_window();

void menu_update_base_window(Window *self, int button_id);

void menu_initialize_pack_window();

void menu_update_pack_window(Window *self, int button_id);

void menu_initialize_map_list_window(char *filename);

void menu_update_map_list_window(Window *self, int button_id);

void menu_initialize_map_window(int button_id);

void menu_update_map_window(Window *self, int button_id);

void menu_draw_window(Window *self);

void menu_pop_window(int handle);

Window *menu_push_window();

Window *menu_get_window(int handle);

void menu_bubble_window(int handle);

void menu_set_button(Window *window, int button_id, int frame, char *text, Sprite *sprite, int x, int y, int w, int h);

void menu_draw_all_window();

void menu_update_top_window(float touch_x, float touch_y);



#endif //MOUSEGAME_MENU_H
