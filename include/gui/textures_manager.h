#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define TEXTURE_DIR "assets/"

#define LOAD_IMAGE_FAILED_MSG "Failed to load image: %s\n"
#define CREATE_TEXTURE_FAILED_MSG "Failed to create texture: %s\n"



SDL_Texture*
load_texture(const char* t_file);


void
destroy_texture(SDL_Texture* t_texture);











