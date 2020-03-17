#ifndef MOUSEGAME_FONT_H
#define MOUSEGAME_FONT_H

#include <SDL_ttf.h>

#include "graphics.h"

typedef struct
{
    SDL_Color color; /**<color of font*/
    int ref; /**<keeps track of reference for this element*/
    char message[128]; /**<message to display*/
    SDL_Texture *message_texture; /**<texture of text loaded*/
    int width; /**<width of message*/
    int height; /**<height of message*/
} Message;

/**
 * @brief initializes the font system
 */
void font_initialize_system();

/**
 * @brief closes the font system
 */
void font_close_system();

/**
 * @brief loads the message
 * @param message the message to be loaded
 * @param r r value of color
 * @param g g value of color
 * @param b b value of color
 * @param size size of font
 * @return pointer to loaded message
 */
Message *font_load_message(const char *message, Uint8 r, Uint8 g, Uint8 b, int size);

/**
 * @brief draws the message
 * @param message the message to draw
 * @param x x coordinate of message
 * @param y y coordinate of message
 * @param padding padding of message
 */
void font_draw_text(Message *message, int x, int y, int padding);

#endif //MOUSEGAME_FONT_H
