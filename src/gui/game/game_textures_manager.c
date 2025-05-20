#include "gui/game/textures_manager.h"
#include "gui/game/game_engine.h"
#include "gui/game/textures.h"
#include "gui/game_flow/map_setup.h"
#include "gui/game/events_manager.h"

void
PNG_load(GameEngine* engine)
{
    SDL_Surface* map_PNG = IMG_Load(ASSETS_DIR MAP_SPRITE_SHEET);
    if (!map_PNG) {
        printf(LOAD_MAP_FAILED_MSG, IMG_GetError());
        return;
    }

    engine->map_texture = SDL_CreateTextureFromSurface(engine->renderer, map_PNG);
    SDL_FreeSurface(map_PNG);
    if (!engine->map_texture) {
        fprintf(stderr, CREATE_MAP_TEXTURE_FAILED_MSG, SDL_GetError());
        return;
    }

    SDL_Surface* hider_PNG = IMG_Load(ASSETS_DIR HIDER_SPRITE_SHEET);
    if (!hider_PNG) {
        printf(LOAD_HIDER_FAILED_MSG, IMG_GetError());
        return;
    }
    engine->hider_texture = SDL_CreateTextureFromSurface(engine->renderer, hider_PNG);
    SDL_FreeSurface(hider_PNG);
    if (!engine->hider_texture) {
        fprintf(stderr, CREATE_HIDER_TEXTURE_FAILED_MSG, SDL_GetError());
        return;
    }

    // SDL_Surface* seeker_PNG = IMG_Load("assets/seeker_SpritedSheet.png");
    // if (!seeker_PNG) {
    //     printf(LOAD_SEEKER_FAILED_MSG, IMG_GetError());
    //     return;
    // }
    // engine->seeker_texture = SDL_CreateTextureFromSurface(engine->renderer, seeker_PNG);
    // SDL_FreeSurface(seeker_PNG);
    // if (!engine->seeker_texture) {
    //     fprintf(stderr, CREATE_SEEKER_TEXTURE_FAILED_MSG, SDL_GetError());
    //     return;
    // }

    SDL_Surface* easy_Chest_PNG = IMG_Load(ASSETS_DIR EASY_CHEST_IMG);
    if (!easy_Chest_PNG) {
        printf(LOAD_EASY_CHEST_FAILED_MSG, IMG_GetError());
        return;
    }
    engine->easy_chest_texture = SDL_CreateTextureFromSurface(engine->renderer, easy_Chest_PNG);
    SDL_FreeSurface(easy_Chest_PNG);
    if (!engine->easy_chest_texture) {
        fprintf(stderr, CREATE_EASY_CHEST_TEXTURE_FAILED_MSG, SDL_GetError());
        return;
    }

    SDL_Surface* nutural_Chest_PNG = IMG_Load(ASSETS_DIR NEUTRAL_CHEST_IMG);
    if (!nutural_Chest_PNG) {
        printf(LOAD_NEUTRAL_CHEST_FAILED_MSG, IMG_GetError());
        return;
    }
    engine->nutural_chest_texture = SDL_CreateTextureFromSurface(engine->renderer, nutural_Chest_PNG);
    SDL_FreeSurface(nutural_Chest_PNG);
    if (!engine->nutural_chest_texture) {
        fprintf(stderr, CREATE_NEUTRAL_CHEST_TEXTURE_FAILED_MSG, SDL_GetError());
        return;
    }

    SDL_Surface* hard_Chest_PNG = IMG_Load(ASSETS_DIR HARD_CHEST_IMG);
    if (!hard_Chest_PNG) {
        printf(LOAD_HARD_CHEST_FAILED_MSG, IMG_GetError());
        return;
    }
    engine->hard_chest_texture = SDL_CreateTextureFromSurface(engine->renderer, hard_Chest_PNG);
    SDL_FreeSurface(hard_Chest_PNG);
    if (!engine->hard_chest_texture) {
        fprintf(stderr, CREATE_HARD_CHEST_TEXTURE_FAILED_MSG, SDL_GetError());
        return;
    }
}

