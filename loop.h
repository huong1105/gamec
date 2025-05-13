#ifndef LOOP_H
#define LOOP_H

#include "defs.h"

void options(SDL_Renderer* renderer, bool& running) {
    bool mute = false;
    bool run = true;
    int selectedOption = 0;

    SDL_Event e;
    while (run) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) run = false;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE: run = false; break;
                    case SDLK_UP: selectedOption = (selectedOption - 1 + 5) % 5; break;
                    case SDLK_DOWN: selectedOption = (selectedOption + 1) % 5; break;
                    case SDLK_RIGHT:
                        if (selectedOption == 1) fps = min(60, fps + 15);
                        else if (selectedOption == 2) {
                            volume = min(150, volume + 10);
                            Mix_Volume(-1, mute ? 0 : volume);
                            Mix_VolumeMusic(volume);
                        }
                        break;
                    case SDLK_LEFT:
                        if (selectedOption == 1) fps = max(15, fps - 15);
                        else if (selectedOption == 2) {
                            volume = max(0, volume - 10);
                            Mix_Volume(-1, mute ? 0 : volume);
                            Mix_VolumeMusic(volume);
                        }
                        break;
                    case SDLK_RETURN:
                        if (selectedOption == 3) {
                            mute = !mute;
                            Mix_Volume(-1, mute ? 0 : volume);
                            Mix_VolumeMusic(mute ? 0 : volume);
                        }
                        if (selectedOption == 4) {
                            running = false;
                            run = false;
                            gameOver = false;
                        }
                        if (selectedOption == 0) run = false;
                        break;
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                SDL_Point p = {mouseX, mouseY};
                for (int i = 0; i < 5; ++i) {
                    string label;
                    if (i == 0) label = "Continue";
                    else if (i == 1) label = "FPS: " + to_string(fps);
                    else if (i == 2) label = "Volume: " + to_string(mute ? 0 : volume);
                    else if (i == 3) label = "Mute: " + string(mute ? "ON" : "OFF");
                    else label = "Exit";

                    SDL_Texture* tmpTex = renderText(label, {255, 255, 255}, renderer);
                    if (!tmpTex) continue;
                    int w, h;
                    SDL_QueryTexture(tmpTex, nullptr, nullptr, &w, &h);
                    SDL_Rect box = {SCREEN_WIDTH / 2 - w / 2, 150 + i * 50, w, h};

                    if (SDL_PointInRect(&p, &box)) {
                        selectedOption = i;
                        if (i == 3) {
                            mute = !mute;
                            Mix_Volume(-1, mute ? 0 : volume);
                            Mix_VolumeMusic(mute ? 0 : volume);
                        } else if (i == 4) {
                            running = false;
                            run = false;
                            gameOver = false;
                        } else if (i == 0) run = false;
                    }

                    SDL_DestroyTexture(tmpTex);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect dstrect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, menu, NULL, &dstrect);

        SDL_Texture* tex = renderText("OPTIONS", {255, 255, 255}, renderer);
        if (tex) out(renderer, tex, 50);

        vector<string> labels = {
            "Continue",
            "FPS: " + to_string(fps),
            "Volume: " + to_string(mute ? 0 : volume),
            "Mute: " + string(mute ? "ON" : "OFF"),
            "Exit"
        };

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Point mousePoint = {mouseX, mouseY};

        for (int i = 0; i < labels.size(); ++i) {
            SDL_Texture* textTex = renderText(labels[i], {255, 255, 255}, renderer);
            if (!textTex) continue;

            int texW, texH;
            SDL_QueryTexture(textTex, nullptr, nullptr, &texW, &texH);
            SDL_Rect dst = {SCREEN_WIDTH / 2 - texW / 2, 150 + i * 50, texW, texH};

            if (SDL_PointInRect(&mousePoint, &dst)) selectedOption = i;

            SDL_Color textColor = (i == selectedOption) ? SDL_Color{255, 0, 0} : SDL_Color{255, 255, 255};
            SDL_DestroyTexture(textTex);
            textTex = renderText(labels[i], textColor, renderer);

            if (i == selectedOption) {
                SDL_Rect box = {dst.x - 10, dst.y - 5, dst.w + 20, dst.h + 10};
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderFillRect(renderer, &box);
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderDrawRect(renderer, &box);
            }

            SDL_RenderCopy(renderer, textTex, nullptr, &dst);
            SDL_DestroyTexture(textTex);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1000.0 / fps);
    }
}


void handleEvents(SDL_Renderer* renderer, bool& running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP: FirstCoordinates -= SPEED; break;
                case SDLK_DOWN: FirstCoordinates += SPEED; break;
                case SDLK_SPACE: options(renderer,running);
            }
            if (FirstCoordinates > SCREEN_HEIGHT) FirstCoordinates = 30;
            else if (FirstCoordinates < 0) FirstCoordinates = 430;
        }
    }
}

void gameLoop(SDL_Renderer* renderer) {
    bool running = true;
    gunshot = loadSound("gunshot.mp3");
    hitImpact = loadSound("hitImpact.mp3");
    enemyDeath = loadSound("enemyDeath.mp3");
    character = loadTexture("character.png", renderer);
    background = loadTexture("background.png", renderer);
    explosion = loadTexture("explosion.png", renderer);
    arrow = loadTexture("arrow.png", renderer);
    while (running) {
        if (!gameOver) {
            if(win(viruses, activeViruses, renderer)){
                gameOver = false;
                running = false;
            }
            handleEvents(renderer, running);

            shootBullet(gunshot,enemyDeath, hitImpact);
            updateBullets();
            if (SDL_GetTicks() - realtime >= 2000) {
                damage = 50;
                RADIUS = 5;
                virusspeed = 10;
                SPEEDBULLET = 40;
            }
            updateViruses();
            updateExplosions();
            spawnVirus();
            renderImage(renderer);

        }
        else {
            lose(renderer);
            gameOver = false;
            return;
        }
        SDL_Delay(1000.0/fps);
    }
}

#endif
