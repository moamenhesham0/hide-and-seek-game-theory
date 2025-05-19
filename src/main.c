#include "gui/game_engine.h"
#include "gui/game_menu.h"


int main(int argc, char* argv[])
{
    menu_init("Start Menu", MENU_MIN_WIDTH, MENU_MIN_HEIGHT);

    while(menu_run_status()){
        menu_handle_events();
        menu_render();
        SDL_Delay(16);
    }

    menu_destroy();

    game_engine_init("Game Engine", MIN_WIDTH, MIN_HEIGHT);

    while (game_engine_run_status())
    {
        game_engine_handle_events();
        game_engine_render();
        SDL_Delay(FRAME_RATE);
    }

    game_engine_destroy();

    IMG_Quit();
    SDL_Quit();

    return 0;
}