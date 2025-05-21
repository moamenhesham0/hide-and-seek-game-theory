#include "gui/game/events_manager.h"
#include "gui/game/game_engine.h"

#include "gui/game_flow/map_setup.h"

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

    // animate_seeker(seeker_choice);

    if (game_engine_get_is_2d())
    {
        if (seeker_choice == chest){
                // seeker.score += SEEKER_GAIN[chest];
                // hider.score -= SEEKER_GAIN[chest];
                // animate_hider_caught();
        }
        else{
            // seeker.score += HIDER_GAIN[chest];   //HIDER_GAIN[chest] is negative so keep the sign as it is
            // hider.score -= HIDER_GAIN[chest];    //HIDER_GAIN[chest] is negative so keep the sign as it is
            // animate_hider_uncaught();
        }
    }else{
        if (seeker_choice == chest){
                // seeker.score += SEEKER_GAIN[chest];
                // hider.score -= SEEKER_GAIN[chest];
                // animate_hider_caught();
        }
        else{
            // double penalty = HIDER_GAIN[chest];
            // if (abs(seeker_choice - chest) == 1)
                // penalty *= 0.5;
                //printf("\nPenalty multiplied by 0.5\n");
            // else if (abs(seeker_choice - chest) == 2)
                // penalty *= 0.75;
                //printf("\nPenalty multiplied by 0.75\n");

            // seeker.score += penalty; //HIDER_GAIN[chest] is negative so keep the sign as it is
            // hider.score -= penalty;  //HIDER_GAIN[chest] is negative so keep the sign as it is
            // animate_hider_uncaught();
        }

    
    }
}

void play_human_seeker(int chest)
{
    //HIDER TURN
    struct hider* hider = game_engine_get_hider();
    int hider_choice = choose_chest(game_engine_get_dimension() , hider->probabilities);

    // SEEKER TURN

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






