#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "macros.h"


#define FIRST_GPU -1

#define MAX_INPUT_LENGTH 5

#define MENU_MIN_WIDTH 800
#define MENU_MIN_HEIGHT 600

#define GAME_MENU_ALREADY_RUNNING_MSG "Game menu is already initialized.\n"
#define FAILED_RENDERER_MSG "Failed to create renderer: %s\n"
#define FAILED_WINDOW_MSG "Failed to create window: %s\n"
#define FAILED_INIT_MSG "SDL could not initialize! SDL_Error: %s\n"

#define FONT_PATH "./assets/Times_New_Roman_Bold.ttf"

#define MUTE_MUSIC Mix_VolumeMusic(0)
#define UNMUTE_MUSIC Mix_VolumeMusic(MIX_MAX_VOLUME)
#define IS_MUSIC_MUTE Mix_VolumeMusic(-1) == 0

SDL_Thread *theme_music_thread; /*To start theme_music*/


typedef struct {
    SDL_Rect rect;
    SDL_Texture *texture;
    bool is_hovered;
} Button;

typedef struct GameMenu {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *title;
    Button *one_dimension;
    Button *two_dimension;
    Button *exit;
    Button *music;
    Button *start;
    bool is_running;
    bool play_menu;
    bool play_game;
    char input_text[MAX_INPUT_LENGTH];
    bool is_input_active;
    TTF_Font *font;
    SDL_Texture *input_texture;
    SDL_Rect input_rect;


    // Game info
    int dimension;
    bool is_2d;


} GameMenu;

GameMenu* menu_init(const char *title, int width, int height);

void menu_window_init(const char* title, int width, int height);

void menu_handle_events();

void menu_renderer_init();

void menu_render();

bool menu_run_status();

void menu_destroy();