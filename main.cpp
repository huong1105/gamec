#include <iostream>
#include <SDL.h>
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Hi, robot";

void waitUntilKeyPressed(){
    SDL_Event event;
    while(true){
        if(SDL_PollEvent(&event) != 0 && (event.type == SDL_KEYDOWN || event.type == SDL_QUIT)){
            return;
        }
        SDL_Delay(100);
    }
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void logErrorAndExit(const char* msg, const char* error){
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}

SDL_Window* initSDL(){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        logErrorAndExit("SDL_Init", SDL_GetError());
    }
    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN /* || SDL_WINDOW_FULLSCREEN_DESKTOP */ );

    if(window == nullptr)   logErrorAndExit("CreateWindow", SDL_GetError());
    return window;
}

SDL_Renderer* createRenderer(SDL_Window* window){
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

    if(renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    return renderer;
}

int main(int argc, char* agrv[])
{
    SDL_Window* window = initSDL();
    SDL_Renderer* renderer = createRenderer(window);
    SDL_RenderPresent(renderer);


    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}
