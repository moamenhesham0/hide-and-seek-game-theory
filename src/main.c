#include "gui/game/game_engine.h"
#include "gui/menu/main_menu.h"
#include "gui/sidemenu/sidemenu.h"


SDL_Thread* thread = NULL;


int main(int argc, char* argv[])
{

    bool play_game = false;
    bool play_menu = true;
    while(play_game || play_menu)
    {
        GameMenu *menu = menu_init("Start Menu", MENU_FRAME_WIDTH, MENU_FRAME_HEIGHT);
        while(menu_run_status()){
        menu_handle_events();
        menu_render();
        SDL_Delay(16);
    }
        play_game = menu->play_game;
        play_menu = false;
        int dimension = menu->dimension;
        bool is_2d = menu->is_2d;
        bool is_hider = menu->is_hider;

        menu_destroy();


        if(play_game)
        {
            game_engine_init("Game Engine", MIN_WIDTH, MIN_HEIGHT, dimension, is_2d , is_hider);



        while (game_engine_run_status())
        {
            game_engine_handle_events();
            game_engine_render();
            side_menu_render();
            SDL_Delay(FRAME_RATE);
        }
        play_menu = game_engine_get_play_menu();
        game_engine_destroy();
        side_menu_destroy();
        }


    }







    IMG_Quit();
    SDL_Quit();

    return 0;
}
