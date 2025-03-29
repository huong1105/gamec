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
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
        logErrorAndExit( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }
    Mix_AllocateChannels(40);
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

Mix_Music *loadMusic(const char* path){
    Mix_Music *gMusic = Mix_LoadMUS(path);
    if (gMusic == nullptr) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
            "Could not load music! SDL_mixer Error: %s", Mix_GetError());
    }
    return gMusic;
}

void play(Mix_Music *gMusic){
    if (gMusic == nullptr) return;
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic( gMusic, -1 );
    }
    else if( Mix_PausedMusic() == 1 ) {
        Mix_ResumeMusic();
    }
}

Mix_Chunk* loadSound(const char* path) {
    Mix_Chunk* gChunk = Mix_LoadWAV(path);
    if (gChunk == nullptr) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
            "Could not load sound! SDL_mixer Error: %s", Mix_GetError());
    }
}
void playgunshot(Mix_Chunk* gChunk) {
    if (gChunk != nullptr) {
        int channel = Mix_PlayChannel(-1, gChunk, 0);
        if (channel != -1) {
            Mix_Volume(channel, MIX_MAX_VOLUME / 3);
        }
    }
}

void playhitImpact(Mix_Chunk* gChunk) {
    if (gChunk != nullptr) {
        Mix_PlayChannel( -1, gChunk, 0 );
    }
}

void playenemyDeath(Mix_Chunk* gChunk) {
    if (gChunk != nullptr) {
        Mix_PlayChannel( -1, gChunk, 0 );
    }
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

        SDL_Rect dstRect = {virus.x, virus.y, virus.Size, virus.Size};
        SDL_RenderCopy(renderer, virus.viruses, NULL, &dstRect);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect healthBar = {virus.x + (virus.Size - ((virus.hps * barWidth) / 100))/2, virus.y + 4*virus.Size/5, (virus.hp * barWidth) / 100, barHeight};
        SDL_RenderFillRect(renderer, &healthBar);
    }
}

void renderImage(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Texture* background = loadTexture("background.png", renderer);
    SDL_QueryTexture(background, NULL, NULL, &bgWidth, &bgHeight);

    bgX -= SCROLL_SPEED;
    if (bgX <= -bgWidth) {
        bgX = 0;
    }
    SDL_RenderClear(renderer);
    SDL_Rect srcRect = {0, 0, bgWidth, bgHeight};
    SDL_Rect dstRect1 = {bgX, 0, bgWidth, bgHeight};
    SDL_Rect dstRect2 = {bgX + bgWidth, 0, bgWidth, bgHeight};
    SDL_RenderCopy(renderer, background, &srcRect, &dstRect1);
    SDL_RenderCopy(renderer, background, &srcRect, &dstRect2);
    //SDL_RenderPresent(renderer);
    SDL_Delay(16);

    SDL_Texture* character = loadTexture("character.png", renderer);
    SDL_Rect dstrect = {50, FirstCoordinates, 160, 120};
    SDL_RenderCopy(renderer, character, NULL, &dstrect);

    renderBullets(renderer);
    renderViruses(renderer);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(character);
    SDL_DestroyTexture(background);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer) {
    TTF_Quit();
    Mix_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#endif
