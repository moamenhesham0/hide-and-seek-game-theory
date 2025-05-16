#include "gui/textures_manager.h"
#include "gui/game_engine.h"

SDL_Texture*
load_texture(const char* t_file)
{
    SDL_Surface* temp_surface = IMG_Load(t_file);

    if (!temp_surface)
    {
        fprintf(stderr, LOAD_IMAGE_FAILED_MSG , IMG_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(game_engine_get_renderer(), temp_surface);
    SDL_FreeSurface(temp_surface);

    if (!texture)
    {
        fprintf(stderr, CREATE_TEXTURE_FAILED_MSG, SDL_GetError());
        return NULL;
    }

    return texture;

}



void
destroy_texture(SDL_Texture* t_texture)
{
    if (t_texture != NULL)
    {
        SDL_DestroyTexture(t_texture);
    }
}