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

    struct seeker* seeker = game_engine_get_seeker();
    int seeker_choice = choose_chest(game_engine_get_dimension() , seeker->probabilities);

    if(seeker_choice == chest)
    {
        printf("Seeker found your ass %d\n" , seeker_choice);
    }
    else
    {
        printf("Seeker is blind %d\n" , seeker_choice);
    }
}

void play_human_seeker(int chest)
{
    struct hider* hider = game_engine_get_hider();

    int hider_choice = choose_chest(game_engine_get_dimension() , hider->probabilities);

    if(hider_choice == chest)
    {
        printf("I found your ass %d\n", hider_choice);
    }
    else
    {
        printf("I didnt find your ass %d\n", hider_choice);
    }
}

void play_round(int chest)
{
    bool is_hider = game_engine_get_is_hider();

    if(is_hider)
        play_human_hider(chest);
    else
        play_human_seeker(chest);
}






