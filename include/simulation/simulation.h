#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Initialize the simulation GUI
void simulation_init();

// Clean up simulation resources
void simulation_cleanup();

// Reset the output buffer
void simulation_reset_output();

// Append text to the simulation output
void simulation_append_text(const char* format, ...);

// Handle simulation events
void simulation_handle_events();

// Render the simulation output
void simulation_render();

// Run the simulation
void run_simulation();