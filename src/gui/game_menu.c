#include "gui/game_menu.h"
#include "macros.h"

static GameMenu* menu = NULL;
static bool play_menu = false;

void
menu_window_init(const char* title, int width, int height)
{
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

void
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
    menu->start = (Button*) malloc(sizeof(Button));

    load_texture();

    menu->is_running = true;
}

void
load_texture(){
    menu->title = IMG_LoadTexture(menu->renderer, "assets/menu/title.png");
    if (menu->title == NULL) {
        fprintf(stderr, "Failed to load title texture: %s\n", IMG_GetError());
        return;
    }
    menu->one_dimension->texture = IMG_LoadTexture(menu->renderer, "assets/menu/oneD.png");
    if (menu->one_dimension->texture == NULL) {
        fprintf(stderr, "Failed to load one dimension texture: %s\n", IMG_GetError());
        return;
    }
    menu->two_dimension->texture = IMG_LoadTexture(menu->renderer, "assets/menu/twoD.png");
    if (menu->two_dimension->texture == NULL) {
        fprintf(stderr, "Failed to load two dimension texture: %s\n", IMG_GetError());
        return;
    }
    menu->exit->texture = IMG_LoadTexture(menu->renderer, "assets/menu/exit.png");
    if (menu->exit->texture == NULL) {
        fprintf(stderr, "Failed to load exit texture: %s\n", IMG_GetError());
        return;
    }
    menu->music->texture = IMG_LoadTexture(menu->renderer, "assets/menu/music_note.png");
    if (menu->music->texture == NULL) {
        fprintf(stderr, "Failed to load music texture: %s\n", IMG_GetError());
        return;
    }
    menu->start->texture = IMG_LoadTexture(menu->renderer, "assets/menu/start.png");
    if (menu->start->texture == NULL) {
        fprintf(stderr, "Failed to load start texture: %s\n", IMG_GetError());
        return;
    }
    load_button();
}

void
load_button(){
    // One dimension button
    menu->one_dimension->is_hovered = false;
    menu->one_dimension->rect = (SDL_Rect){296, 275, 200, 36};
    
    // Two dimension button
    menu->two_dimension->is_hovered = false;
    menu->two_dimension->rect = (SDL_Rect){296, 375, 200, 36};

    // Exit button - moved to bottom left
    menu->exit->is_hovered = false;
    menu->exit->rect = (SDL_Rect){25, 550, 80, 30};

    // Music button - moved to bottom right
    menu->music->is_hovered = false;
    menu->music->rect = (SDL_Rect){750, 550, 35, 35};

    // Start button - moved to center
    menu->start->is_hovered = false;
    menu->start->rect = (SDL_Rect){400, 450, 105, 30};
}

void
menu_render(){
    SDL_SetRenderDrawColor(menu->renderer, 128, 128, 128, 255);
    SDL_RenderClear(menu->renderer);

    render_menu_objects();
    SDL_RenderPresent(menu->renderer);
}

