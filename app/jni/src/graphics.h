#ifndef MOUSEGAME_GRAPHICS_H
#define MOUSEGAME_GRAPHICS_H

#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

extern SDL_Window *graphics_window;
extern SDL_Renderer *graphics_renderer;
extern SDL_Rect graphics_screen;

void graphics_initialize_system(char const *window_name);
void graphics_close_system();
void graphics_frame_delay();
void graphics_next_frame();

#endif //MOUSEGAME_GRAPHICS_H
