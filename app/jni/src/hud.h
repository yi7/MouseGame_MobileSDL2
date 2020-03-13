#ifndef MOUSEGAME_HUD_H
#define MOUSEGAME_HUD_H

#include "graphics.h"
#include "sprite.h"

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
    SDL_Rect *window_frame;
    Button buttons[WINDOW_BUTTONS];
    Sprite *background;

    void (*draw)(struct HUDInfo_S *self);
    int (*update)(struct HUDInfo_S *self, int button_id);
} Window;

void hud_initialize_system();

void hud_pop_window(int handle);

Window *hud_push_window();

Window *hud_get_window(int handle);

void hud_bubble_window(int handle);





#endif //MOUSEGAME_HUD_H
