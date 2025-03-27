#include "defs.h"
#include "menu.h"
#include "prepare.h"
#include "graphics.h"
#include "update.h"
#include "end.h"

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
    }
}

int main(int argc, char* argv[]) {
    srand(time(0));
    SDL_Window* window = initSDL();
    SDL_Renderer* renderer = createRenderer(window);
    prepareVirus(renderer);
    Music = loadMusic("dark-black-neon.mp3");
    play(Music);
    if (!showMenu(renderer)) {
        cout << "End!";
        quitSDL(window, renderer);
        return 0;
    }
    gameLoop(renderer);
    quitSDL(window, renderer);
    return 0;
}
