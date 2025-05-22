#include "gui/menu/main_menu.h"

static GameMenu* menu = NULL;
SDL_Thread *theme_music_thread = NULL; /*To start theme_music*/

int play_music_thread(void *data) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Mix_OpenAudio Error: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

    Mix_Music *music = Mix_LoadMUS("assets/music/theme.mp3");
    if (!music) {
        fprintf(stderr, "Mix_LoadMUS Error: %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    Mix_VolumeMusic(MIX_MAX_VOLUME / 16);

    Mix_PlayMusic(music, -1);  // Loop infinitely

    while (Mix_PlayingMusic()) {
        SDL_Delay(100);
    }

    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}

void
menu_window_init(const char* title, int width, int height)
{
    SDL_Thread *theme_music_thread = NULL; /*To start theme_music*/
    theme_music_thread = SDL_CreateThread(play_music_thread, "MusicThread", NULL);
    if (!theme_music_thread) {
        fprintf(stderr, "SDL_CreateThread Error: %s\n", SDL_GetError());
    }

    menu->window = SDL_CreateWindow(title,
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    width,
                                    height,
                                    SDL_WINDOW_SHOWN);

    if (menu->window == NULL) {
        SDL_Init(SDL_INIT_VIDEO);
        IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
        fprintf(stderr, FAILED_WINDOW_MSG, SDL_GetError());
        free(menu);
        exit(EXIT_FAILURE);
    }

}


void
menu_renderer_init()
{
    menu->renderer = SDL_CreateRenderer(menu->window, FIRST_GPU, SDL_RENDERER_ACCELERATED);

    if (menu->renderer == NULL) {
        fprintf(stderr, FAILED_RENDERER_MSG, SDL_GetError());
        SDL_DestroyWindow(menu->window);
        free(menu);
        exit(EXIT_FAILURE);
    }
}

struct GameMenu*
menu_init(const char *title, int width, int height)
{
    if (menu != NULL) {
        fprintf(stderr, GAME_MENU_ALREADY_RUNNING_MSG);
        return;
    }

    menu = (GameMenu*) malloc(sizeof(GameMenu));

    menu_window_init(title, width, height);

    menu_renderer_init();





    if (menu->renderer == NULL) {
        fprintf(stderr, FAILED_RENDERER_MSG, SDL_GetError());
        SDL_DestroyWindow(menu->window);
        free(menu);
        exit(EXIT_FAILURE);
    }
    menu->one_dimension = (Button*) malloc(sizeof(Button));
    menu->two_dimension = (Button*) malloc(sizeof(Button));
    menu->exit = (Button*) malloc(sizeof(Button));
    menu->music = (Button*) malloc(sizeof(Button));
    menu->simulation = (Button*) malloc(sizeof(Button));
    menu->music_current_frame = 0;
    menu->start = (Button*) malloc(sizeof(Button));
    menu->hider_seeker = (Button*) malloc(sizeof(Button));
    menu->is_hider = true;
    menu->play_menu = false;
    menu->play_game = false;
    menu->is_2d = false;
    menu->dimension = 0;
    menu->background = NULL;
    menu->current_bg_frame = 0;
    menu->last_bg_frame_time = NULL;

    load_texture(menu);

    menu->is_running = true;

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        fprintf(stderr, "TTF_Init failed: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    menu->font = TTF_OpenFont(FONT_PATH, 24);

    if (!menu->font) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    // Initialize text input related fields
    memset(menu->input_text, 0, MAX_INPUT_LENGTH);
    menu->is_input_active = false;
    menu->input_texture = NULL;
    menu->input_rect = (SDL_Rect){450, 275, 36, 36};

    return menu;
}

void
menu_render(){
    SDL_RenderClear(menu->renderer);
    render_menu_objects(menu);
    SDL_RenderPresent(menu->renderer);
}

void
menu_handle_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        menu_handler(menu, event);
    }
}

bool
menu_run_status(){
    return menu->is_running;
}

void
menu_destroy() {
    if (menu == NULL) {
        printf("Menu is NULL, returning\n");
        return;
    }

    // Free background texture
    if (menu->background) {
        SDL_DestroyTexture(menu->background);
    }

    if(menu->input_texture) {
        SDL_DestroyTexture(menu->input_texture);
    }

    // Free textures
    if (menu->title) {
        SDL_DestroyTexture(menu->title);
    }

    if (menu->one_dimension && menu->one_dimension->texture) {
        SDL_DestroyTexture(menu->one_dimension->texture);
        free(menu->one_dimension);
    }

    if (menu->two_dimension && menu->two_dimension->texture) {
        SDL_DestroyTexture(menu->two_dimension->texture);
        free(menu->two_dimension);
    }

    if (menu->exit && menu->exit->texture) {
        SDL_DestroyTexture(menu->exit->texture);
        free(menu->exit);
    }

    if (menu->hider_seeker && menu->hider_seeker->texture) {
        SDL_DestroyTexture(menu->hider_seeker->texture);
        free(menu->hider_seeker);
    }

    if (menu->music && menu->music->texture) {
        SDL_DestroyTexture(menu->music->texture);
        free(menu->music);
    }

    if (menu->font) {
        TTF_CloseFont(menu->font);
    }

    TTF_Quit();

    SDL_DestroyRenderer(menu->renderer);
    SDL_DestroyWindow(menu->window);
    free(menu);
    menu = NULL;
    printf("Game menu destroyed.\n");
}
