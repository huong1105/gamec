#ifndef MENU_H
#define MENU_H
#include "defs.h"
#include "graphics.h"

void out(SDL_Renderer* renderer, SDL_Texture* tex, int y){
    int texW = 0;
    int texH = 0;
    SDL_Rect dst;
    SDL_QueryTexture(tex, nullptr, nullptr, &texW, &texH);
    dst.x = SCREEN_WIDTH / 2 - texW / 2;
    dst.y = y;
    dst.w = texW;
    dst.h = texH;
    SDL_RenderCopy(renderer, tex, nullptr, &dst);
    SDL_DestroyTexture(tex);
}

void options(SDL_Renderer* renderer) {
    bool mute = false;
    bool running = true;
    int selectedOption = 0;

    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE: running = false; break;
                    case SDLK_UP:
                        selectedOption = (selectedOption - 1 + 4) % 4;
                        break;
                    case SDLK_DOWN:
                        selectedOption = (selectedOption + 1) % 4;
                        break;
                    case SDLK_RIGHT:
                        if (selectedOption == 0) fps = min(60, fps + 15);
                        else if (selectedOption == 1){
                            volume = min(128, volume + 8);
                            Mix_Volume(-1, mute ? 0 : volume);
                            Mix_VolumeMusic(volume);
                        }
                        break;
                    case SDLK_LEFT:
                        if (selectedOption == 0) fps = max(15, fps - 15);
                        else if (selectedOption == 1){
                            volume = max(0, volume - 8);
                            Mix_Volume(-1, mute ? 0 : volume);
                            Mix_VolumeMusic(volume);
                        }
                        break;
                    case SDLK_RETURN:
                        if (selectedOption == 2) {
                            mute = !mute;
                            Mix_Volume(-1, mute ? 0 : volume);
                            Mix_VolumeMusic(mute ? 0 : volume);
                        }
                        if (selectedOption == 3) running = false;
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Color white = {255, 255, 255};
        SDL_Color yellow = {255, 255, 0};
        SDL_Rect dst;
        SDL_Texture* tex = nullptr;
        int texW = 0;
        int texH = 0;

        tex = renderText("OPTIONS", white, renderer);
        if (tex)    out(renderer, tex, 50);

        SDL_Color fpsColor = (selectedOption == 0) ? yellow : white;
        tex = renderText("FPS: " + to_string(fps), fpsColor, renderer);
        if (tex)    out(renderer, tex, 150);

        SDL_Color volumeColor = (selectedOption == 1) ? yellow : white;
        tex = renderText("Volume: " + to_string(mute ? 0 : volume), volumeColor, renderer);
        if (tex)    out(renderer, tex, 200);

        SDL_Color muteColor = (selectedOption == 2) ? yellow : white;
        tex = renderText("Mute: " + string(mute ? "ON" : "OFF"), muteColor, renderer);
        if (tex)    out(renderer, tex, 250);

        SDL_Color exitColor = (selectedOption == 3) ? yellow : white;
        tex = renderText("ESC to exit", exitColor, renderer);
        if (tex)    out(renderer, tex, 300);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000.0/fps);
    }
}


bool showMenu(SDL_Renderer* renderer) {
    vector<string> menuItems = {"Start Game", "Options", "Exit"};
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        selected = (selected - 1 + menuItems.size()) % menuItems.size();
                        break;
                    case SDLK_DOWN:
                        selected = (selected + 1) % menuItems.size();
                        break;
                    case SDLK_RETURN:
                        if (menuItems[selected] == "Exit") quit = true;
                        else
                            if (menuItems[selected] == "Start Game") return true;
                            else options(renderer);
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < menuItems.size(); i++) {
            SDL_Color color = (i == selected) ? SDL_Color{255, 255, 0} : SDL_Color{255, 255, 255};
            SDL_Texture* textTex = renderText(menuItems[i], color, renderer);
            if (textTex) {
                int texW, texH;
                SDL_QueryTexture(textTex, nullptr, nullptr, &texW, &texH);
                SDL_Rect dstRect = {SCREEN_WIDTH / 2 - texW / 2, 150 + i * 60, texW, texH};
                SDL_RenderCopy(renderer, textTex, nullptr, &dstRect);
                SDL_DestroyTexture(textTex);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1000.0/fps);
    }

    return false;
}

#endif
