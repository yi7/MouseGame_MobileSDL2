#ifndef MOUSEGAME_FILE_H
#define MOUSEGAME_FILE_H

#include <string.h>
#include <SDL.h>

#define JSMN_HEADER
#include "jsmn.h"

typedef struct
{
    int x; /**<x position of entity loaded from file*/
    int y; /**<y position of entity loaded from file*/
    int angle; /**<angle of entity loaded from file*/
    int type; /**<type of entity loaded from file: 0-9 mouse, 10-19 cat*/
} Entity_Placement;

typedef struct
{
    char map[285][3]; /**<the map loaded from file*/
    Entity_Placement entities[63]; /**<list of entities associated with map*/
    int entity_count; /**<total of entity associated with map*/
    int arrow_count; /**<max number of arrow tiles for associated map*/
    char *map_name; /**<name of map*/
    int map_id; /**<id of map*/
    bool initialized; /**<flag to determine if initialized or not*/
} Map_Detail;

/**
 * @brief initializes the file system
 */
void file_initialize_system();

/**
 * @brief closes the file system
 */
void file_close_system();

/**
 * @brief frees maps from map list
 */
void file_free_all();

/**
 * loads empty map to list
 * @return pointer to loaded map
 */
Map_Detail *file_new();

void file_load_editor();

/**
 * @brief gets the map from the list
 * @param map_id id of map to get
 * @return pointer to requested map, NULL otherwise
 */
Map_Detail *file_get_map(int map_id);

/**
 * @brief reads the file and extracts json information
 * @param filename file to read
 */
void file_parse(const char *filename);

/**
 * @brief parses the extracted json
 * @param json json to parse
 */
void file_convert_json(char *json);

/**
 * @brief jsmn function to check if token key equals passed string
 * @param json json to check
 * @param token token in json to check
 * @param s string to compare with token
 * @return true if it matches, false otherwise
 */
bool file_jsoneq(char *json, jsmntok_t *token, char *s);

#endif //MOUSEGAME_FILE_H
