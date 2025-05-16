#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "gui/game_engine.h"
#include "gui/events_manager.h"


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
game_engine_init(const char* title, int width, int height)
{
    if (engine != NULL) {
        fprintf(stderr, GAME_ENGINE_ALREADY_RUNNING_MSG);
        return;
    }

    engine = (GameEngine*) malloc(sizeof(GameEngine));

    window_init(title, width, height);

    renderer_init();

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

/* Sets the run status of the game engine */
void
game_engine_set_run_status(bool status)
{
    engine->run_status = status;
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

    SDL_DestroyRenderer(engine->renderer);
    SDL_DestroyWindow(engine->window);
    free(engine);
    engine = NULL;
    printf(GAME_ENGINE_DESTROY_MSG);
}
