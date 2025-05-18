#include "gui/game_engine.h"


int main(int argc, char* argv[])
{
    game_engine_init("Game Engine", MIN_WIDTH, MIN_HEIGHT);

    while (game_engine_run_status())
    {
        game_engine_handle_events();
        game_engine_render();
        SDL_Delay(FRAME_RATE);
    }

    game_engine_destroy();

    return 0;
}