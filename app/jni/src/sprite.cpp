#include "sprite.h"

static Sprite *sprite_list = NULL;
const int SPRITE_MAX = 1000;
int sprite_count;

void sprite_initialize_system()
{
    sprite_list = (Sprite *)malloc(sizeof(Sprite) * SPRITE_MAX);
    if(!sprite_list)
    {
        SDL_Log("sprite_initialize_system() failed to allocate sprite system -Error:");
        return;
    }

    memset(sprite_list, 0, sizeof(Sprite) * SPRITE_MAX);
    sprite_count = 0;
    atexit(sprite_close_system);
}

void sprite_close_system()
{
    if(!sprite_list)
    {
        return;
    }

    for(int i = 0; i < SPRITE_MAX; i++)
    {
        if(sprite_list[i].image != 0)
        {
            SDL_DestroyTexture(sprite_list[i].image);
        }
    }

    free(sprite_list);
    sprite_list = NULL;
}

Sprite *sprite_load(char *filename, int frame_width, int frame_height)
{
    void *sprite_pixels = NULL;
    int sprite_pitch = 0;
    int i;
    SDL_Surface *sprite_temp_bmp;
    SDL_Surface *sprite_formatted_surface;
    SDL_Texture *sprite_texture = NULL;

    if(!sprite_list)
    {
        SDL_Log("sprite_load() sprite system uninitialized -Error:");
        return NULL;
    }

    //first search to see if the requested sprite image is already loaded
    for(i = 0; i < SPRITE_MAX; i++)
    {
        if(sprite_list[i].ref == 0)
        {
            continue;
        }

        if(strcmp(filename, sprite_list[i].filename) == 0)
        {
            sprite_list[i].ref++;
            return &sprite_list[i];
        }
    }

    //make sure there's enough room for a new sprite
    if(sprite_count + 1 >= SPRITE_MAX)
    {
        SDL_Log("sprite_load() maximum sprite reached -Error:");
        exit(1);
    }
    sprite_count++;

    for(i = 0; i <= sprite_count; i++)
    {
        if(!sprite_list[i].ref)
        {
            break;
        }
    }

    sprite_temp_bmp = SDL_LoadBMP(filename);
    if(!sprite_temp_bmp)
    {
        SDL_Log("load_sprite() unable to load sprite -Error:");
        exit(0);
    }

    //sprite_formatted_surface = SDL_ConvertSurfaceFormat(sprite_temp_bmp, SDL_PIXELFORMAT_RGBA8888, 0);
    //sprite_texture = SDL_CreateTexture(graphics_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, sprite_formatted_surface->w, sprite_formatted_surface->h);


    sprite_list[i].image = SDL_CreateTextureFromSurface(graphics_renderer, sprite_temp_bmp);
    strcpy(sprite_list[i].filename, filename);
    sprite_list[i].fpl = 16;
    sprite_list[i].frame_size.x = frame_width;
    sprite_list[i].frame_size.y = frame_height;
    sprite_list[i].ref++;

    //SDL_FreeSurface(sprite_formatted_surface);

    return &sprite_list[i];
}

void sprite_draw(Sprite *sprite, int frame, int x, int y)
{
    SDL_Rect src, dest;
    if ((!sprite) || (!graphics_renderer))
    {
        return;
    }

    src.x = frame % sprite->fpl * sprite->frame_size.x;
    src.y = frame / sprite->fpl * sprite->frame_size.y;
    src.w = sprite->frame_size.x;
    src.h = sprite->frame_size.y;

    dest.x = x;
    dest.y = y;
    dest.w = sprite->frame_size.x;
    dest.h = sprite->frame_size.y;

    SDL_RenderCopy(graphics_renderer, sprite->image, &src, &dest);
}
