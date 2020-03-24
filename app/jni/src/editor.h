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

void editor_update_base_window(Window *self, int button_id);

void editor_initialize_side_window();

void editor_update_side_window(Window *self, int button_id);

void editor_load_empty_map();

#endif //MOUSEGAME_EDITOR_H
