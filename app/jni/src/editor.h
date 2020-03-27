#ifndef MOUSEGAME_EDITOR_H
#define MOUSEGAME_EDITOR_H

#include "graphics.h"
#include "sprite.h"
#include "vector.h"
#include "font.h"
#include "file.h"
#include "map.h"
#include "menu.h"

void editor_initialize_button_to_base();

void editor_load_empty_map();

void editor_update_base_window(Window *self, int button_id);

void editor_initialize_side_window();

void editor_update_side_window(Window *self, int button_id);

void editor_initialize_test_window();

void editor_update_test_window(Window *self, int button_id);

void editor_initialize_mouse_select_window();

void editor_update_mouse_select_window(Window *self, int button_id);

void editor_initialize_cat_select_window();

void editor_update_cat_select_window(Window *self, int button_id);

void editor_initialize_tile_select_window();

void editor_update_tile_select_window(Window *self, int button_id);

void editor_initialize_wall_select_window();

void editor_update_wall_select_window(Window *self, int button_id);

#endif //MOUSEGAME_EDITOR_H
