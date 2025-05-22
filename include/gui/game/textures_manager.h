#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game_engine.h"


#define LOAD_MAP_FAILED_MSG "Failed to load map: %s\n"
#define CREATE_MAP_TEXTURE_FAILED_MSG "Failed to create map texture: %s\n"

#define LOAD_HIDER_FAILED_MSG "Failed to load hider: %s\n"
#define CREATE_HIDER_TEXTURE_FAILED_MSG "Failed to create hider texture: %s\n"

#define LOAD_SEEKER_FAILED_MSG "Failed to load seeker: %s\n"
#define CREATE_SEEKER_TEXTURE_FAILED_MSG "Failed to create seeker texture: %s\n"

#define LOAD_EASY_CHEST_FAILED_MSG "Failed to load easy chest: %s\n"
#define CREATE_EASY_CHEST_TEXTURE_FAILED_MSG "Failed to create easy chest texture: %s\n"

#define LOAD_NEUTRAL_CHEST_FAILED_MSG "Failed to load nutural chest: %s\n"
#define CREATE_NEUTRAL_CHEST_TEXTURE_FAILED_MSG "Failed to create nutural chest texture: %s\n"

#define LOAD_HARD_CHEST_FAILED_MSG "Failed to load hard chest: %s\n"
#define CREATE_HARD_CHEST_TEXTURE_FAILED_MSG "Failed to create hard chest texture: %s\n"

#define FRAME_DELAY 250

#define MAP_FRAME_WIDTH 1024
#define MAP_FRAME_HEIGHT 1024
#define MAP_FRAME_COUNT 2

#define CHEST_FRAME_WIDTH 796
#define CHEST_FRAME_HEIGHT 592
#define CHEST_FRAME_COUNT 3

#define CHARACTER_FRAME_WIDTH 384
#define CHARACTER_FRAME_HEIGHT 1024
#define CHARACTER_FRAME_COUNT 4

#define HIDE_DELAY 500
#define BOX_CLOSE_DELAY 500

#define MOVE_SPEED 17
#define RUNNING_SPEED 25
#define THRESHOLD_DIST 384

#define CHEST_SPRITE_SHEET_WIDTH 1536
#define CHEST_SPRITE_SHEET_HEIGHT 5120

#define CHEST_FRAME_X_OFFSET 360
#define CHEST_FRAME_Y_OFFSET 131


void
PNG_load(GameEngine* engine);

void
game_engine_update_animations(GameEngine* engine);

void
render_game_objects(GameEngine* engine);


void
render_hover(Chest* chest);

void
animate_movement(SDL_Rect* src , SDL_Rect* dst , int* curr_dir , bool is_hider);

void
render_side_menu(GameEngine * engine );

void
destroy_texture(GameEngine* engine);











