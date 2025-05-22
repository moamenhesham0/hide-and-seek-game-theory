#include "gui/menu/menu_textures_manager.h"

void
load_texture(GameMenu* menu){
    menu->background = IMG_LoadTexture(menu->renderer, "assets/menu/leaves.png");
    if (menu->background == NULL) {
        fprintf(stderr, "Failed to load background texture: %s\n", IMG_GetError());
        return;
    }
    menu->current_bg_frame = 0;
    menu->last_bg_frame_time = SDL_GetTicks();

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

    menu->hider_seeker->texture = IMG_LoadTexture(menu->renderer, "assets/menu/hider.png");
    if (menu->hider_seeker->texture == NULL) {
        fprintf(stderr, "Failed to load hider_seeker texture: %s\n", IMG_GetError());
        return;
    }

    menu->start->texture = IMG_LoadTexture(menu->renderer, "assets/menu/start.png");
    if (menu->start->texture == NULL) {
        fprintf(stderr, "Failed to load start texture: %s\n", IMG_GetError());
        return;
    }

    SDL_Surface* places_PNG = IMG_Load("assets/menu/places.png");
    if (!places_PNG) {
        printf("Failed to load places texture: %s\n", IMG_GetError());
        return;
    }
    menu->places = SDL_CreateTextureFromSurface(menu->renderer, places_PNG);
    SDL_FreeSurface(places_PNG);
    if (!menu->places) {
        fprintf(stderr, "Failed to create places texture: %s\n", SDL_GetError());
        return;
    }

    SDL_Surface* mood_PNG = IMG_Load("assets/menu/mood.png");
    if (!mood_PNG) {
        printf("Failed to load mood texture: %s\n", IMG_GetError());
        return;
    }
    menu->mood = SDL_CreateTextureFromSurface(menu->renderer, mood_PNG);
    SDL_FreeSurface(mood_PNG);
    if (!menu->mood) {
        fprintf(stderr, "Failed to create mood texture: %s\n", SDL_GetError());
        return;
    }

    SDL_Surface* simulate_PNG = IMG_Load("assets/menu/simulation.png");
    if (!simulate_PNG) {
        printf("Failed to load simulation texture: %s\n", IMG_GetError());
        return;
    }
    menu->simulation->texture = SDL_CreateTextureFromSurface(menu->renderer, simulate_PNG);
    SDL_FreeSurface(simulate_PNG);
    if (!menu->simulation) {
        fprintf(stderr, "Failed to create simulation texture: %s\n", SDL_GetError());
        return;
    }

    // Enable transparency for all button textures
    if(menu->one_dimension->texture)
        SDL_SetTextureBlendMode(menu->one_dimension->texture, SDL_BLENDMODE_BLEND);
    if(menu->two_dimension->texture)
        SDL_SetTextureBlendMode(menu->two_dimension->texture, SDL_BLENDMODE_BLEND);
    if(menu->exit->texture)
        SDL_SetTextureBlendMode(menu->exit->texture, SDL_BLENDMODE_BLEND);
    if(menu->music->texture)
        SDL_SetTextureBlendMode(menu->music->texture, SDL_BLENDMODE_BLEND);
    if(menu->hider_seeker->texture)
        SDL_SetTextureBlendMode(menu->hider_seeker->texture, SDL_BLENDMODE_BLEND);
    if(menu->start->texture)
        SDL_SetTextureBlendMode(menu->start->texture, SDL_BLENDMODE_BLEND);

    load_button(menu);
}

void
load_button(GameMenu* menu){
    // One dimension button
    menu->one_dimension->is_hovered = false;
    menu->one_dimension->rect = (SDL_Rect){296, 225, 200, 36};

    // Two dimension button
    menu->two_dimension->is_hovered = false;
    menu->two_dimension->rect = (SDL_Rect){296, 325, 200, 36};

    // Exit button - moved to bottom left
    menu->exit->is_hovered = false;
    menu->exit->rect = (SDL_Rect){25, 550, 80, 30};

    // Music button - moved to bottom right
    menu->music->is_hovered = false;
    menu->music->rect = (SDL_Rect){750, 550, 35, 35};

    menu->hider_seeker->is_hovered = false;
    menu->hider_seeker->rect = (SDL_Rect){435, 325, 80, 80};

    // Start button - moved to center
    menu->start->is_hovered = false;
    menu->start->rect = (SDL_Rect){350, 500, 105, 30};

    menu->simulation->is_hovered = false;
    menu->simulation->rect = (SDL_Rect){296, 425, 200, 36};
}

