#ifndef MOUSEGAME_EDITOR_H
#define MOUSEGAME_EDITOR_H

#include "graphics.h"
#include "sprite.h"
#include "vector.h"
#include "font.h"
#include "file.h"
#include "map.h"
#include "menu.h"

void editor_initialize_button_link();

void editor_update_base_window(Window *self, int button_id);

#endif //MOUSEGAME_EDITOR_H
