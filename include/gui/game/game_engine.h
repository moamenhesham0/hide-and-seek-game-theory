#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "game_logic/ai_characters/hider.h"
#include "game_logic/ai_characters/seeker.h"
#include "game_logic/logic.h"

#define MIN_WIDTH 1024
#define MIN_HEIGHT 1024

#define RUN_STATUS_RUNNING 1
#define RUN_STATUS_QUIT 0

#define FIRST_GPU -1

#define FRAME_RATE 62

#define UNINITIALIZED -1

#define FAILED_RENDERER_MSG "Failed to create renderer: %s\n"
#define FAILED_WINDOW_MSG "Failed to create window: %s\n"
#define FAILED_INIT_MSG "SDL could not initialize! SDL_Error: %s\n"


#define GAME_ENGINE_ALREADY_RUNNING_MSG "Game engine is already initialized.\n"
#define GAME_ENGINE_DESTROY_MSG "Game engine destroyed.\n"


typedef enum
{
    _EASY,
    _NEUTRAL,
    _HARD,
    DIFFICULTY_COUNT,
}
Difficulty;
typedef struct
{
    SDL_Rect rect;
    SDL_Texture* texture;
    bool is_hovered;
    Difficulty difficulty;
}
Chest;
typedef struct GameEngine
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool run_status;


    // Textures
    SDL_Texture* map_texture;
    SDL_Texture* hider_texture;
    SDL_Texture* seeker_texture;
    SDL_Texture* easy_chest_texture;
    SDL_Texture* nutural_chest_texture;
    SDL_Texture* hard_chest_texture;

    // Game info
    int dimension;
    bool is_2d;
    bool is_hider;
    int** game_matrix;
    Chest* chests;

    //Players
    struct hider* hider;
    struct seeker* seeker;


    // Animation parameters
    int frame_time;
    int frame_delay;

    // Animation state for each sprite
    int map_current_frame;
    int map_frame_count;

    int hider_current_frame;
    int hider_frame_count;

    int seeker_current_frame;
    int seeker_frame_count;

    int easy_chest_current_frame;
    int easy_chest_frame_count;

    int nutural_chest_current_frame;
    int nutural_chest_frame_count;

    int hard_chest_current_frame;
    int hard_chest_frame_count;


    // Last time we updated animation
    Uint32 last_update_time;
}GameEngine;


void window_init(const char* title, int width, int height);

void renderer_init();

void game_engine_init(const char* title, int width, int height , int dimension , bool is_2d , bool is_hider);

bool game_engine_run_status();

int game_engine_get_dimension();

bool game_engine_get_is_2d();

bool game_engine_get_is_hider();

int** game_engine_get_game_matrix();

Chest* game_engine_get_chests();

void game_engine_set_chests(Chest* chests);
void game_engine_set_game_matrix(int** matrix);
void game_engine_set_dimension(int dimension);
void game_engine_set_is_2d(bool is_2d);
void game_engine_set_is_hider(bool is_hider);

SDL_Window* game_engine_get_window();

SDL_Renderer* game_engine_get_renderer();

SDL_Texture* game_engine_get_map_texture();

SDL_Texture* game_engine_get_hider_texture();

SDL_Texture* game_engine_get_seeker_texture();

SDL_Texture* game_engine_get_easy_chest_texture();

SDL_Texture* game_engine_get_nutural_chest_texture();

SDL_Texture* game_engine_get_hard_chest_texture();

void game_engine_set_run_status(bool status);

void game_engine_render();

void game_engine_handle_events();

void game_engine_destroy();