/* Update animation frames based on elapsed time */
void
game_engine_update_animations(GameEngine* engine)
{
    Uint32 current_time = SDL_GetTicks();
    Uint32 elapsed_time = current_time - engine->last_update_time;

    // Update frame time accumulator
    engine->frame_time += elapsed_time;

    // Update animation frames if enough time has passed
    if (engine->frame_time >= engine->frame_delay) {
        // Reset frame time and save some remainder for smoother animation
        engine->frame_time %= engine->frame_delay;

        // Update each animated sprite's current frame
        engine->map_current_frame = (engine->map_current_frame + 1) % engine->map_frame_count;
        engine->hider_current_frame = (engine->hider_current_frame + 1) % engine->hider_frame_count;
        engine->seeker_current_frame = (engine->seeker_current_frame + 1) % engine->seeker_frame_count;
        // engine->easy_chest_current_frame = (engine->easy_chest_current_frame + 1) % engine->easy_chest_frame_count;
        // engine->nutural_chest_current_frame = (engine->nutural_chest_current_frame + 1) % engine->nutural_chest_frame_count;
        // engine->hard_chest_current_frame = (engine->hard_chest_current_frame + 1) % engine->hard_chest_frame_count;
    }

    // Save current time for next update
    engine->last_update_time = current_time;
}

/* Render game objects with animations */
void
render_game_objects(GameEngine* engine)
{
    // Render map (assuming it's a background)
    if (engine->map_texture) {
        SDL_Rect srcRect = {
            engine->map_current_frame * MAP_FRAME_WIDTH,
            0,
            MAP_FRAME_WIDTH,
            MAP_FRAME_HEIGHT
        };
        SDL_Rect destRect = {0, 0, MAP_FRAME_WIDTH, MAP_FRAME_HEIGHT};
        SDL_RenderCopy(engine->renderer, engine->map_texture, &srcRect, &destRect);
    }



    // Render hider character
    if (engine->hider_texture) {
        SDL_Rect srcRect = {
            engine->hider_current_frame * CHARACTER_FRAME_WIDTH,
            0,
            CHARACTER_FRAME_WIDTH,
            CHARACTER_FRAME_HEIGHT
        };

        SDL_Rect destRect = {730, 180, CHARACTER_FRAME_WIDTH/8, CHARACTER_FRAME_HEIGHT/8};

        SDL_RenderCopy(engine->renderer, engine->hider_texture, &srcRect, &destRect);
    }

    if(engine->chests == NULL)
        init_chests();

    for(int i = 0 ; i < engine->dimension ; ++i)
    {
        Chest* chests = game_engine_get_chests();
        SDL_RenderCopy(engine->renderer, chests[i].texture, NULL, &(chests[i].rect));
    }

    // // Render seeker character
    // if (engine->seeker_texture) {
    //     SDL_Rect srcRect = {
    //         engine->seeker_current_frame * CHARACTER_FRAME_WIDTH,
    //         0,
    //         CHARACTER_FRAME_WIDTH,
    //         CHARACTER_FRAME_HEIGHT
    //     };

    //     SDL_Rect destRect = {500, 300, CHARACTER_FRAME_WIDTH/8, CHARACTER_FRAME_HEIGHT/8};

    //     SDL_RenderCopy(engine->renderer, engine->seeker_texture, &srcRect, &destRect);
    // }
}





void
destroy_texture(GameEngine* engine)
{
    if (engine->map_texture) {
        SDL_DestroyTexture(engine->map_texture);
    }
    if (engine->hider_texture) {
        SDL_DestroyTexture(engine->hider_texture);
    }
    if (engine->seeker_texture) {
        SDL_DestroyTexture(engine->seeker_texture);
    }
    if (engine->easy_chest_texture) {
        SDL_DestroyTexture(engine->easy_chest_texture);
    }
    if (engine->nutural_chest_texture) {
        SDL_DestroyTexture(engine->nutural_chest_texture);
    }
    if (engine->hard_chest_texture) {
        SDL_DestroyTexture(engine->hard_chest_texture);
    }
}