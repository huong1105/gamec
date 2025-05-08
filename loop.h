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
                    case SDLK_UP:
                        selectedOption = (selectedOption - 1 + 5) % 5;
                        break;
                    case SDLK_DOWN:
                        selectedOption = (selectedOption + 1) % 5;
                        break;
                    case SDLK_RIGHT:
                        if (selectedOption == 1) fps = min(60, fps + 15);
                        else if (selectedOption == 2){
                            volume = min(150, volume + 10);
                            Mix_Volume(-1, mute ? 0 : volume);
                            Mix_VolumeMusic(volume);
                        }
                        break;
                    case SDLK_LEFT:
                        if (selectedOption == 1) fps = max(15, fps - 15);
                        else if (selectedOption == 2){
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
                        }
                        if (selectedOption == 0) run = false;
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

        SDL_Color continueColor = (selectedOption == 0) ? yellow : white;
        tex = renderText("Continue ", continueColor, renderer);
        if (tex)    out(renderer, tex, 150);

        SDL_Color fpsColor = (selectedOption == 1) ? yellow : white;
        tex = renderText("FPS: " + to_string(fps), fpsColor, renderer);
        if (tex)    out(renderer, tex, 200);

        SDL_Color volumeColor = (selectedOption == 2) ? yellow : white;
        tex = renderText("Volume: " + to_string(mute ? 0 : volume), volumeColor, renderer);
        if (tex)    out(renderer, tex, 250);

        SDL_Color muteColor = (selectedOption == 3) ? yellow : white;
        tex = renderText("Mute: " + string(mute ? "ON" : "OFF"), muteColor, renderer);
        if (tex)    out(renderer, tex, 300);

        SDL_Color exitColor = (selectedOption == 4) ? yellow : white;
        tex = renderText("Exit", exitColor, renderer);
        if (tex)    out(renderer, tex, 350);
        SDL_RenderPresent(renderer);
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
    while (running) {
        if (!gameOver) {
            if(win(viruses, activeViruses, renderer)){
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
            spawnVirus();
            renderImage(renderer);

        }
        else {
            lose(renderer);
            return;
        }
        SDL_Delay(1000.0/fps);
    }
}

#endif
