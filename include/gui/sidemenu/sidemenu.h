#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

struct GameEngine;
typedef struct GameEngine GameEngine;
#define SIDE_MENU_NAME "Game Stats"
#define SIDE_MENU_WIDTH 400
#define SIDE_MENU_HEIGHT 1024

#define WORLD_WIDTH 1200
#define WORLD_HEIGHT 2048

typedef struct
{
    GameEngine* engine; // Pointer to the game engine
    SDL_Window* window; // Pointer to the SDL window
    SDL_Renderer* renderer; // Pointer to the SDL renderer

    SDL_Texture* texture; // Pointer to the SDL texture

    int width; // Width of the texture
    int height; // Height of the texture

    TTF_Font* header_font;
    TTF_Font* body_font;

} SideMenu;

void side_menu_init(GameEngine * engine);

void side_menu_render();


