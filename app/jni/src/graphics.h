#ifndef MOUSEGAME_GRAPHICS_H
#define MOUSEGAME_GRAPHICS_H

#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

extern SDL_Window *graphics_window; /**<the window of the game*/
extern SDL_Renderer *graphics_renderer; /**the renderer of the game*/
extern SDL_Rect graphics_screen;

/**
 * @brief initializes the graphics system
 * @param window_name the name of the window
 */
void graphics_initialize_system(char const *window_name);

/**
 * @brief closes the graphics system
 */
void graphics_close_system();


void graphics_frame_delay();


void graphics_next_frame();

#endif //MOUSEGAME_GRAPHICS_H
