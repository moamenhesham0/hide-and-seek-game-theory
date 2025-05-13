#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

// Adjust these constants based on your actual sprite sheet
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FRAME_WIDTH 384      // 1536 / 4 frames = 384 pixels wide per frame
#define FRAME_HEIGHT 1024    // Full height of each frame
#define FRAME_COUNT 4
#define ANIM_DELAY 150       // milliseconds
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Pixel Animation",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Load first sprite sheet
    SDL_Surface* tempSurface1 = IMG_Load("assets/player1.png");
    if (!tempSurface1) {
        printf("Failed to load image 1: %s\n", IMG_GetError());
        return 1;
    }

    // Print image dimensions to confirm they match expectations
    printf("Loaded image 1 with dimensions: %d x %d\n", tempSurface1->w, tempSurface1->h);

    SDL_Texture* spriteSheet1 = SDL_CreateTextureFromSurface(renderer, tempSurface1);
    SDL_FreeSurface(tempSurface1);

    // Load second sprite sheet
    SDL_Surface* tempSurface2 = IMG_Load("assets/player.png");
    if (!tempSurface2) {
        printf("Failed to load image 2: %s\n", IMG_GetError());
        return 1;
    }

    printf("Loaded image 2 with dimensions: %d x %d\n", tempSurface2->w, tempSurface2->h);

    SDL_Texture* spriteSheet2 = SDL_CreateTextureFromSurface(renderer, tempSurface2);
    SDL_FreeSurface(tempSurface2);

    // Source rectangles for both sprites
    SDL_Rect srcRect1 = {0, 0, FRAME_WIDTH, FRAME_HEIGHT};
    SDL_Rect srcRect2 = {0, 0, FRAME_WIDTH, FRAME_HEIGHT};

    // Adjust the destination rectangle to maintain aspect ratio
    // Scale for the ground (player.png)
    float groundScale = MIN((float)SCREEN_HEIGHT * 0.5 / FRAME_HEIGHT,
                          (float)SCREEN_WIDTH * 0.8 / FRAME_WIDTH);

    int groundWidth = FRAME_WIDTH * groundScale;
    int groundHeight = FRAME_HEIGHT * groundScale;

    // Scale for the character (player1.png) - make it a bit smaller
    float characterScale = groundScale * 0.8; // 80% of ground scale

    int characterWidth = FRAME_WIDTH * characterScale;
    int characterHeight = FRAME_HEIGHT * characterScale;

    // Position the ground at the bottom center of the screen
    SDL_Rect destRect2 = { // ground
        SCREEN_WIDTH/2 - groundWidth/2,  // Center horizontally
        SCREEN_HEIGHT - groundHeight,    // Align to bottom
        groundWidth,
        groundHeight
    };

    // Position the character on top of the ground
    SDL_Rect destRect1 = { // character
        SCREEN_WIDTH/2 - characterWidth/2,  // Center horizontally
        destRect2.y - characterHeight/2,    // Position on top of ground
        characterWidth,
        characterHeight
    };

    int currentFrame = 0;
    Uint32 lastFrameTime = SDL_GetTicks();

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
        }

        Uint32 now = SDL_GetTicks();
        if (now - lastFrameTime > ANIM_DELAY) {
            currentFrame = (currentFrame + 1) % FRAME_COUNT;
            srcRect2.x = currentFrame * FRAME_WIDTH;
            srcRect1.x = currentFrame * FRAME_WIDTH;
            lastFrameTime = now;
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Draw both sprites
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for the outline
        SDL_RenderDrawRect(renderer, &destRect2); // Draw around the destination rectangle

        // Draw the ground image (static, no animation)
        SDL_RenderCopy(renderer, spriteSheet2, NULL, &destRect2);

        // Draw the character image
        SDL_RenderCopy(renderer, spriteSheet1, &srcRect1, &destRect1);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS (changed from 62ms)
    }

    // Free both textures
    SDL_DestroyTexture(spriteSheet1);
    SDL_DestroyTexture(spriteSheet2);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}