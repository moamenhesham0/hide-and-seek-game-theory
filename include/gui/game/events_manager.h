#pragma once
#include <SDL2/SDL.h>

#define HIDER 1
#define SEEKER 0
typedef struct EventEntry
{
    uint32_t event_type;
    void (*event_handler)(SDL_Event* event);
}EventEntry;

void dispatch_event(SDL_Event* event);

void mouse_button_event_handler(SDL_Event* event);

void mouse_motion_event_handler(SDL_Event* event);

void quit_event_handler(SDL_Event*  event );

void play_round(int chest);

void handle_score();

void play_human_hider(int chest);
void play_human_seeker(int chest);



