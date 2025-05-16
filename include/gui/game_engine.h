#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define MIN_WIDTH 800
#define MIN_HEIGHT 600


#define RUN_STATUS_RUNNING 1
#define RUN_STATUS_QUIT 0

#define FIRST_GPU -1

#define FAILED_RENDERER_MSG "Failed to create renderer: %s\n"
#define FAILED_WINDOW_MSG "Failed to create window: %s\n"
#define FAILED_INIT_MSG "SDL could not initialize! SDL_Error: %s\n"


#define GAME_ENGINE_ALREADY_RUNNING_MSG "Game engine is already initialized.\n"
#define GAME_ENGINE_DESTROY_MSG "Game engine destroyed.\n"


typedef struct GameEngine
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool run_status;
}GameEngine;


void window_init(const char* title, int width, int height);

void renderer_init();

void game_engine_init(const char* title, int width, int height);

bool game_engine_run_status();

SDL_Window* game_engine_get_window();

SDL_Renderer* game_engine_get_renderer();

void game_engine_set_run_status(bool status);

void game_engine_handle_events();

void game_engine_destroy();




