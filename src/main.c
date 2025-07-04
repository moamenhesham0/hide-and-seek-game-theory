#include "gui/game/game_engine.h"
#include "gui/menu/main_menu.h"


SDL_Thread* thread = NULL;

void
run_side_menu()
{
    while(true)

        side_menu_render();
}
int main(int argc, char* argv[])
{
    GameMenu *menu = menu_init("Start Menu", MENU_FRAME_WIDTH, MENU_FRAME_HEIGHT);

    while(menu_run_status()){
        menu_handle_events();
        menu_render();
        SDL_Delay(16);
    }
    bool play_game = menu->play_game;
    int dimension = menu->dimension;
    bool is_2d = menu->is_2d;
    bool is_hider = menu->is_hider;

    menu_destroy();
    if(play_game){
    game_engine_init("Game Engine", MIN_WIDTH, MIN_HEIGHT, dimension, is_2d , is_hider);
    thread = SDL_CreateThread(run_side_menu, "Game Engine Thread", NULL);

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
