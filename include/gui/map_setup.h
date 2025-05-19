#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "gui/game_menu.h"

#define HIDER_LOSS ((int[3]){-1, -1, -3})
#define HIDER_GAIN ((int[3]){2, 1, 1})


typedef enum
{
    EASY,
    NEUTRAL,
    HARD,
    DIFFICULTY_COUNT,
}
Difficulty;

int** create_game_matrix(int dimension , bool is_2d);


Difficulty generate_difficulty();

void* generate_chests_locations(int chests_count , bool is_2d );

Button* linear_world_chests_generator(int chests_count);
Button** two_d_world_chests_generator(int chests_count);


