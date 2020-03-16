#ifndef MOUSEGAME_FONT_H
#define MOUSEGAME_FONT_H

#include <SDL_ttf.h>

#include "graphics.h"
#include "vector.h"

void font_initialize_system();

void font_draw_text(char *text, int x, int y, SDL_Color color, int size);

#endif //MOUSEGAME_FONT_H
