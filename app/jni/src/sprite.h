#ifndef MOUSEGAME_SPRITE_H
#define MOUSEGAME_SPRITE_H

#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

#include "graphics.h"
#include "vector.h"

typedef struct
{
    int ref; /**<keeps track of reference to this element*/
    int fpl; /**<frames per line in sprite sheet*/
    char filename[128]; /**<name of image file loaded*/
    SDL_Texture *image; /**<texture of the image loaded*/
    Vector2D image_size; /**<the dimensions of the image loaded*/
    Vector2D frame_size; /**<the dimensions of the frame in sprite sheet*/
} Sprite;

/**
 * @brief initializes the sprite system
 */
void sprite_initialize_system();

/**
 * @brief closes the sprite system
 */
void sprite_close_system();

/**
 * @brief loads the sprite
 * @param filename the path of image to be loaded
 * @param frame_width width of the image
 * @param frame_height height of the image
 * @param fpl frames per line
 * @return pointer to loaded sprite
 */
Sprite* sprite_load(char *filename, int frame_width, int frame_height, int fpl);

/**
 * @brief frees loaded sprite from memory
 * @param sprite the sprite to be freed
 */
void sprite_free(Sprite **sprite);

/**
 * @brief draws the sprite
 * @param sprite sprite to draw
 * @param frame index of sprite to draw
 * @param x x coordinate of sprite
 * @param y y coordinate of sprite
 * @param width width of sprite to draw
 * @param height height of sprite to draw
 * @param angle angle of the sprite
 * @param center center of the sprite to rotate from
 * @param flip SDL_RendererFlip value
 */
void sprite_draw(Sprite *sprite, int frame, int x, int y, int width, int height, int angle, SDL_RendererFlip flip);

#endif //MOUSEGAME_SPRITE_H
