#include "defs.h"
#include "menu.h"
#include "prepare.h"
#include "graphics.h"
#include "update.h"
#include "end.h"
#include "loop.h"

int main(int argc, char* argv[]) {
    srand(time(0));
    SDL_Window* window = initSDL();
    SDL_Renderer* renderer = createRenderer(window);
    Music = loadMusic("dark-black-neon.mp3");
    play(Music);
    while(true){
        if (!showMenu(renderer)) {
            cout << "End!";
            quitSDL(window, renderer);
            return 0;
        }
        prepareVirus(renderer);
        gameLoop(renderer);
        SDL_DestroyTexture(character);
        SDL_DestroyTexture(background);
        Mix_FreeChunk(gunshot);
        Mix_FreeChunk(hitImpact);
        Mix_FreeChunk(enemyDeath);
        SDL_DestroyTexture(explosion);
        viruses.clear();
        bullets.clear();
        activeViruses.clear();
    }
    return 0;
}