void
render_menu_objects(GameMenu* menu){
    // Render background animation
    if(menu->background){
        Uint32 current_time = SDL_GetTicks();
        if (current_time - menu->last_bg_frame_time >= MENU_FRAME_DELAY) {
            menu->current_bg_frame = (menu->current_bg_frame + 1) % MENU_FRAME_COUNT;
            menu->last_bg_frame_time = current_time;
        }
        int current_row = menu->current_bg_frame / MENU_SPRITE_COLS;
        int current_col = menu->current_bg_frame % MENU_SPRITE_COLS;
        SDL_Rect srcRect = {
            current_col * MENU_FRAME_WIDTH,
            current_row * MENU_FRAME_HEIGHT,
            MENU_FRAME_WIDTH,
            MENU_FRAME_HEIGHT
        };
        SDL_Rect dstRect = {0, 0, MENU_FRAME_WIDTH, MENU_FRAME_HEIGHT};
        SDL_RenderCopy(menu->renderer, menu->background, &srcRect, &dstRect);
    }
    
    // Render title
    if(menu->title){
        SDL_Rect title_rect = {200, 50, 400, 100};
        SDL_RenderCopy(menu->renderer, menu->title, NULL, &title_rect);
    }

    // Render one dimension button
    if(menu->one_dimension && menu->one_dimension->texture) {
        // Set blend mode for transparency
        SDL_SetTextureBlendMode(menu->one_dimension->texture, SDL_BLENDMODE_BLEND);
        // Set alpha based on hover state
        SDL_SetTextureAlphaMod(menu->one_dimension->texture, 
                              menu->one_dimension->is_hovered ? 255 : 180);
        SDL_RenderCopy(menu->renderer, menu->one_dimension->texture, NULL, 
                      &menu->one_dimension->rect);
    }

    // Render two dimension button
    if(menu->two_dimension && menu->two_dimension->texture) {
        SDL_SetTextureBlendMode(menu->two_dimension->texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(menu->two_dimension->texture, 
                              menu->two_dimension->is_hovered ? 255 : 180);
        SDL_RenderCopy(menu->renderer, menu->two_dimension->texture, NULL, 
                      &menu->two_dimension->rect);
    }

    if(menu->simulation && menu->simulation->texture && !menu->play_menu) {
        SDL_SetTextureBlendMode(menu->simulation->texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(menu->simulation->texture, 
                              menu->simulation->is_hovered ? 255 : 180);
        SDL_RenderCopy(menu->renderer, menu->simulation->texture, NULL, 
                      &menu->simulation->rect);
    }

    // Render exit button
    if(menu->exit && menu->exit->texture) {
        SDL_SetTextureBlendMode(menu->exit->texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(menu->exit->texture, 
                              menu->exit->is_hovered ? 255 : 180);
        SDL_RenderCopy(menu->renderer, menu->exit->texture, NULL, 
                      &menu->exit->rect);
    }

    // Render music button
    if(menu->music && menu->music->texture) {
        SDL_SetTextureBlendMode(menu->music->texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(menu->music->texture, 
                              menu->music->is_hovered ? 255 : 180);
        SDL_Rect srcRect = {
            menu->music_current_frame * MUSIC_FRAME_WIDTH,
            0,
            MUSIC_FRAME_WIDTH,
            MUSIC_FRAME_HEIGHT
        };
        SDL_RenderCopy(menu->renderer, menu->music->texture, &srcRect, 
                      &menu->music->rect);
    }

    // Render hider seeker button
    if(menu->play_menu && menu->hider_seeker && menu->hider_seeker->texture) {
        SDL_SetTextureBlendMode(menu->hider_seeker->texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(menu->hider_seeker->texture, 
                              menu->hider_seeker->is_hovered ? 255 : 180);
        SDL_RenderCopy(menu->renderer, menu->hider_seeker->texture, NULL, 
                      &menu->hider_seeker->rect);
    }

    // Render start button
    if(menu->play_menu && menu->start && menu->start->texture) {
        SDL_SetTextureBlendMode(menu->start->texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(menu->start->texture, 
                              menu->start->is_hovered ? 255 : 180);
        SDL_RenderCopy(menu->renderer, menu->start->texture, NULL, 
                      &menu->start->rect);
    }

    // Render places texture
    if (menu->places && menu->play_menu) {
        SDL_Rect places_rect = {260, 280, 173, 23};
        SDL_RenderCopy(menu->renderer, menu->places, NULL, &places_rect);
    }

    // Render mood texture
    if (menu->mood && menu->play_menu) {
        SDL_Rect mood_rect = {260, 350, 173, 23};
        SDL_RenderCopy(menu->renderer, menu->mood, NULL, &mood_rect);
    }

    // Render input field when play menu is active
    if (menu->play_menu) {
        // Draw input box background with different colors based on active state
        if (menu->is_input_active) {
            SDL_SetRenderDrawColor(menu->renderer, 255, 255, 255, 255); // White when active
        } else {
            SDL_SetRenderDrawColor(menu->renderer, 200, 200, 200, 255); // Light gray when inactive
        }
        SDL_RenderFillRect(menu->renderer, &menu->input_rect);

        // Draw border
        SDL_SetRenderDrawColor(menu->renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(menu->renderer, &menu->input_rect);

        // Render input text
        if (strlen(menu->input_text) > 0) {
            SDL_Color text_color = {0, 0, 0, 255};
            SDL_Surface* text_surface = TTF_RenderText_Solid(menu->font, menu->input_text, text_color);
            if (text_surface) {
                SDL_Texture* text_texture = SDL_CreateTextureFromSurface(menu->renderer, text_surface);
                SDL_Rect text_rect = {menu->input_rect.x, menu->input_rect.y + 5,
                                    text_surface->w, text_surface->h};
                SDL_RenderCopy(menu->renderer, text_texture, NULL, &text_rect);
                SDL_DestroyTexture(text_texture);
                SDL_FreeSurface(text_surface);
            }
        }
    }
}