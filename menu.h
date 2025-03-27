#ifndef MENU_H
#define MENU_H
#include "defs.h"
#include "graphics.h"

bool showMenu(SDL_Renderer* renderer) {
    SDL_Texture* menuTexture = loadTexture("menu.png", renderer);
    SDL_Texture* menu1Texture = loadTexture("menu1.jpg", renderer);
    SDL_Texture* menu2Texture = loadTexture("menu2.jpg", renderer);
    if (!menuTexture) return false;

    SDL_Event e;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) return false;
            if (e.type == SDL_MOUSEMOTION){
                int x = e.motion.x, y = e.motion.y;
                if (x > 310 && x < 540 && y > 170 && y < 300){
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, menu1Texture, NULL, NULL);
                    SDL_RenderPresent(renderer);

                }
                else{
                    if(x > 650 && x < 920 && y > 170 && y < 300){
                        SDL_RenderClear(renderer);
                        SDL_RenderCopy(renderer, menu2Texture, NULL, NULL);
                        SDL_RenderPresent(renderer);
                    }
                    else{
                        SDL_RenderClear(renderer);
                        SDL_RenderCopy(renderer, menuTexture, NULL, NULL);
                        SDL_RenderPresent(renderer);
                    }
                }
                continue;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x, y = e.button.y;
                if (x > 310 && x < 540 && y > 170 && y < 300) running = false;
                if (x > 650 && x < 920 && y > 170 && y < 300) return false;
            }
        }

    }

    SDL_DestroyTexture(menuTexture);
    return true;
}

#endif
