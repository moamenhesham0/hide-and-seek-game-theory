#include "gui/game/textures_manager.h"
#include "gui/game/game_engine.h"
#include "gui/game/textures.h"
#include "gui/game_flow/map_setup.h"
#include "gui/game/events_manager.h"
#include "macros.h"
#include "game_logic/score.h"
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

    SDL_Surface* side_menu_PNG = IMG_Load(ASSETS_DIR SIDE_MENU_IMG);
    if (!side_menu_PNG) {
        printf(LOAD_MAP_FAILED_MSG, IMG_GetError());
        return;
    }
    engine->side_menu_texture = SDL_CreateTextureFromSurface(engine->renderer, side_menu_PNG);


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


void render_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, int x, int y)
{
    if (!font) return;

    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, color);
    if (!textSurface) return;

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void
render_side_menu(GameEngine* engine)
{
    // Define the side menu boundaries

    SDL_Rect menuBoundary = {1065, 96, 333, 821};

    // Render the side menu background within the specified boundaries
    SDL_RenderCopy(engine->renderer, engine->side_menu_texture, NULL, &((SDL_Rect){1024,0,400,1024}));

    // Get game data
    struct hider* hider = game_engine_get_hider();
    struct seeker* seeker = game_engine_get_seeker();
    int** payoff_matrix = game_engine_get_game_matrix();
    int dimension = game_engine_get_dimension();
    struct score* score = get_score(); // Access the score

    // Setup font and colors
    TTF_Font* titleFont = engine->font;
    TTF_Font* font = engine->font;
    TTF_Font* smallFont = engine->font;

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color gold = {255, 215, 0, 255};
    SDL_Color green = {100, 255, 100, 255};
    SDL_Color red = {255, 100, 100, 255};

    // Starting position within the menu boundaries
    int baseX = menuBoundary.x + 10;  // Adding padding from left edge
    int y_offset = menuBoundary.y + 20;
    int padding = 10;

    // --- TITLE ---
    render_text(engine->renderer, titleFont, "HIDE AND SEEK", gold, baseX + 80, y_offset);
    y_offset += 35;

    // --- LEGENDS ---
    render_text(engine->renderer, font, "LEGENDS:", white, baseX, y_offset);
    y_offset += 25;

    // Chest difficulty colors
    render_text(engine->renderer, smallFont, "Easy:", green, baseX, y_offset);
    SDL_SetRenderDrawColor(engine->renderer, 100, 200, 100, 255);
    SDL_Rect easyRect = {baseX + 80, y_offset, 20, 20};
    SDL_RenderFillRect(engine->renderer, &easyRect);

    render_text(engine->renderer, smallFont, "Neutral:", gold, baseX + 150, y_offset);
    SDL_SetRenderDrawColor(engine->renderer, 200, 200, 0, 255);
    SDL_Rect neutralRect = {baseX + 230, y_offset, 20, 20};
    SDL_RenderFillRect(engine->renderer, &neutralRect);

    y_offset += 25;
    render_text(engine->renderer, smallFont, "Hard:", red, baseX, y_offset);
    SDL_SetRenderDrawColor(engine->renderer, 200, 80, 80, 255);
    SDL_Rect hardRect = {baseX + 80, y_offset, 20, 20};
    SDL_RenderFillRect(engine->renderer, &hardRect);

    y_offset += 30;

    // Horizontal separator
    SDL_SetRenderDrawColor(engine->renderer, 180, 180, 180, 100);
    SDL_Rect separatorRect = {baseX, y_offset, menuBoundary.w - 20, 1};
    SDL_RenderFillRect(engine->renderer, &separatorRect);
    y_offset += 15;

    // --- SCORE ---
    render_text(engine->renderer, font, "SCORE:", white, baseX, y_offset);
    y_offset += 25;

    char scoreText[100];
    if (score) {
        snprintf(scoreText, 100, "Hider: %.2f", score->hider_score);
        render_text(engine->renderer, smallFont, scoreText, green, baseX, y_offset);

        render_text(engine->renderer, smallFont, "Seeker: %.2f", red, baseX + 150, y_offset);
        snprintf(scoreText, 100, "%.2f", score->seeker_score);
        render_text(engine->renderer, smallFont, scoreText, white, baseX + 220, y_offset);
    } else {
        render_text(engine->renderer, smallFont, "Score data unavailable", white, baseX, y_offset);
    }

    y_offset += 25;

    // Horizontal separator
    SDL_SetRenderDrawColor(engine->renderer, 180, 180, 180, 100);
    separatorRect.y = y_offset;
    SDL_RenderFillRect(engine->renderer, &separatorRect);
    y_offset += 15;

    // --- PAYOFF MATRIX ---
    render_text(engine->renderer, font, "PAYOFF MATRIX:", white, baseX, y_offset);
    y_offset += 25;

    if (payoff_matrix && dimension > 0) {
        int cellSize = 25;  // Slightly smaller cells to fit in narrower space
        int maxDisplayedDim = MIN(dimension, 50); // Limit display to 5x5

        // Draw column headers (Seeker choices)
        render_text(engine->renderer, smallFont, "S\\H", white, baseX, y_offset);
        for (int j = 0; j < maxDisplayedDim; j++) {
            char colHeader[10];
            snprintf(colHeader, 10, "%d", j+1);
            render_text(engine->renderer, smallFont, colHeader, white,
                      baseX + cellSize + j * cellSize, y_offset);
        }
        y_offset += 20;

        // Draw rows with row headers
        for (int i = 0; i < maxDisplayedDim; i++) {
            char rowHeader[10];
            snprintf(rowHeader, 10, "%d", i+1);
            render_text(engine->renderer, smallFont, rowHeader, white,
                      baseX, y_offset + i * cellSize);

            for (int j = 0; j < maxDisplayedDim; j++) {
                char cellValue[10];
                snprintf(cellValue, 10, "%d", payoff_matrix[i][j]);
                render_text(engine->renderer, smallFont, cellValue, white,
                          baseX + cellSize + j * cellSize, y_offset + i * cellSize);
            }
        }

        y_offset += cellSize * maxDisplayedDim + 10;

        if (dimension > maxDisplayedDim) {
            render_text(engine->renderer, smallFont, "Matrix truncated...", white, baseX, y_offset);
            y_offset += 20;
        }
    } else {
        render_text(engine->renderer, smallFont, "Matrix data unavailable", white, baseX, y_offset);
        y_offset += 20;
    }

    // Horizontal separator
    SDL_SetRenderDrawColor(engine->renderer, 180, 180, 180, 100);
    separatorRect.y = y_offset;
    SDL_RenderFillRect(engine->renderer, &separatorRect);
    y_offset += 15;

    // --- PROBABILITIES ---
    render_text(engine->renderer, font, "STRATEGIES:", white, baseX, y_offset);
    y_offset += 25;

    // Hider probabilities
    render_text(engine->renderer, smallFont, "Hider Probabilities:", green, baseX, y_offset);
    y_offset += 20;

    if (hider && hider->probabilities) {
        // Use two columns to save vertical space
        int probs_per_row = 2;
        int displayLimit = MIN(dimension, 8); // Show fewer to fit in space

        for (int i = 0; i < displayLimit; i++) {
            char probText[30];
            snprintf(probText, 30, "Box %d: %.2f", i+1, hider->probabilities[i]);
            render_text(engine->renderer, smallFont, probText, white,
                      baseX + (i % probs_per_row) * 160,
                      y_offset + (i / probs_per_row) * 20);
        }

        y_offset += ((displayLimit + probs_per_row - 1) / probs_per_row) * 20 + 5;

        if (dimension > displayLimit) {
            render_text(engine->renderer, smallFont, "More probabilities...", white, baseX, y_offset);
            y_offset += 20;
        }
    } else {
        render_text(engine->renderer, smallFont, "Hider data unavailable", white, baseX, y_offset);
        y_offset += 20;
    }

    // Seeker probabilities
    render_text(engine->renderer, smallFont, "Seeker Probabilities:", red, baseX, y_offset);
    y_offset += 20;

    if (seeker && seeker->probabilities) {
        int probs_per_row = 2;
        int displayLimit = MIN(dimension, 8); // Show fewer to fit in space

        for (int i = 0; i < displayLimit; i++) {
            char probText[30];
            snprintf(probText, 30, "Box %d: %.2f", i+1, seeker->probabilities[i]);
            render_text(engine->renderer, smallFont, probText, white,
                      baseX + (i % probs_per_row) * 160,
                      y_offset + (i / probs_per_row) * 20);
        }

        // Don't need to update y_offset here as we're at the end
    } else {
        render_text(engine->renderer, smallFont, "Seeker data unavailable", white, baseX, y_offset);
    }

    // Ensure we don't draw outside our boundaries
    if (y_offset > menuBoundary.y + menuBoundary.h - 20) {
        // We've gone too far - in a full implementation, you might need to adjust layout
        printf("Warning: Side menu content exceeds available height\n");
    }
}

// Helper function to render text





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