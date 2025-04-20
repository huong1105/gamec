#ifndef LOOP_H
#define LOOP_H

#include "defs.h"
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
            handleEvents(running);

            shootBullet(gunshot,enemyDeath, hitImpact);
            updateBullets();
            updateViruses();
            spawnVirus();
            renderImage(renderer);

        }
        else {
            lose(renderer);
            return;
        }
        SDL_Delay(16);
    }
}

#endif
