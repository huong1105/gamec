#ifndef END_H
#define END_H

#include "defs.h"

bool win(vector<Virus> &viruses, vector<Virus> &activeViruses, SDL_Renderer* renderer){
    if(viruses.empty() && activeViruses.empty()){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Color textColor = {255, 0, 0, 255};
        TTF_Font* font = TTF_OpenFont("arial.ttf", 48);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, "You win :^", textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 200, 100};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_RenderPresent(renderer);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
        TTF_CloseFont(font);
        SDL_Delay(2000);
        return true;
    }
}

void lose(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Color textColor = {255, 0, 0, 255};
    TTF_Font* font = TTF_OpenFont("arial.ttf", 48);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Game Over", textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 200, 100};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_RenderPresent(renderer);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_Delay(2000);
}

#endif
