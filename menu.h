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
                    case SDLK_UP: selectedOption = (selectedOption - 1 + 4) % 4; break;
                    case SDLK_DOWN: selectedOption = (selectedOption + 1) % 4; break;
                    case SDLK_RIGHT:
                        if (selectedOption == 0) fps = min(60, fps + 15);
                        else if (selectedOption == 1) {
                            volume = min(128, volume + 8);
                            Mix_Volume(-1, mute ? 0 : volume);
                            Mix_VolumeMusic(volume);
                        }
                        break;
                    case SDLK_LEFT:
                        if (selectedOption == 0) fps = max(15, fps - 15);
                        else if (selectedOption == 1) {
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
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                for (int i = 0; i < 4; i++) {
                    string label;
                    if (i == 0) label = "FPS: " + to_string(fps);
                    else if (i == 1) label = "Volume: " + to_string(mute ? 0 : volume);
                    else if (i == 2) label = "Mute: " + string(mute ? "ON" : "OFF");
                    else label = "ESC to exit";

                    SDL_Texture* tmpTex = renderText(label, {255, 255, 255}, renderer);
                    if (!tmpTex) continue;

                    int w, h;
                    SDL_QueryTexture(tmpTex, NULL, NULL, &w, &h);
                    SDL_Rect box = {SCREEN_WIDTH / 2 - w / 2, 150 + i * 50, w, h};
                    SDL_Point p = {mouseX, mouseY};

                    if (SDL_PointInRect(&p, &box)) {
                        selectedOption = i;
                        SDL_DestroyTexture(tmpTex);
                        if (i == 2) {
                            mute = !mute;
                            Mix_Volume(-1, mute ? 0 : volume);
                            Mix_VolumeMusic(mute ? 0 : volume);
                        }
                        if (i == 3) running = false;
                        break;
                    }
                    SDL_DestroyTexture(tmpTex);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect dstrect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, menu, NULL, &dstrect);

        SDL_Texture* titleTex = renderText("OPTIONS", {255, 255, 255}, renderer);
        if (titleTex) out(renderer, titleTex, 50);

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Point mousePoint = {mouseX, mouseY};

        for (int i = 0; i < 4; i++) {
            string label;
            if (i == 0) label = "FPS: " + to_string(fps);
            else if (i == 1) label = "Volume: " + to_string(mute ? 0 : volume);
            else if (i == 2) label = "Mute: " + string(mute ? "ON" : "OFF");
            else label = "ESC to exit";

            SDL_Texture* tex = renderText(label, {255, 255, 255}, renderer);
            if (!tex) continue;

            int texW, texH;
            SDL_QueryTexture(tex, nullptr, nullptr, &texW, &texH);
            SDL_Rect dst = {SCREEN_WIDTH / 2 - texW / 2, 150 + i * 50, texW, texH};

            if (SDL_PointInRect(&mousePoint, &dst)) {
                selectedOption = i;
            }

            SDL_Color color = (i == selectedOption) ? SDL_Color{255, 0, 0} : SDL_Color{255, 255, 255};
            SDL_DestroyTexture(tex);
            tex = renderText(label, color, renderer);

            if (i == selectedOption) {
                SDL_Rect box = {dst.x - 10, dst.y - 5, dst.w + 20, dst.h + 10};
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderFillRect(renderer, &box);
                SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);
                SDL_RenderDrawRect(renderer, &box);
            }

            SDL_RenderCopy(renderer, tex, nullptr, &dst);
            SDL_DestroyTexture(tex);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1000.0 / fps);
    }
}

bool showMenu(SDL_Renderer* renderer) {
    vector<string> menuItems = {"Start Game", "Options", "Exit"};
    int selected = 0;
    bool quit = false;
    SDL_Event e;

    menu = loadTexture("hinhnen.jfif", renderer);

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP: selected = (selected - 1 + menuItems.size()) % menuItems.size(); break;
                    case SDLK_DOWN: selected = (selected + 1) % menuItems.size(); break;
                    case SDLK_RETURN:
                        if (menuItems[selected] == "Exit") quit = true;
                        else if (menuItems[selected] == "Start Game") return true;
                        else options(renderer);
                        break;
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                for (int i = 0; i < menuItems.size(); ++i) {
                    SDL_Texture* tmpTex = renderText(menuItems[i], {255, 255, 255}, renderer);
                    if (!tmpTex) continue;
                    int w, h;
                    SDL_QueryTexture(tmpTex, nullptr, nullptr, &w, &h);
                    SDL_Rect box = {SCREEN_WIDTH / 2 - w / 2, 150 + i * 60, w, h};
                    SDL_Point p = {mouseX, mouseY};
                    if (SDL_PointInRect(&p, &box)) {
                        selected = i;
                        SDL_DestroyTexture(tmpTex);
                        if (menuItems[i] == "Exit") quit = true;
                        else if (menuItems[i] == "Start Game") return true;
                        else options(renderer);
                        break;
                    }
                    SDL_DestroyTexture(tmpTex);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect dstrect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, menu, NULL, &dstrect);

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Point mousePoint = {mouseX, mouseY};

        for (int i = 0; i < menuItems.size(); i++) {
            SDL_Texture* textTex = renderText(menuItems[i], {255, 255, 255}, renderer);
            if (!textTex) continue;

            int texW, texH;
            SDL_QueryTexture(textTex, nullptr, nullptr, &texW, &texH);
            SDL_Rect dstRect = {SCREEN_WIDTH / 2 - texW / 2, 150 + i * 60, texW, texH};

            if (SDL_PointInRect(&mousePoint, &dstRect)) {
                selected = i;
            }

            SDL_Color color = (i == selected) ? SDL_Color{255, 0, 0} : SDL_Color{255, 255, 255};
            SDL_DestroyTexture(textTex);
            textTex = renderText(menuItems[i], color, renderer);

            if (i == selected) {
                SDL_Rect box = {dstRect.x - 10, dstRect.y - 5, dstRect.w + 20, dstRect.h + 10};
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderFillRect(renderer, &box);
                SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);
                SDL_RenderDrawRect(renderer, &box);
            }

            SDL_RenderCopy(renderer, textTex, nullptr, &dstRect);
            SDL_DestroyTexture(textTex);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1000.0 / fps);
    }
    SDL_DestroyTexture(menu);
    return false;
}





#endif
