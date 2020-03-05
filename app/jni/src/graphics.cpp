#include <SDL_video.h>
#include "graphics.h"

SDL_Window *graphics_window = NULL;
SDL_Renderer *graphics_renderer = NULL;
SDL_Rect graphics_screen = {0, 0, 320, 240};

static Uint32 graphics_f_delay = 30;
static Uint32 graphics_now = 0;
static Uint32 graphics_then = 0;
static float graphics_fps = 0;

void graphics_initialize_system(char const *window_name)
{
    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("graphics_initialize_system() could not initialize -Error: %s\n", SDL_GetError());
        graphics_close_system();
        return;
    }

    //Get device display mode
    SDL_DisplayMode graphics_display;
    if(SDL_GetCurrentDisplayMode(0, &graphics_display) == 0)
    {
        graphics_screen.w = graphics_display.w;
        graphics_screen.h = graphics_display.h;
    }

    //Create window
    graphics_window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, graphics_screen.w, graphics_screen.h, SDL_WINDOW_FULLSCREEN);
    if(!graphics_window)
    {
        SDL_Log("graphics_initialize_system() window not created -Error: %s\n", SDL_GetError());
        graphics_close_system();
        return;
    }

    graphics_renderer = SDL_CreateRenderer(graphics_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!graphics_renderer)
    {
        SDL_Log("graphics_initialize_system() renderer not created -Error: %s\n", SDL_GetError());
        graphics_close_system();
        return;
    }

    //SDL_SetRenderDrawColor(graphics_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_SetRenderDrawColor(graphics_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    atexit(graphics_close_system);
    SDL_Log("graphics_initialize_system() graphics initialized");
}

void graphics_close_system()
{
    if(graphics_window)
    {
        SDL_DestroyWindow(graphics_window);
    }
    if(graphics_renderer)
    {
        SDL_DestroyRenderer(graphics_renderer);
    }

    graphics_window = NULL;
    graphics_renderer = NULL;
}

void graphics_frame_delay()
{
    Uint32 diff;
    graphics_then = graphics_now;
    graphics_now = SDL_GetTicks();
    diff = (graphics_now - graphics_then);
    if(diff < graphics_f_delay)
    {
        SDL_Delay(graphics_f_delay - diff);
    }
}

void graphics_next_frame()
{
    SDL_RenderPresent(graphics_renderer);
    graphics_frame_delay();
}