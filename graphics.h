#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "defs.h"

void logErrorAndExit(const char* msg, const char* error) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}

SDL_Window* initSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) logErrorAndExit("SDL_Init", SDL_GetError());
    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) logErrorAndExit("CreateWindow", SDL_GetError());
    if (TTF_Init() == -1) logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error:", TTF_GetError());
    return window;
}

SDL_Renderer* createRenderer(SDL_Window* window) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) logErrorAndExit("CreateRenderer", SDL_GetError());
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    return renderer;
}

SDL_Texture* loadTexture(const string& path, SDL_Renderer* renderer) {
    SDL_Surface* loadSurface = IMG_Load(path.c_str());
    if (!loadSurface) {
        cout << "Unable to load image " << path << " SDL_image Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadSurface);
    SDL_FreeSurface(loadSurface);
    return newTexture;
}

void drawPixel(SDL_Renderer* renderer, int x, int y) {
    SDL_RenderDrawPoint(renderer, x, y);
}

void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) drawPixel(renderer, centerX + x, centerY + y);
        }
    }
}

void renderBullets(SDL_Renderer* renderer) {
    for (const auto& bullet : bullets) {
        if (bullet.active) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            drawCircle(renderer, bullet.x, bullet.y, RADIUS);
        }
    }
}

void renderViruses(SDL_Renderer* renderer) {
    for (const auto& virus : activeViruses) {
        SDL_SetRenderDrawColor(renderer, virus.color.r, virus.color.g, virus.color.b, 255);
        drawCircle(renderer, virus.x, virus.y, virus.Size);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect healthBar = {virus.x - virus.Size, virus.y + virus.Size + 5, (virus.hp * virus.Size) / 100, 5};
        SDL_RenderFillRect(renderer, &healthBar);
    }
}

void renderImage(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Texture* background = loadTexture("background.png", renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);

    SDL_Texture* character = loadTexture("character.png", renderer);
    SDL_Rect dstrect = {50, FirstCoordinates, 160, 120};
    SDL_RenderCopy(renderer, character, NULL, &dstrect);

    renderBullets(renderer);
    renderViruses(renderer);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(character);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer) {
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#endif
