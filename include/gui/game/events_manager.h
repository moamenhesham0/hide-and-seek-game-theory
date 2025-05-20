#pragma once
#include <SDL2/SDL.h>

typedef struct EventEntry
{
    uint32_t event_type;
    void (*event_handler)(SDL_Event* event);
}EventEntry;

void dispatch_event(SDL_Event* event);

void mouse_button_event_handler(SDL_Event* event);

void quit_event_handler();

