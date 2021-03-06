#ifndef MOUSEGAME_GRAPHICS_H
#define MOUSEGAME_GRAPHICS_H

#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_video.h>

typedef struct
{
    int map_columns; /**<total columns of map*/
    int map_rows; /**<total rows of map*/
    int tpl; /**<tiles per line of map*/
    float map_width; /**<the width of map*/
    float map_height; /**<the height of map*/
    float screen_width; /**<the phone screen width*/
    float screen_height; /**<the phone screen height*/
    int tile_padding; /**<the tile dimension*/
    int tile_padding_2;
    int tile_padding_4;
    int tile_padding_8;
    int wall_padding; /**<the map wall's width*/
    float button_width; /**<default button width*/
    float button_height; /**<default button height*/
    float button_padding; /**<default padding for button text*/
} Graphics_Reference;

//extern SDL_Window *graphics_window; /**<the window of the game*/
extern SDL_Renderer *graphics_renderer; /**<the renderer of the game*/
//extern SDL_Rect graphics_screen; /**<the screen of the game*/
extern Graphics_Reference graphics_reference; /**<contains necessary dimensions across the game*/
extern Uint64 graphics_now;
extern Uint64 graphics_then;
extern double graphics_delta;

/**
 * @brief initializes the graphics system
 * @param window_name the name of the window
 */
void graphics_initialize_system(char const *window_name);

/**
 * @brief closes the graphics system
 */
void graphics_close_system();

void graphics_restart_time();

void graphics_update_time();

#endif //MOUSEGAME_GRAPHICS_H
