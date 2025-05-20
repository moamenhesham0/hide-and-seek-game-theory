#include "gui/game/events_manager.h"
#include "gui/game/game_engine.h"
#include "gui/game_flow/map_setup.h"

static const EventEntry event_table[] =
{
    {SDL_QUIT, quit_event_handler},
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



void
mouse_button_event_handler(SDL_Event* event)
{}




