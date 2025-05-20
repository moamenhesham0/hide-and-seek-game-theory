#include "gui/menu/menu_textures_manager.h"

void
load_texture(GameMenu* menu){
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
    load_button(menu);
}

void
load_button(GameMenu* menu){
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
render_menu_objects(GameMenu* menu){
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

    if(menu->play_menu && menu->start && menu->start->texture){
        SDL_SetRenderDrawColor(menu->renderer,
                              menu->start->is_hovered ? 150 : 128,
                              menu->start->is_hovered ? 150 : 128,
                              menu->start->is_hovered ? 150 : 128,
                              255);
        SDL_RenderFillRect(menu->renderer, &menu->start->rect);
        SDL_RenderCopy(menu->renderer, menu->start->texture, NULL, &menu->start->rect);
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
                SDL_Rect text_rect = {menu->input_rect.x + 5, menu->input_rect.y + 5,
                                    text_surface->w, text_surface->h};
                SDL_RenderCopy(menu->renderer, text_texture, NULL, &text_rect);
                SDL_DestroyTexture(text_texture);
                SDL_FreeSurface(text_surface);
            }
        }

        // Render placeholder text when input is empty
        if (strlen(menu->input_text) == 0) {
            SDL_Color text_color = {128, 128, 128, 255}; // Gray color for placeholder
            SDL_Surface* text_surface = TTF_RenderText_Solid(menu->font, "Click to enter size", text_color);
            if (text_surface) {
                SDL_Texture* text_texture = SDL_CreateTextureFromSurface(menu->renderer, text_surface);
                SDL_Rect text_rect = {menu->input_rect.x + 5, menu->input_rect.y + 5,
                                    text_surface->w, text_surface->h};
                SDL_RenderCopy(menu->renderer, text_texture, NULL, &text_rect);
                SDL_DestroyTexture(text_texture);
                SDL_FreeSurface(text_surface);
            }
        }
    }
}