void
render_menu_objects(){
    // Render title
    if(menu->title){
        SDL_Rect title_rect = {200, 100, 400, 100};
        SDL_RenderCopy(menu->renderer, menu->title, NULL, &title_rect);
    }
    
    // Render one dimension button
    if(menu->one_dimension && menu->one_dimension->texture){
        SDL_SetRenderDrawColor(menu->renderer, 
                              menu->one_dimension->is_hovered ? 150 : 128,  // Gray values 
                              menu->one_dimension->is_hovered ? 150 : 128,  // Gray values
                              menu->one_dimension->is_hovered ? 150 : 128,  // Gray values
                              255);
        SDL_RenderFillRect(menu->renderer, &menu->one_dimension->rect);
        SDL_RenderCopy(menu->renderer, menu->one_dimension->texture, NULL, &menu->one_dimension->rect);
    }
    
    // Render two dimension button
    if(menu->two_dimension && menu->two_dimension->texture){
        SDL_SetRenderDrawColor(menu->renderer, 
                              menu->two_dimension->is_hovered ? 150 : 128,  // Gray values
                              menu->two_dimension->is_hovered ? 150 : 128,  // Gray values
                              menu->two_dimension->is_hovered ? 150 : 128,  // Gray values
                              255);
        SDL_RenderFillRect(menu->renderer, &menu->two_dimension->rect);
        SDL_RenderCopy(menu->renderer, menu->two_dimension->texture, NULL, &menu->two_dimension->rect);
    }
    
    // Render exit button
    if(menu->exit && menu->exit->texture){
        SDL_SetRenderDrawColor(menu->renderer, 
                              menu->exit->is_hovered ? 150 : 128,  // Gray values
                              menu->exit->is_hovered ? 150 : 128,  // Gray values
                              menu->exit->is_hovered ? 150 : 128,  // Gray values
                              255);
        SDL_RenderFillRect(menu->renderer, &menu->exit->rect);
        SDL_RenderCopy(menu->renderer, menu->exit->texture, NULL, &menu->exit->rect);
    }
    
    // Render music button
    if(menu->music && menu->music->texture){
        SDL_SetRenderDrawColor(menu->renderer, 
                              menu->music->is_hovered ? 150 : 128,  // Gray values
                              menu->music->is_hovered ? 150 : 128,  // Gray values
                              menu->music->is_hovered ? 150 : 128,  // Gray values
                              255);
        SDL_RenderFillRect(menu->renderer, &menu->music->rect);
        SDL_RenderCopy(menu->renderer, menu->music->texture, NULL, &menu->music->rect);
    }

    if(play_menu && menu->start && menu->start->texture){
    SDL_SetRenderDrawColor(menu->renderer,
                          menu->start->is_hovered ? 150 : 128,
                          menu->start->is_hovered ? 150 : 128,
                          menu->start->is_hovered ? 150 : 128,
                          255);
    SDL_RenderFillRect(menu->renderer, &menu->start->rect);
    SDL_RenderCopy(menu->renderer, menu->start->texture, NULL, &menu->start->rect);
}
}

void
menu_handle_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            menu->is_running = false;
        } else if (event.type == SDL_MOUSEMOTION) {
            int x = event.motion.x;
            int y = event.motion.y;
            
            // Check if mouse is over buttons and update hover state
            menu->one_dimension->is_hovered = IS_MOUSE_INSIDE(x, y, menu->one_dimension->rect);
            menu->two_dimension->is_hovered = IS_MOUSE_INSIDE(x, y, menu->two_dimension->rect);
            menu->exit->is_hovered = IS_MOUSE_INSIDE(x, y, menu->exit->rect);
            menu->music->is_hovered = IS_MOUSE_INSIDE(x, y, menu->music->rect);
            menu->start->is_hovered = IS_MOUSE_INSIDE(x, y, menu->start->rect);
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x = event.button.x;
            int y = event.button.y;

            if(IS_MOUSE_INSIDE(x, y, menu->one_dimension->rect)){
                // Handle one dimension button click
                printf("One Dimension button clicked.\n");
            }
            if(IS_MOUSE_INSIDE(x, y, menu->two_dimension->rect)){
                // Handle two dimension button click
                SDL_DestroyTexture(menu->one_dimension->texture);
                SDL_DestroyTexture(menu->two_dimension->texture);
                menu->one_dimension->texture = NULL;
                menu->two_dimension->texture = NULL;
                menu->one_dimension->rect = (SDL_Rect){0, 0, 0, 0};
                menu->two_dimension->rect = (SDL_Rect){0, 0, 0, 0};
                play_menu = true;
            }
            if(IS_MOUSE_INSIDE(x, y, menu->music->rect)){
                // Handle music button click
                printf("Music button clicked.\n");
            }

            // Handle button clicks
            if (IS_MOUSE_INSIDE(x, y, menu->exit->rect)) {
                menu->is_running = false;
            }
            
            // Add handling for other buttons as needed
        }
    }
}

bool
menu_run_status(){
    return menu->is_running;
}

void
menu_destroy(){
    if (menu == NULL)
    {
        return;
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
    
    if (menu->music && menu->music->texture) {
        SDL_DestroyTexture(menu->music->texture);
        free(menu->music);
    }

    SDL_DestroyRenderer(menu->renderer);
    SDL_DestroyWindow(menu->window);
    free(menu);
    menu = NULL;
    printf("Game menu destroyed.\n");
}
