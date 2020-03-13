#include "hud.h"

Window *window_stack[WINDOW_MAX];
Window window_list[WINDOW_MAX];
int window_count = 0;
int window_tag = 0;

#define WIN_TOP window_stack[window_count - 1]

void hud_initialize_system()
{
    for(int i = 0; i < WINDOW_MAX; i++)
    {
        window_stack[i] = NULL;
        if(window_list[i].background != NULL)
        {
            sprite_free(&window_list[i].background);
            memset(&window_stack[i], 0, sizeof(Window));
            window_list[i].handle = -1;
        }
    }

    window_count = 0;
}

void hud_pop_window(int handle)
{
    Window *window = NULL;
    Window *this_window = NULL;

    if(handle == -1)
    {
        //Can't kill base window
        return;
    }

    this_window = hud_get_window(handle);
    if(this_window->child != NULL)
    {
        hud_pop_window(this_window->handle);
        this_window->child = NULL;
    }

    window = WIN_TOP;
    if(window->handle != handle)
    {
        hud_bubble_window(handle);
        window = WIN_TOP;
    }

    WIN_TOP = NULL;
    window->handle = 0;
    window->inuse = false;
    memset(window, 0, sizeof(window));
    window_count--;
}

Window *hud_push_window()
{
    int i;
    if((window_count + 1) > WINDOW_MAX)
    {
        SDL_Log("hud_push_window() no more space for window -Error:");
        return NULL;
    }

    for(i = 0; i < WINDOW_MAX; i++)
    {
        if(!window_list[i].inuse)
        {
            break;
        }
    }

    window_count++;
    WIN_TOP = &window_list[i];
    memset(&window_list[i], 0, sizeof(Window));
    WIN_TOP->inuse = true;
    WIN_TOP->handle = ++window_tag;
    return WIN_TOP;
}

Window *hud_get_window(int handle)
{
    for(int i = 0; i < WINDOW_MAX; i++)
    {
        if(window_list[i].handle == handle)
        {
            return &window_list[i];
        }
    }

    return NULL;
}

void hud_bubble_window(int handle)
{
    Window *window_temp = NULL;
    int i;
    bool found = false;

    for(i = 0; i < WINDOW_MAX; i++)
    {
        if(window_stack[i]->handle == handle)
        {
            found = true;
            break;
        }
    }

    if(!found)
    {
        return;
    }

    window_temp = window_stack[i];
    for(; i < (WINDOW_MAX - 1); i++)
    {
        window_stack[i] = window_stack[i + 1];
    }
    WIN_TOP = window_temp;
}





