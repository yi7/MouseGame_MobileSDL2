#include "map_parser.h"

Map_Detail *map_list = NULL;
const int MAP_MAX = 100;

void map_parser_initialize_system()
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

Map_Detail *map_parser_new()
{
    for(int i = 0; i < MAP_MAX; i++)
    {
        if(map_list[i].initialized)
        {
            continue;
        }

        memset(&map_list[i], 0, sizeof(Map_Detail));
        map_list[i].initialized = true;
        return &map_list[i];
    }

    return NULL;
}

Map_Detail *map_parser_get_map(int map_id)
{
    for(int i = 0; i < MAP_MAX; i++)
    {
        if(map_list[i].initialized &&
           map_list[i].map_id == map_id)
        {
            return &map_list[i];
        }
    }

    return NULL;
}

void map_parser_test()
{
    for(int i = 0; i < MAP_MAX; i++)
    {
        if(map_list[i].initialized)
        {
            /*for(int j = 0; j < (sizeof(map_list[i].map) / sizeof(map_list[i].map[0])); j++)
            {
                SDL_Log("%s", map_list[i].map[j]);
            }*/

            for(int k = 0; k < map_list[i].entity_count; k++)
            {
                SDL_Log("%d, %d, %3d, %2d", map_list[i].entities[k].x, map_list[i].entities[k].y, map_list[i].entities[k].angle, map_list[i].entities[k].type);
            }

            continue;
        }
    }
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

    map_parser_convert_json(file_content);
}

void map_parser_convert_json(char *json)
{
    int count;
    char delim[] = "|";
    char *ptr = strtok(json, delim);
    int map_index = 0;

    while(ptr != NULL)
    {
        //SDL_Log("%s", ptr);
        jsmn_parser jp;
        jsmntok_t tokens[strlen(ptr)];

        jsmn_init(&jp);
        count = jsmn_parse(&jp, ptr, strlen(ptr), tokens, sizeof(tokens) / sizeof(tokens[0]));

        if(count < 0)
        {
            SDL_Log("map_parser_convert_json() failed to parse JSON -Error:");
            return;
        }

        Map_Detail *map = map_parser_new();
        map->map_id = map_index++;

        for(int i = 1; i < count; i++)
        {
            if(map_parser_jsoneq(ptr, &tokens[i], "map"))
            {
                for(int j = 0; j < tokens[i + 1].size; j++)
                {
                    jsmntok_t *temp_tok = &tokens[i + j + 2];
                    char temp_val[3];

                    strncpy(temp_val, ptr + temp_tok->start, temp_tok->end - temp_tok->start);
                    temp_val[2] = '\0';
                    strcpy(map->map[j], temp_val);
                    //SDL_Log("%.*s", tokens[j].end - tokens[j].start, json + tokens[j].start);
                }

                i += tokens[i + 1].size + 1;
            }
            else if(map_parser_jsoneq(ptr, &tokens[i], "name"))
            {
                char temp_val[tokens[i + 1].end - tokens[i + 1].start + 1];
                strncpy(temp_val, ptr + tokens[i + 1].start, tokens[i + 1].end - tokens[i + 1].start);
                temp_val[tokens[i + 1].end - tokens[i + 1].start] = '\0';

                map->map_name = (char *)malloc(tokens[i + 1].end - tokens[i + 1].start + 1);
                strcpy(map->map_name, temp_val);

                i++;
            }
            else if(map_parser_jsoneq(ptr, &tokens[i], "arrow_count"))
            {
                char temp_val[2];
                strncpy(temp_val, ptr + tokens[i + 1].start, tokens[i + 1].end - tokens[i + 1].start);
                temp_val[1] = '\0';
                map->arrow_count = atoi(temp_val);

                i++;
            }
            else if(map_parser_jsoneq(ptr, &tokens[i], "start"))
            {
                int entity_count = tokens[i + 1].size;
                map->entity_count = entity_count;
                for(int j = 0; j < entity_count; j++)
                {
                    //map->entities = (Entity_Placement *)malloc(sizeof(Entity_Placement) * entity_count);
                    jsmntok_t *temp_tok = &tokens[i + j + 2];
                    if(temp_tok->type == JSMN_OBJECT) {
                        i++;
                    }

                    for(int k = 0; k < temp_tok->size; k++)
                    {
                        if(map_parser_jsoneq(ptr, &tokens[i + j + k + 2], "x"))
                        {
                            //SDL_Log("%.*s", tokens[i + j + k + 3].end - tokens[i + j + k + 3].start, ptr + tokens[i + j + k + 3].start);
                            char temp_val[2];
                            strncpy(temp_val, ptr + tokens[i + j + k + 3].start, tokens[i + j + k + 3].end - tokens[i + j + k + 3].start);
                            temp_val[1] = '\0';
                            map->entities[j].x = atoi(temp_val);
                            i++;
                        }
                        else if(map_parser_jsoneq(ptr, &tokens[i + j + k + 2], "y"))
                        {
                            //SDL_Log("%.*s", tokens[i + j + k + 3].end - tokens[i + j + k + 3].start, ptr + tokens[i + j + k + 3].start);
                            char temp_val[2];
                            strncpy(temp_val, ptr + tokens[i + j + k + 3].start, tokens[i + j + k + 3].end - tokens[i + j + k + 3].start);
                            temp_val[1] = '\0';
                            map->entities[j].y = atoi(temp_val);
                            i++;
                        }
                        else if(map_parser_jsoneq(ptr, &tokens[i + j + k + 2], "angle"))
                        {
                            //SDL_Log("%.*s", tokens[i + j + k + 3].end - tokens[i + j + k + 3].start, ptr + tokens[i + j + k + 3].start);
                            char temp_val[tokens[i + j + k + 3].end - tokens[i + j + k + 3].start + 1];
                            strncpy(temp_val, ptr + tokens[i + j + k + 3].start, tokens[i + j + k + 3].end - tokens[i + j + k + 3].start);
                            temp_val[tokens[i + j + k + 3].end - tokens[i + j + k + 3].start] = '\0';
                            map->entities[j].angle = atoi(temp_val);
                            i++;
                        }
                        else if(map_parser_jsoneq(ptr, &tokens[i + j + k + 2], "type"))
                        {
                            //SDL_Log("%.*s", tokens[i + j + k + 3].end - tokens[i + j + k + 3].start, ptr + tokens[i + j + k + 3].start);
                            char temp_val[tokens[i + j + k + 3].end - tokens[i + j + k + 3].start + 1];
                            strncpy(temp_val, ptr + tokens[i + j + k + 3].start, tokens[i + j + k + 3].end - tokens[i + j + k + 3].start);
                            temp_val[tokens[i + j + k + 3].end - tokens[i + j + k + 3].start] = '\0';
                            map->entities[j].type = atoi(temp_val);
                            i++;
                        }
                    }

                    i += temp_tok->size - 1;
                }
            }
        }

        ptr = strtok(NULL, delim);
    }

    //map_parser_test();
}

bool map_parser_jsoneq(char *json, jsmntok_t *token, char *s)
{
    if(token->type == JSMN_STRING &&
       (int)strlen(s) == token->end - token->start &&
       strncmp(json + token->start, s, token->end - token->start) == 0)
    {
        return true;
    }

    return false;
}