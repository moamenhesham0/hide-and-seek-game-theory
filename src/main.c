#include "gui/game_engine.h"
#include "gui/main_menu.h"


int main(int argc, char* argv[])
{
    GameMenu *menu = menu_init("Start Menu", MENU_MIN_WIDTH, MENU_MIN_HEIGHT);

    while(menu_run_status()){
        menu_handle_events();
        menu_render();
        SDL_Delay(16);
    }
    bool play_game = menu->play_game;

    menu_destroy();

    if(play_game){
    game_engine_init("Game Engine", MIN_WIDTH, MIN_HEIGHT);

    while (game_engine_run_status())
    {
        game_engine_handle_events();
        game_engine_render();
        SDL_Delay(FRAME_RATE);
    }

    game_engine_destroy();
}

    IMG_Quit();
    SDL_Quit();

    return 0;
}