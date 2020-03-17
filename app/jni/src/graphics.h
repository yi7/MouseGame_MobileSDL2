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
    float tile_padding; /**<the tile dimension*/
    float wall_padding; /**<the map wall's width*/
    float button_width; /**<default button width*/
    float button_height; /**<default button height*/
} Graphics_Reference;

//extern SDL_Window *graphics_window; /**<the window of the game*/
extern SDL_Renderer *graphics_renderer; /**<the renderer of the game*/
//extern SDL_Rect graphics_screen; /**<the screen of the game*/
extern Graphics_Reference graphics_reference; /**<contains necessary dimensions across the game*/


/**
 * @brief initializes the graphics system
 * @param window_name the name of the window
 */
void graphics_initialize_system(char const *window_name);

/**
 * @brief closes the graphics system
 */
void graphics_close_system();

#endif //MOUSEGAME_GRAPHICS_H
