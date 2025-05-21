#include "gui/game/events_manager.h"
#include "gui/game/game_engine.h"
#include "gui/game_flow/map_setup.h"
#include "game_logic/score.h"

static const EventEntry event_table[] =
{
    {SDL_QUIT, quit_event_handler},
    {SDL_MOUSEMOTION , mouse_motion_event_handler},
    {SDL_MOUSEBUTTONDOWN, mouse_button_event_handler},
};

const int event_table_size = sizeof(event_table) / sizeof(EventEntry);


void
dispatch_event(SDL_Event* event)
{
    for (int i = 0; i < event_table_size; ++i)
    {
        if (event->type == event_table[i].event_type)
        {
            event_table[i].event_handler(event);
            break;
        }
    }
}

void
quit_event_handler(SDL_Event * event )
{
     game_engine_set_run_status(RUN_STATUS_QUIT);
}


void mouse_motion_event_handler(SDL_Event* event)
{
    Chest * chests = game_engine_get_chests();
    if(chests == NULL)
        return;
    int dim = game_engine_get_dimension();
    int x = event->motion.x;
    int y = event->motion.y;

    for(int i = 0 ; i < dim ; ++i)
    {
        chests[i].is_hovered = IS_MOUSE_INSIDE(x, y, chests[i].rect);
    }

}



void
mouse_button_event_handler(SDL_Event* event)
{
    Chest * chests = game_engine_get_chests();
    if(chests == NULL)
        return;

    int dim = game_engine_get_dimension();
    int x = event->button.x;
    int y = event->button.y;

    for(int i = 0 ; i < dim ; ++i)
    {
        if(IS_MOUSE_INSIDE(x, y, chests[i].rect))
        {
            chests[i].is_hovered = true;
            play_round(i);
        }
    }
}

void play_human_hider(int chest)
{
    //HIDER TURN
    // animate_hiding(chest);


    // SEEKER TURN
    struct seeker* seeker = game_engine_get_seeker();
    int seeker_choice = choose_chest(game_engine_get_dimension() , seeker->probabilities);
    int **game_matrix = game_engine_get_game_matrix();
    handle_score(seeker_choice , chest , game_matrix);
    // animate_seeker(seeker_choice);

}

void handle_score(int seeker_choice , int hider_chest, int **game_matrix){
        if (game_engine_get_is_2d())
    {
        if (seeker_choice == hider_chest){
                update_score((float)game_matrix[seeker_choice][hider_chest]);
                printf("Seeker caught the hider: %d\n", game_matrix[seeker_choice][hider_chest]);
                // animate_hider_caught();
        }
        else{
            update_score((float)game_matrix[seeker_choice][hider_chest]);
            printf("Seeker uncaught the hider: %d\n", game_matrix[seeker_choice][hider_chest]);
            // animate_hider_uncaught();
        }
    }else{
        if (seeker_choice == hider_chest){
                update_score((float)game_matrix[seeker_choice][hider_chest]);
                printf("Seeker caught the hider: %d\n", game_matrix[seeker_choice][hider_chest]);
                // animate_hider_caught();
        }
        else{
            float penalty = game_matrix[seeker_choice][hider_chest];
            if (abs(seeker_choice - hider_chest) == 1){
                penalty *= 0.5;
                printf("Penalty multiplied by 0.5\n");
            }
            else if (abs(seeker_choice - hider_chest) == 2){
                penalty *= 0.75;
                printf("Penalty multiplied by 0.75\n");
            }
            update_score(penalty);
            printf("Seeker uncaught the hider: %.2f\n", penalty);
            // animate_hider_uncaught();
        }

    
    }

}

void play_human_seeker(int chest)
{
    //HIDER TURN
    struct hider* hider = game_engine_get_hider();
    int hider_choice = choose_chest(game_engine_get_dimension() , hider->probabilities);
    int **game_matrix = game_engine_get_game_matrix();
    // SEEKER TURN
    handle_score(chest , hider_choice , game_matrix);

    // animate_seeker(chest);
}

void play_round(int chest)
{
    bool is_hider = game_engine_get_is_hider();

    if(is_hider)
        play_human_hider(chest);
    else
        play_human_seeker(chest);
}






