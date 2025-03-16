#include "defs.h"
#include "menu.h"
#include "prepare.h"
#include "graphics.h"
#include "update.h"

void handleEvents(bool& running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP: FirstCoordinates -= SPEED; break;
                case SDLK_DOWN: FirstCoordinates += SPEED; break;
            }
            if (FirstCoordinates > SCREEN_HEIGHT) FirstCoordinates = 30;
            else if (FirstCoordinates < 0) FirstCoordinates = 430;
        }
    }
}

void gameLoop(SDL_Renderer* renderer) {
    bool running = true;
    while (running) {
        handleEvents(running);
        if (!gameOver) {
            shootBullet();
            updateBullets();
            updateViruses();
            spawnVirus();
            renderImage(renderer);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_Color textColor = {255, 0, 0, 255};
            TTF_Font* font = TTF_OpenFont("arial.ttf", 48);
            if (!font) {
                std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
                return;
            }
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Game Over", textColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 200, 100};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_RenderPresent(renderer);
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
            TTF_CloseFont(font);
            SDL_Delay(2000);
            return;
        }

        if(viruses.empty() && activeViruses.empty()){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_Color textColor = {255, 0, 0, 255};
            TTF_Font* font = TTF_OpenFont("arial.ttf", 48);
            if (!font) {
                std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
                return;
            }
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, "You win :^", textColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 200, 100};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_RenderPresent(renderer);
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
            TTF_CloseFont(font);
            SDL_Delay(2000);
            return;
        }

        SDL_Delay(16);
    }
}

int main(int argc, char* argv[]) {
    srand(time(0));
    SDL_Window* window = initSDL();
    SDL_Renderer* renderer = createRenderer(window);
    prepareVirus();
    if (!showMenu(renderer)) {
        cout << "End!";
        quitSDL(window, renderer);
        return 0;
    }
    gameLoop(renderer);
    quitSDL(window, renderer);
    return 0;
}
