#include "gui/game/textures_manager.h"
#include "gui/game/game_engine.h"
#include "gui/game/textures.h"
#include "gui/game_flow/map_setup.h"
#include "gui/game/events_manager.h"
#include "macros.h"
#include <math.h>
#include <stdio.h>

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


    if(engine->is_hider)
    {
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
    }


    SDL_Surface* seeker_PNG = IMG_Load("assets/game/seeker_sprite_sheet.png");
    if (!seeker_PNG) {
        printf(LOAD_SEEKER_FAILED_MSG, IMG_GetError());
        return;
    }
    engine->seeker_texture = SDL_CreateTextureFromSurface(engine->renderer, seeker_PNG);
    SDL_FreeSurface(seeker_PNG);
    if (!engine->seeker_texture) {
        fprintf(stderr, CREATE_SEEKER_TEXTURE_FAILED_MSG, SDL_GetError());
        return;
    }

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
        if(engine->is_2d)
            engine->map_current_frame = (engine->map_current_frame + 1) % engine->map_frame_count;

        if(!IS_SAME_RECT(engine->hider_src_rect , engine->hider_dst_rect))
        {
            engine->hider_current_frame = (engine->hider_current_frame + 1) % CHARACTER_FRAME_COUNT;
            animate_movement(&engine->hider_src_rect , &engine->hider_dst_rect , &engine->hider_current_direction , HIDER);
        }
        else
        {
            hider_chest_action();
        }

        if(!IS_SAME_RECT(engine->seeker_src_rect , engine->seeker_dst_rect) && engine->hiding_flag == HIDING)
        {
            engine->seeker_current_frame = (engine->seeker_current_frame + 1) % CHARACTER_FRAME_COUNT;
            animate_movement(&engine->seeker_src_rect , &engine->seeker_dst_rect , &engine->seeker_current_direction , SEEKER);
        }
        else
        {
            seeker_chest_action();
        }
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

        if(engine->is_2d)
        {
            SDL_RenderCopy(engine->renderer, engine->map_texture, &srcRect, &destRect);
        }
        else
        {
            // Create a full gray background rect
            SDL_SetRenderDrawColor(engine->renderer, 100, 100, 100, 255); // Darker gray
            SDL_RenderFillRect(engine->renderer, &srcRect);
        }


    }

    if(engine->chests == NULL)
        init_chests();

    for(int i = 0 ; i < engine->dimension ; ++i)
    {
        Chest* chests = game_engine_get_chests();
        SDL_Rect srcRect = {
            CHEST_FRAME_X_OFFSET,
            chests[i].state*CHARACTER_FRAME_HEIGHT + CHEST_FRAME_Y_OFFSET,
            CHEST_FRAME_WIDTH,
            CHEST_FRAME_HEIGHT
        };
        SDL_RenderCopy(engine->renderer, chests[i].texture, &srcRect , &(chests[i].rect));

        if(chests[i].state == CLOSED)
            render_hover(&chests[i]);
    }

    // Render hider character
    if (engine->hider_texture && engine->hiding_flag <= HIDER_OPEN_BOX && engine->is_hider) {
        SDL_Rect srcRect = {
            engine->hider_current_frame * CHARACTER_FRAME_WIDTH,
            engine->hider_current_direction * CHARACTER_FRAME_HEIGHT,
            CHARACTER_FRAME_WIDTH,
            CHARACTER_FRAME_HEIGHT
        };


        SDL_RenderCopy(engine->renderer, engine->hider_texture, &srcRect  , &engine->hider_src_rect);
    }


    // Render seeker character
    if (engine->seeker_texture && engine->hiding_flag >= HIDING && engine->hiding_flag < SEEKER_OPEN_BOX) {
        SDL_Rect srcRect = {
            engine->seeker_current_frame * CHARACTER_FRAME_WIDTH,
            engine->seeker_current_direction * CHARACTER_FRAME_HEIGHT,
            CHARACTER_FRAME_WIDTH,
            CHARACTER_FRAME_HEIGHT
        };


        SDL_RenderCopy(engine->renderer, engine->seeker_texture, &srcRect, &engine->seeker_src_rect);
    }
}


