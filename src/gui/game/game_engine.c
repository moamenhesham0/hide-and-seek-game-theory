#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "gui/game/game_engine.h"
#include "gui/game/events_manager.h"
#include "gui/game/textures_manager.h"
#include "gui/game_flow/map_setup.h"
#include "game_logic/score.h"
#include "macros.h"


static GameEngine* engine = NULL;



/* Initializes SDL main window*/
void
window_init(const char* title, int width, int height)
{
    engine->window = SDL_CreateWindow(  title,
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        width,
                                        height,
                                        SDL_WINDOW_SHOWN
                                    );


    if (engine->window == NULL)
    {
        // SDL_Init(SDL_INIT_VIDEO);
        // IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
        fprintf(stderr, FAILED_WINDOW_MSG, SDL_GetError());
        free(engine);
        exit(EXIT_FAILURE);
    }
}

/* Creates Renderer*/
void
renderer_init()
{
    engine->renderer = SDL_CreateRenderer(engine->window, FIRST_GPU, SDL_RENDERER_ACCELERATED);

    if (engine->renderer == NULL)
    {
        fprintf(stderr, FAILED_RENDERER_MSG, SDL_GetError());
        SDL_DestroyWindow(engine->window);
        free(engine);
        exit(EXIT_FAILURE);
    }
}

/* Initializes the game engine */
void
game_engine_init(const char* title, int width, int height, int dimension, bool is_2d , bool is_hider)
{
    if (engine != NULL) {
        fprintf(stderr, GAME_ENGINE_ALREADY_RUNNING_MSG);
        return;
    }

    engine = (GameEngine*) malloc(sizeof(GameEngine));

    window_init(title, width, height);

    renderer_init();

    PNG_load(engine);

    engine->dimension = dimension;
    engine->is_2d = is_2d;
    engine->is_hider = is_hider;
    engine->game_matrix = NULL;
    engine->chests = NULL;

    init_game_matrix(); // Initialize the game matrix

    init_score();   // Initialize the score structure

    

    engine->seeker =  initialize_seeker(dimension , engine->game_matrix);
    engine->hider = initialize_hider(dimension , engine->game_matrix);

    find_hider_strategy(engine->hider , dimension);
    find_seeker_strategy(engine->seeker , dimension);

    check_valid_probablities();


    engine->frame_delay = FRAME_DELAY;
    engine->map_frame_count = MAP_FRAME_COUNT;
    engine->map_current_frame = 0;
    engine->hider_frame_count = CHARACTER_FRAME_COUNT;
    engine->hider_current_frame = 0;
    engine->seeker_frame_count = CHARACTER_FRAME_COUNT;
    engine->seeker_current_frame = 0;
    engine->easy_chest_frame_count = CHEST_FRAME_COUNT;
    engine->easy_chest_current_frame = 0;
    engine->nutural_chest_frame_count = CHEST_FRAME_COUNT;
    engine->nutural_chest_current_frame = 0;
    engine->hard_chest_frame_count = CHEST_FRAME_COUNT;
    engine->hard_chest_current_frame = 0;
    engine->last_update_time = SDL_GetTicks();

    engine->run_status = RUN_STATUS_RUNNING;
}

/* Returns the run status of the game engine */
bool
game_engine_run_status()
{
    return engine->run_status;
}

/* Returns the window of the game engine */
SDL_Window*
game_engine_get_window()
{
    return engine->window;
}

/* Returns the renderer of the game engine */
SDL_Renderer*
game_engine_get_renderer()
{
    return engine->renderer;
}

/* Returns the map texture of the game engine */
SDL_Texture*
game_engine_get_map_texture()
{
    return engine->map_texture;
}

/* Returns the hider texture of the game engine */
SDL_Texture*
game_engine_get_hider_texture()
{
    return engine->hider_texture;
}

/* Returns the seeker texture of the game engine */
SDL_Texture*
game_engine_get_seeker_texture()
{
    return engine->seeker_texture;
}

/* Returns easy chest texuture of the game engine */
SDL_Texture*
game_engine_get_easy_chest_texture()
{
    return engine->easy_chest_texture;
}

/* Returns nutural chest texture of the game engine */
SDL_Texture*
game_engine_get_nutural_chest_texture()
{
    return engine->nutural_chest_texture;
}

/* Returns hard chest texture of the game engine */
SDL_Texture*
game_engine_get_hard_chest_texture()
{
    return engine->hard_chest_texture;
}

/* Sets the run status of the game engine */
void
game_engine_set_run_status(bool status)
{
    engine->run_status = status;
}

/* Returns the dimension of the game engine */
int
game_engine_get_dimension()
{
    return engine->dimension;
}

/* Returns the is_2d status of the game engine */
bool
game_engine_get_is_2d()
{
    return engine->is_2d;
}

/* Returns the is_hider status of the game engine */
bool
game_engine_get_is_hider()
{
    return engine->is_hider;
}

/* Returns the game matrix of the game engine */
int**
game_engine_get_game_matrix()
{
    return engine->game_matrix;
}

/* Returns the difficulty frequency of the game engine */
Chest*
game_engine_get_chests()
{
    return &(engine->chests[0]);
}

struct hider*
game_engine_get_hider()
{
    return engine->hider;
}

struct seeker*
game_engine_get_seeker()
{
    return engine->seeker;
}

void
game_engine_set_hider(struct hider* hider)
{
    engine->hider = hider;
}

void
game_engine_set_seeker(struct seeker* seeker)
{
    engine->seeker = seeker;
}

void
game_engine_set_chests(Chest* chests)
{
    engine->chests = chests;
}

void
game_engine_set_game_matrix(int** matrix)
{
    engine->game_matrix = matrix;
}

void
game_engine_set_dimension(int dimension)
{
    engine->dimension = dimension;
}

void
game_engine_set_is_2d(bool is_2d)
{
    engine->is_2d = is_2d;
}

void
game_engine_set_is_hider(bool is_hider)
{
    engine->is_hider = is_hider;
}


void
game_engine_render()
{
    // Update animation frames
    game_engine_update_animations(engine);

    // Clear the screen with a black color
    SDL_SetRenderDrawColor(engine->renderer, 0, 0, 0, 255);
    SDL_RenderClear(engine->renderer);

    render_game_objects(engine);

    SDL_RenderPresent(engine->renderer);
}

/* Handles events for the game engine */
/* This function checks for events like window close, key press, etc. */
void
game_engine_handle_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        dispatch_event(&event);
    }
}

/* Destroys the game engine */
void
game_engine_destroy()
{
    if (engine == NULL)
    {
        return;
    }

    destroy_texture(engine);

    SDL_DestroyRenderer(engine->renderer);
    SDL_DestroyWindow(engine->window);

    FREE_MATRIX(engine->game_matrix, engine->dimension);
    free(engine->chests);

    free(engine);
    engine = NULL;
    printf(GAME_ENGINE_DESTROY_MSG);
}
