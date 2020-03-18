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
        if(sprite_list[i].image)
        {
            SDL_DestroyTexture(sprite_list[i].image);
        }
    }

    free(sprite_list);
    sprite_list = NULL;
}

Sprite *sprite_load(const  char *filename, int frame_width, int frame_height, int fpl)
{
    int i;
    SDL_Surface *sprite_temp;

    if(!sprite_list)
    {
        SDL_Log("sprite_load() sprite system uninitialized -Error:");
        return NULL;
    }

    //First search to see if the requested sprite image is already loaded
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

    //Make sure there's enough room for a new sprite
    if(sprite_count + 1 >= SPRITE_MAX)
    {
        SDL_Log("sprite_load() maximum sprite reached -Error:");
        exit(1);
    }
    sprite_count++;

    //Loop through list for available index
    for(i = 0; i <= sprite_count; i++)
    {
        if(!sprite_list[i].ref)
        {
            break;
        }
    }

    //Load sprite surface
    sprite_temp = IMG_Load(filename);
    if(!sprite_temp)
    {
        SDL_Log("load_sprite() unable to load sprite -Error:");
        exit(0);
    }

    sprite_list[i].image = SDL_CreateTextureFromSurface(graphics_renderer, sprite_temp);
    strcpy(sprite_list[i].filename, filename);
    sprite_list[i].fpl = fpl;
    sprite_list[i].frame_size.w = frame_width;
    sprite_list[i].frame_size.h = frame_height;
    sprite_list[i].ref++;

    SDL_FreeSurface(sprite_temp);

    return &sprite_list[i];
}

void sprite_free(Sprite **sprite)
{
    Sprite *self;

    if(!sprite)
    {
        return;
    }

    if(!*sprite)
    {
        return;
    }

    self = *sprite;
    self->ref--;

    if(self->ref <= 0)
    {
        SDL_DestroyTexture(self->image);
        memset(self, 0, sizeof(Sprite));
    }

    *sprite = NULL;
}

void sprite_draw(Sprite *sprite, int frame, int x, int y, int width, int height, int angle, SDL_RendererFlip flip)
{
    SDL_Rect src, dest;
    if ((!sprite) || (!graphics_renderer))
    {
        return;
    }

    src.x = frame % sprite->fpl * sprite->frame_size.w;
    src.y = frame / sprite->fpl * sprite->frame_size.h;
    src.w = sprite->frame_size.w;
    src.h = sprite->frame_size.h;

    dest.x = x;
    dest.y = y;
    dest.w = width;
    dest.h = height;

    SDL_RenderCopyEx(graphics_renderer, sprite->image, &src, &dest, angle, NULL, flip);
}