void render_hover(Chest* chest)
{
    if (!chest || !chest->is_hovered) return;

    SDL_Renderer* renderer = game_engine_get_renderer();
    if (!renderer) return;

    // Save current renderer settings
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    SDL_BlendMode originalBlendMode;
    SDL_GetRenderDrawBlendMode(renderer, &originalBlendMode);

    // Set blend mode to additive for a highlight glow effect
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);

    // Choose highlight color based on difficulty
    switch (chest->difficulty) {
        case _EASY:
            SDL_SetRenderDrawColor(renderer, 100, 200, 100, 100); // Green for easy
            break;
        case _NEUTRAL:
            SDL_SetRenderDrawColor(renderer, 200, 200, 0, 100);   // Yellow for neutral
            break;
        case _HARD:
            SDL_SetRenderDrawColor(renderer, 200, 80, 80, 100);   // Red for hard
            break;
        default:
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100); // White as fallback
    }

    // Fill the chest's rectangle with the semi-transparent color
    SDL_RenderFillRect(renderer, &chest->rect);

    // Create a pulsing effect by adjusting opacity
    Uint32 currentTime = SDL_GetTicks();
    int pulseValue = (int)(50.0 * sin(currentTime * 0.005) + 50.0);

    // Draw a brighter inner highlight
    SDL_Rect innerRect = chest->rect;

    switch (chest->difficulty) {
        case EASY:
            SDL_SetRenderDrawColor(renderer, 150, 255, 150, pulseValue);
            break;
        case NEUTRAL:
            SDL_SetRenderDrawColor(renderer, 255, 255, 100, pulseValue);
            break;
        case HARD:
            SDL_SetRenderDrawColor(renderer, 255, 100, 100, pulseValue);
            break;
        default:
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, pulseValue);
    }

    SDL_RenderFillRect(renderer, &innerRect);

    // Restore original renderer settings
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_SetRenderDrawBlendMode(renderer, originalBlendMode);
}

void
hider_chest_action()
{
    static uint32_t start;

    if(game_engine_get_hiding_flag() >= HIDING || !IS_SAME_RECT(game_engine_get_hider_src_rect() , game_engine_get_chests()[game_engine_get_hider_choice()].rect))
    {
        return;
    }

    if(game_engine_get_hiding_flag() == NOT_HIDING)
    {
        start = SDL_GetTicks64();
        game_engine_set_hiding_flag(HIDER_OPEN_BOX);
        game_engine_get_chests()[game_engine_get_hider_choice()].state = OPENED;
        game_engine_set_hider_current_direction(UPWARD);
    }

    uint32_t current_time = SDL_GetTicks64();
    if (current_time - start >= HIDE_DELAY)
    {
            game_engine_set_hiding_flag(INSIDE_BOX);
            game_engine_get_chests()[game_engine_get_hider_choice()].state = HIDER_INSIDE;
    }

    if(current_time - start >= BOX_CLOSE_DELAY + HIDE_DELAY)
    {
            game_engine_get_chests()[game_engine_get_hider_choice()].state = CLOSED;
            game_engine_set_hiding_flag(HIDING);
    }

}

void
seeker_chest_action()
{
    static uint32_t start;
    if(game_engine_get_hiding_flag() < HIDING || !IS_SAME_RECT(game_engine_get_seeker_src_rect() , game_engine_get_chests()[game_engine_get_seeker_choice()].rect))
    {
        return;
    }

    if(game_engine_get_hiding_flag() == HIDING)
    {
        start = SDL_GetTicks64();
        game_engine_set_hiding_flag(SEEKING);
        game_engine_set_seeker_current_direction(UPWARD);
    }




    uint32_t current_time = SDL_GetTicks64();
    if (current_time - start >= HIDE_DELAY)
    {
        game_engine_set_hiding_flag(SEEKER_OPEN_BOX);
        game_engine_get_chests()[game_engine_get_seeker_choice()].state = SEEKER_FOUND_EMPTY;
    }

    if(current_time - start >= BOX_CLOSE_DELAY + HIDE_DELAY)
    {
        if(game_engine_get_hider_choice() == game_engine_get_seeker_choice())
        {
            game_engine_get_chests()[game_engine_get_seeker_choice()].state = SEEKER_FOUND_HIDER;
        }
        else
        {
            game_engine_get_chests()[game_engine_get_hider_choice()].state = HIDER_INSIDE;
        }
        game_engine_set_hiding_flag(ROUND_END);
    }

}

void
animate_movement(SDL_Rect* src , SDL_Rect* dst , int* curr_dir , bool is_hider)
{
    int dx = dst->x - src->x;
    int dy = dst->y - src->y;


    float distance = sqrt(dx*dx + dy*dy);

    // Set appropriate character direction
    if (abs(dx) > abs(dy)) {
        *curr_dir = (dx > 0) ? RIGHTWARD : LEFTWARD;
    } else {
        *curr_dir = (dy > 0) ? DOWNWARD : UPWARD;
    }

    if (distance < MOVE_SPEED)
    {
        src->x  = dst->x;
        src->y  = dst->y;
    }
    else
    {
        float ratio = (distance >= THRESHOLD_DIST ?RUNNING_SPEED : MOVE_SPEED) / distance;
        src->x += dx * ratio;
        src->y += dy * ratio;
    }

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