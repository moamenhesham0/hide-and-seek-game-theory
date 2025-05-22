#include "gui/sidemenu/sidemenu.h"
#include "gui/game/game_engine.h"
#include "gui/menu/main_menu.h"

static SideMenu* side_menu = NULL;

void load_side_menu_texture()
{
    SDL_Surface* surface = IMG_Load("assets/menu/side_menu.png");
    if (!surface) {
        fprintf(stderr, "Failed to load side menu texture: %s\n", IMG_GetError());
        return;
    }

    side_menu->texture = SDL_CreateTextureFromSurface(side_menu->renderer, surface);
    SDL_FreeSurface(surface);
    if (!side_menu->texture) {
        fprintf(stderr, "Failed to create side menu texture: %s\n", SDL_GetError());
        return;
    }

}

void
side_menu_init(GameEngine *engine)
{

    if (side_menu)
    {
        return;
    }

    side_menu->engine = engine;
    side_menu = malloc(sizeof(SideMenu));
    side_menu->texture = NULL;

    side_menu->width = SIDE_MENU_WIDTH;
    side_menu->height = SIDE_MENU_HEIGHT;


    // Initialize SDL window and renderer
    side_menu->window = SDL_CreateWindow(SIDE_MENU_NAME,
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         side_menu->width,
                                         side_menu->
height,
                                        SDL_WINDOW_SHOWN);
    if (!side_menu->window) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        free(side_menu);
        return;
    }

    side_menu->renderer = SDL_CreateRenderer(side_menu->window, -1, SDL_RENDERER_ACCELERATED);
    if (!side_menu->renderer) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(side_menu->window);
        free(side_menu);
        return;
    }

    load_side_menu_texture();

    // Load fonts
    side_menu->header_font = TTF_OpenFont(FONT_PATH, 24);
    side_menu->body_font = TTF_OpenFont(FONT_PATH, 16);
}


void
side_menu_render()
{
    SDL_RenderCopy(side_menu->renderer , side_menu->texture , NULL , NULL);

    SDL_Rect camera = { 0, 0, SIDE_MENU_WIDTH, SIDE_MENU_HEIGHT };

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        camera.x -= 20;
                        break;
                    case SDLK_RIGHT:
                        camera.x += 20;
                        break;
                    case SDLK_UP:
                        camera.y -= 20;
                        break;
                    case SDLK_DOWN:
                        camera.y += 20;
                        break;
                }
            }
        }


        // Clamp camera
        if (camera.x < 0) camera.x = 0;
        if (camera.y < 0) camera.y = 0;
        if (camera.x > WORLD_WIDTH - SIDE_MENU_WIDTH) camera.x = WORLD_WIDTH - SIDE_MENU_WIDTH;
        if (camera.y > WORLD_HEIGHT - SIDE_MENU_HEIGHT) camera.y = WORLD_HEIGHT - SIDE_MENU_HEIGHT;

        SDL_RenderClear(side_menu->renderer);
        SDL_RenderCopy(side_menu->renderer , side_menu->texture , &camera, NULL);
        SDL_RenderPresent(side_menu->renderer);
    }
}

