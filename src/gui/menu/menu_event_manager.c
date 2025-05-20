#include "gui/menu/menu_event_manager.h"
#include "gui/game/events_manager.h"
#include <stdlib.h>

void
menu_handler(GameMenu* menu, SDL_Event event){
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


        // Handle One and Two Dimension button clicks
        if(IS_MOUSE_INSIDE(x, y, menu->two_dimension->rect) ||
           IS_MOUSE_INSIDE(x, y, menu->one_dimension->rect)){

            menu->is_2d = IS_MOUSE_INSIDE(x, y, menu->two_dimension->rect);

            SDL_DestroyTexture(menu->one_dimension->texture);
            SDL_DestroyTexture(menu->two_dimension->texture);
            menu->one_dimension->texture = NULL;
            menu->two_dimension->texture = NULL;
            menu->one_dimension->rect = (SDL_Rect){0, 0, 0, 0};
            menu->two_dimension->rect = (SDL_Rect){0, 0, 0, 0};
            menu->play_menu = true;
        }

        // Add check for clicking the input box
        if(menu->play_menu && IS_MOUSE_INSIDE(x, y, menu->input_rect)) {
            menu->is_input_active = true;
            SDL_StartTextInput();
        } else if (!IS_MOUSE_INSIDE(x, y, menu->input_rect)) {
            // Deactivate input when clicking outside the input box
            menu->is_input_active = false;
            SDL_StopTextInput();
        }

        if(menu->play_menu && menu->input_text[0] != '\0' && IS_MOUSE_INSIDE(x, y, menu->start->rect)){
            // Handle start button click
            printf("Start button clicked. Input: %s\n", menu->input_text);
            menu->dimension = atoi(menu->input_text);
            menu->play_game = true;
            menu->is_running = false;
        }

        if(IS_MOUSE_INSIDE(x, y, menu->music->rect)){
            printf("Music button clicked.\n");
        }

        if (IS_MOUSE_INSIDE(x, y, menu->exit->rect)) {
            menu->is_running = false;
        }
    } else if (event.type == SDL_TEXTINPUT && menu->is_input_active) {
        if (strlen(menu->input_text) < MAX_INPUT_LENGTH - 1) {
            if (isdigit(event.text.text[0])) {
                strcat(menu->input_text, event.text.text);
            }
        }
    } else if (event.type == SDL_KEYDOWN && menu->is_input_active) {
        if (event.key.keysym.sym == SDLK_BACKSPACE) {
            int len = strlen(menu->input_text);
            if (len > 0) {
                menu->input_text[len - 1] = '\0';
            }
        } else if (event.key.keysym.sym == SDLK_RETURN) {
            menu->is_input_active = false;
            SDL_StopTextInput();
            int value = atoi(menu->input_text);
            printf("Entered value: %d\n", value);
        }
    }
}