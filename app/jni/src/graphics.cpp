#include "graphics.h"

SDL_Window *graphics_window = NULL;
SDL_Renderer *graphics_renderer = NULL;
SDL_Rect graphics_screen = {0, 0, 320, 240};
Graphics_Reference graphics_reference;

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

    //Defining reference throughout the game
    graphics_reference.map_columns = 9; //fixed columns
    graphics_reference.map_rows = 7; //fixed rows
    graphics_reference.screen_width = graphics_screen.w;
    graphics_reference.screen_height = graphics_screen.h;
    graphics_reference.map_height = graphics_reference.screen_height;
    graphics_reference.tile_padding = graphics_reference.screen_height / graphics_reference.map_rows;
    graphics_reference.map_width = graphics_reference.tile_padding * graphics_reference.map_columns;
    graphics_reference.wall_padding = graphics_reference.tile_padding * 4 / 64; //4, 64 is pixel width, height of wall
    graphics_reference.tpl = graphics_reference.map_columns;
    graphics_reference.button_width = graphics_reference.screen_width / 8;
    graphics_reference.button_height = graphics_reference.screen_height / 8;
    graphics_reference.button_padding = graphics_reference.wall_padding;

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