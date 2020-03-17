#ifndef MOUSEGAME_MENU_H
#define MOUSEGAME_MENU_H

#include "graphics.h"
#include "sprite.h"
#include "vector.h"
#include "font.h"

#define WINDOW_BUTTON_MAX 64
#define WINDOW_MAX 32
#define BUTTON_TEXT_MAX 80

typedef struct
{
    Sprite *button; /**<sprite associated with button*/
    SDL_Rect box; /**<position and frame size of button*/
    int frame; /**<frame of the button*/
    int button_id; /**<id of button*/
    int padding; /**<padding of button*/
    Message *message; /**<message associated with button*/
} Button;

typedef struct Window_S
{
    bool inuse; /**<flag for tracking resource*/
    int handle; /**<id of window*/
    //struct Window_S *parent;
    //struct Window_S *child;
    SDL_Rect window_frame; /**<position and frame size of window*/
    Button buttons[WINDOW_BUTTON_MAX]; /**<list of buttons associated with window*/
    int button_count; /**<total of buttons on window*/
    Sprite *background; /**<sprite associated with window*/

    void (*draw)(struct Window_S *self); /**<draw function of window*/
    void (*update)(struct Window_S *self, int button_id); /**<update function of window*/
} Window;

/**
 * @brief initializes the menu system
 */
void menu_initialize_system();

/**
 * @brief closes the menu system
 */
void menu_close_system();

/**
 * @brief initializes the base window
 */
void menu_initialize_base_window();

/**
 * @brief update function of the base window
 * @param self current window
 * @param button_id id of button pressed on window
 */
void menu_update_base_window(Window *self, int button_id);

/**
 * @brief pops the window from the stack
 * @param handle the id of window to pop
 */
void menu_pop_window(int handle);

/**
 * @brief pushes the window onto the stack
 * @return pointer to the pushed window
 */
Window *menu_push_window();

/**
 * @brief gets the window from list
 * @param handle the id of window to get
 * @return pointer to the window found, otherwise NULL
 */
Window *menu_get_window(int handle);

/**
 * @brief shifts a window to the top of the stack
 * @param handle the id of window to shift
 */
void menu_bubble_window(int handle);

/**
 * @brief draw function of windows
 * @param self the window to draw
 */
void menu_draw_window(Window *self);

/**
 * @brief initializes button on window
 * @param window window to add button to
 * @param button_id id of button
 * @param frame frame of button
 * @param text text to display on button
 * @param size size of text
 * @param sprite sprite associated with button
 * @param x x coordinate of button
 * @param y y coordinate of button
 * @param w width of button
 * @param h height of button
 */
void menu_set_button(Window *window, int button_id, int frame, const char *text, int size, int padding, Sprite *sprite, int x, int y, int w, int h);

/**
 * @brief draws all window on stack
 */
void menu_draw_all_window();

/**
 * @brief updates top window
 * @param touch_x x coordinate where user touched
 * @param touch_y y coordinate where user touched
 */
void menu_update_top_window(float touch_x, float touch_y);

/**
 * @brief initializes pack list window
 */
void menu_initialize_pack_list_window();

/**
 * @brief update function of pack list window
 * @param self current window
 * @param button_id id of button pressed
 */
void menu_update_pack_list_window(Window *self, int button_id);

/**
 * @brief initializes map list window
 * @param filename file to load the pack maps
 */
void menu_initialize_map_list_window(char *filename);

/**
 * @brief update function of map list window
 * @param self current window
 * @param button_id id of button pressed
 */
void menu_update_map_list_window(Window *self, int button_id);

/*
void menu_initialize_map_window(int button_id);

void menu_update_map_window(Window *self, int button_id);
*/



#endif //MOUSEGAME_MENU_H
