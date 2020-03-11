#include "map_parser.h"

Map_Detail *map_list = NULL;
const int MAP_MAX = 100;

void parser_initialize_system()
{
    map_list = (Map_Detail *)malloc(sizeof(Map_Detail) * MAP_MAX);
    if(!map_list)
    {
        //SDL_Log("parser_initialize_system() failed to allocate map list -Error:");
        return;
    }

    memset(map_list, 0, sizeof(Map_Detail) * MAP_MAX);
    atexit(map_parser_close_system);
}

void map_parser_close_system()
{
    free(map_list);
}

void map_parser_parse_file(char *filename)
{
    SDL_RWops *file = SDL_RWFromFile(filename, "r");
    if(file == NULL)
    {
        return;
    }

    Sint64 file_size = SDL_RWsize(file);
    char *file_content = (char *)malloc(file_size + 1);

    Sint64 read_total = 0;
    Sint64 read = 1;
    char *temp_content = file_content;
    while(read_total < file_size && read != 0)
    {
        read = SDL_RWread(file, temp_content, 1, (file_size - read_total));
        read_total += read;
        temp_content += read;
    }

    SDL_RWclose(file);

    if(read_total != file_size)
    {
        free(file_content);
        return;
    }

    file_content[read_total] = '\0';

    SDL_Log("%s", file_content);
}
