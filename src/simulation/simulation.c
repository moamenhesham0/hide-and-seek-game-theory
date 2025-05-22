#include "simulation/simulation.h"

// Window and rendering variables
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static TTF_Font* font = NULL;
static char* simulation_output = NULL;
static size_t output_buffer_size = 30000;
static int scroll_offset = 0;
static const int MAX_SCROLL_LINES = 300;

// Initialize the simulation GUI
void simulation_init() {
    // Initialize SDL_ttf
    if (TTF_Init() < 0) {
        fprintf(stderr, "TTF_Init failed: %s\n", TTF_GetError());
        return;
    }

    // Create a window for the simulation
    window = SDL_CreateWindow(
        "Hide and Seek Simulation",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return;
    }

    // Load font
    font = TTF_OpenFont("assets/Times_New_Roman_Bold.ttf", 16);
    if (!font) {
        fprintf(stderr, "TTF_OpenFont failed: %s\n", TTF_GetError());
        return;
    }

    // Initialize output buffer if needed
    if (simulation_output == NULL) {
        output_buffer_size = 30000; // Start with 16KB buffer
        simulation_output = malloc(output_buffer_size);
        if (simulation_output) {
            simulation_output[0] = '\0';
        }
    }
}

// Cleanup resources
void simulation_cleanup() {
    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    
    if (simulation_output) {
        free(simulation_output);
        simulation_output = NULL;
    }
    
    TTF_Quit();
}

// Reset the output buffer
void simulation_reset_output() {
    if (simulation_output) {
        simulation_output[0] = '\0';
    }
    scroll_offset = 0;
}

// Append text to the simulation output
void simulation_append_text(const char* format, ...) {
    if (!simulation_output) return;
    
    va_list args;
    va_start(args, format);
    
    // Calculate remaining buffer size
    size_t current_length = strlen(simulation_output);
    size_t remaining = output_buffer_size - current_length - 1;
    
    // Grow buffer if needed
    if (remaining < 1024) { // Less than 1KB remaining
        output_buffer_size *= 2;
        char* new_buffer = realloc(simulation_output, output_buffer_size);
        if (new_buffer) {
            simulation_output = new_buffer;
            remaining = output_buffer_size - current_length - 1;
        } else {
            va_end(args);
            return;
        }
    }
    
    // Format and append text
    vsnprintf(simulation_output + current_length, remaining, format, args);
    
    va_end(args);
}

// Handle simulation events
void simulation_handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            simulation_cleanup();
            exit(0);
        } else if (event.type == SDL_MOUSEWHEEL) {
            if (event.wheel.y > 0) { // Scroll up
                scroll_offset -= 3;
                if (scroll_offset < 0) scroll_offset = 0;
            } else if (event.wheel.y < 0) { // Scroll down
                scroll_offset += 3;
                
                // Calculate maximum scroll offset based on text content
                int line_count = 0;
                char* temp = simulation_output;
                while (temp && *temp) {
                    if (*temp == '\n') line_count++;
                    temp++;
                }
                
                int max_scroll = line_count > 20 ? line_count - 20 : 0;
                if (scroll_offset > max_scroll) scroll_offset = max_scroll;
            }
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                simulation_cleanup();
                exit(0);
            }
        }
    }
}

// Render a single line of text
void render_text_line(const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if (!surface) return;
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture) {
        SDL_Rect rect = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
    }
    
    SDL_FreeSurface(surface);
}

// Render the simulation output
void simulation_render() {
    if (!renderer || !font || !simulation_output) return;
    
    bool running = true;
    
    while (running) {
        // Handle events
        simulation_handle_events();
        
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Draw border for the text area
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_Rect border = {20, 20, 760, 560};
        SDL_RenderDrawRect(renderer, &border);
        
        // Draw title
        SDL_Color titleColor = {255, 255, 255, 255};
        render_text_line("Hide and Seek Simulation Results", 30, 30, titleColor);
        
        // Draw instruction
        SDL_Color instructionColor = {200, 200, 200, 255};
        render_text_line("Use mouse wheel to scroll, ESC to exit", 30, 560, instructionColor);
        
        // Render text output
        SDL_Color textColor = {255, 255, 255, 255};
        
        int y_pos = 70;
        int line_count = 0;
        char line[256];
        int line_index = 0;
        
        // Skip lines based on scroll offset
        int skipped_lines = 0;
        
        for (const char* p = simulation_output; *p; p++) {
            if (line_index < 255) {
                if (*p == '\n') {
                    line[line_index] = '\0';
                    
                    if (skipped_lines >= scroll_offset) {
                        render_text_line(line, 30, y_pos, textColor);
                        y_pos += 20;
                        line_count++;
                        
                        if (line_count >= 24) break; // Only show N lines at a time
                    }
                    
                    skipped_lines++;
                    line_index = 0;
                } else {
                    line[line_index++] = *p;
                }
            } else {
                line[line_index] = '\0';
                
                if (skipped_lines >= scroll_offset) {
                    render_text_line(line, 30, y_pos, textColor);
                    y_pos += 20;
                    line_count++;
                    
                    if (line_count >= 24) break; // Only show N lines at a time
                }
                
                skipped_lines++;
                line_index = 0;
                line[line_index++] = *p;
            }
        }
        
        // Render the last line if it's not terminated with a newline
        if (line_index > 0) {
            line[line_index] = '\0';
            if (skipped_lines >= scroll_offset && line_count < 24) {
                render_text_line(line, 30, y_pos, textColor);
            }
        }
        
        // Present the rendered content
        SDL_RenderPresent(renderer);
    }
}

// Public function to run the simulation
void run_simulation() {
    simulation_init();
    simulation_render(); // This will enter a loop and handle events
    simulation_cleanup();
}