#include "font.h"

static Message *message_list = NULL;
const int MESSAGE_MAX = 1000;
int message_count;

TTF_Font *small_font = NULL;
TTF_Font *medium_font = NULL;
TTF_Font *large_font = NULL;

void font_initialize_system()
{
    if(TTF_Init() == 0)
    {
        atexit(TTF_Quit);
    }
    else
    {
        SDL_Log("font_initialize_system() failed to initialize font system -Error:");
    }

    small_font = TTF_OpenFont("fonts/zerotwos.ttf", graphics_reference.screen_height * 0.025f);
    if(!small_font)
    {
        SDL_Log("font_initialize_system() failed to initialize small font -Error:");
    }

    medium_font = TTF_OpenFont("fonts/zerotwos.ttf", graphics_reference.screen_height * 0.04f);
    if(!medium_font)
    {
        SDL_Log("font_initialize_system() failed to initialize medium font -Error:");
    }

    large_font = TTF_OpenFont("fonts/zerotwos.ttf", graphics_reference.screen_height * 0.055f);
    if(!large_font)
    {
        SDL_Log("font_initialize_system() failed to initialize large font -Error:");
    }

    message_list = (Message *)malloc(sizeof(Message) * MESSAGE_MAX);
    if(!message_list)
    {
        SDL_Log("font_initialize_system() failed to initialize font system -Error:");
        return;
    }

    memset(message_list, 0, sizeof(Message) * MESSAGE_MAX);
    message_count = 0;
    atexit(font_close_system);
}

void font_close_system()
{
    if(!message_list)
    {
        return;
    }

    for(int i = 0; i < MESSAGE_MAX; i++)
    {
        if(message_list[i].message_texture != 0)
        {
            SDL_DestroyTexture(message_list[i].message_texture);
        }
    }

    free(message_list);
    message_list = NULL;
}

Message *font_load_message(const char *message, Uint8 r, Uint8 g, Uint8 b, int size)
{
    int i;
    SDL_Surface *temp_surface = NULL;
    SDL_Color text_color = {r, g, b};

    if(!message_list)
    {
        SDL_Log("font_load_message() font system uninitialized -Error:");
        return NULL;
    }

    //First search to see if the requested sprite image is already loaded
    for(i = 0; i < MESSAGE_MAX; i++)
    {
        if(message_list[i].ref == 0)
        {
            continue;
        }

        if(strcmp(message, message_list[i].message) == 0)
        {
            message_list[i].ref++;
            return &message_list[i];
        }
    }

    //Make sure there's enough room for a new font message
    if(message_count + 1 >= MESSAGE_MAX)
    {
        SDL_Log("font_load_message() maximum message reached -Error:");
        exit(1);
    }
    message_count++;

    for(i = 0; i <= message_count; i++)
    {
        if(!message_list[i].ref)
        {
            break;
        }
    }

    switch(size)
    {
        case 0:
            temp_surface = TTF_RenderText_Solid(small_font, message, text_color);
            break;
        case 1:
            temp_surface = TTF_RenderText_Solid(medium_font, message, text_color);
            break;
        case 2:
            temp_surface = TTF_RenderText_Solid(large_font, message, text_color);
            break;
        default:
            return NULL;
    }

    if(!temp_surface)
    {
        SDL_Log("font_draw_text() failed to render text surface -Error:");
    }

    strcpy(message_list[i].message, message);
    message_list[i].ref++;
    message_list[i].message_texture = SDL_CreateTextureFromSurface(graphics_renderer, temp_surface);
    message_list[i].color = text_color;
    message_list[i].width = temp_surface->w;
    message_list[i].height = temp_surface->h;

    return &message_list[i];
}

void font_draw_text(Message *message, int x, int y, int padding)
{
    SDL_Rect dest;
    dest.x = x + padding;
    dest.y = y + padding;
    dest.w = message->width;
    dest.h = message->height;

    SDL_RenderCopyEx(graphics_renderer, message->message_texture, NULL, &dest, 0, NULL, SDL_FLIP_NONE);
}
