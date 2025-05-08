#ifndef END_H
#define END_H

#include "defs.h"
void showMessage(const string& message, SDL_Color color, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Rect textRect;
    SDL_Texture* textTexture = renderText(message, color, renderer);

    if (textTexture) {
        SDL_QueryTexture(textTexture, nullptr, nullptr, &textRect.w, &textRect.h);

        textRect.x = SCREEN_WIDTH / 2 - textRect.w / 2;
        textRect.y = SCREEN_HEIGHT / 2 - textRect.h / 2;

        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(textTexture);
    }
    SDL_Delay(2000);
}


bool win(vector<Virus>& viruses, vector<Virus>& activeViruses, SDL_Renderer* renderer) {
    if (viruses.empty() && activeViruses.empty()) {
        SDL_Color red = {255, 0, 0, 255};
        showMessage("You win :^", red, renderer);
        return true;
    }
    return false;
}

void lose(SDL_Renderer* renderer) {
    SDL_Color red = {255, 0, 0, 255};
    showMessage("Game Over", red, renderer);
}
#endif
