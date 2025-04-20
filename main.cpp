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
