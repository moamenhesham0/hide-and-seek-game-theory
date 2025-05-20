#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "gui/menu/main_menu.h"
#include "gui/game/game_engine.h"
#include "gui/game/game_engine.h"

#define HIDER_LOSS ((int[3]){-1, -1, -3})
#define HIDER_GAIN ((int[3]){2, 1, 1})



Difficulty generate_difficulty();

SDL_Texture* get_chest_texure(Difficulty difficulty);

bool invalid_pos(Chest* chests , int index);

void set_box_rect(SDL_Rect* rect, int index);

void init_game_matrix();

void init_chests();


