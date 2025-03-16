#ifndef MENU_H
#define MENU_H
#include "defs.h"
#include "graphics.h"

bool showMenu(SDL_Renderer* renderer) {
    SDL_Texture* menuTexture = loadTexture("menu.png", renderer);
    if (!menuTexture) return false;

    SDL_Event e;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) return false;
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x, y = e.button.y;
                if (x > 310 && x < 540 && y > 170 && y < 250) running = false;
                if (x > 650 && x < 920 && y > 170 && y < 250) return false;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, menuTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(menuTexture);
    return true;
}

#endif
