#ifndef MOUSEGAME_FONT_H
#define MOUSEGAME_FONT_H

#include <SDL_ttf.h>

#include "graphics.h"
#include "vector.h"

typedef struct
{
    SDL_Color color;
    int ref;
    char message[128];
    SDL_Texture *message_texture;
    int width;
    int height;
} Message;

void font_initialize_system();

void font_close_system();

Message *font_load_message(char *message, int r, int g, int b, int size);

void font_draw_text(Message *message, int x, int y);

#endif //MOUSEGAME_FONT_H
