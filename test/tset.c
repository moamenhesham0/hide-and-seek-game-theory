#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define MAX_INPUT_LENGTH 10

int main() {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font = NULL;
    bool success = true;

    // Initialize SDL and SDL_ttf
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
        goto cleanup;
    }

    if (TTF_Init() < 0) {
        SDL_Log("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        success = false;
        goto cleanup;
    }

    window = SDL_CreateWindow("Number Input Field",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        success = false;
        goto cleanup;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        success = false;
        goto cleanup;
    }

    font = TTF_OpenFont("/usr/share/fonts/truetype/msttcorefonts/Times_New_Roman_Bold.ttf", 32);
    if (!font) {
        SDL_Log("Failed to load font: %s\n", TTF_GetError());
        success = false;
        goto cleanup;
    }

    char inputText[MAX_INPUT_LENGTH + 1] = "";
    bool running = true;
    bool inputActive = false;
    SDL_Event event;
    SDL_Rect inputRect = { 300, 250, 200, 50 };

    SDL_StartTextInput();

    while (running && success) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                // Check if click is inside input box
                if (mouseX >= inputRect.x && mouseX <= (inputRect.x + inputRect.w) &&
                    mouseY >= inputRect.y && mouseY <= (inputRect.y + inputRect.h)) {
                    inputActive = true;
                } else {
                    inputActive = false;
                }
            } else if (event.type == SDL_TEXTINPUT && inputActive) {
                // Only allow digits
                if (isdigit(event.text.text[0]) && strlen(inputText) < MAX_INPUT_LENGTH) {
                    strncat(inputText, event.text.text, 1);
                }
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0 && inputActive) {
                    inputText[strlen(inputText) - 1] = '\0';
                } else if (event.key.keysym.sym == SDLK_RETURN && strlen(inputText) > 0) {
                    printf("Entered number: %s\n", inputText);
                    // Clear the input after printing
                    inputText[0] = '\0';
                }
            }
        }

        // Draw background
        if (SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255) < 0 ||
            SDL_RenderClear(renderer) < 0) {
            SDL_Log("Render error: %s\n", SDL_GetError());
            success = false;
            break;
        }

        // Draw input box (highlight when active)
        if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) < 0 ||
            SDL_RenderDrawRect(renderer, &inputRect) < 0) {
            SDL_Log("Render error: %s\n", SDL_GetError());
            success = false;
            break;
        }

        // Draw filled rectangle when active
        if (inputActive) {
            SDL_Rect highlightRect = { inputRect.x + 2, inputRect.y + 2, inputRect.w - 4, inputRect.h - 4 };
            if (SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255) < 0 ||
                SDL_RenderFillRect(renderer, &highlightRect) < 0) {
                SDL_Log("Render error: %s\n", SDL_GetError());
                success = false;
                break;
            }
        }

        // Render the input text
        if (strlen(inputText) > 0) {
            SDL_Color textColor = { 255, 255, 255, 255 };
            SDL_Surface* textSurface = TTF_RenderText_Blended(font, inputText, textColor);
            if (!textSurface) {
                SDL_Log("Text rendering error: %s\n", TTF_GetError());
                success = false;
                break;
            }

            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (!textTexture) {
                SDL_FreeSurface(textSurface);
                SDL_Log("Texture creation error: %s\n", SDL_GetError());
                success = false;
                break;
            }

            SDL_Rect textRect = { inputRect.x + 10, inputRect.y + 10, textSurface->w, textSurface->h };
            if (SDL_RenderCopy(renderer, textTexture, NULL, &textRect) < 0) {
                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
                SDL_Log("Render copy error: %s\n", SDL_GetError());
                success = false;
                break;
            }

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }

        SDL_RenderPresent(renderer);
    }

cleanup:
    SDL_StopTextInput();
    if (font) TTF_CloseFont(font);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    
    return success ? 0 : 1;
}
