#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define FIRST_GPU -1

#define MAX_INPUT_LENGTH 10

#define MENU_MIN_WIDTH 800
#define MENU_MIN_HEIGHT 600

#define GAME_MENU_ALREADY_RUNNING_MSG "Game menu is already initialized.\n"
#define FAILED_RENDERER_MSG "Failed to create renderer: %s\n"
#define FAILED_WINDOW_MSG "Failed to create window: %s\n"
#define FAILED_INIT_MSG "SDL could not initialize! SDL_Error: %s\n"

typedef struct {
    SDL_Rect rect;
    SDL_Texture *texture;
    bool is_hovered;
} Button;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *title;
    Button *one_dimension;
    Button *two_dimension;
    Button *exit;
    Button *music;
    Button *start;
    bool is_running;
} GameMenu;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool is_one_dimension;
    Button *title;
    Button *start;
    Button *exit;
    Button *music;
} PlayMenu;

void menu_init(const char *title, int width, int height);

void menu_window_init(const char* title, int width, int height);

void load_texture();

void load_button();

void menu_handle_events();

void menu_renderer_init();

void menu_render();

void render_menu_objects();

bool menu_run_status();

void menu_destroy();