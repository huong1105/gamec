#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <algorithm>
using namespace std;

const int SCREEN_WIDTH = 1800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Hi, robot";
const int SPEED = 200;
const int RADIUS = 3;
const int SPEEDBULLET = 50;
const int FIRE_RATE = 100;
int FirstCoordinates = 30;

struct Bullet{
    int x, y;
    bool active;
};
vector<Bullet> bullets;
Uint32 lastFireTime = 0;

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

SDL_Texture* loadTexture (string path, SDL_Renderer* renderer){
    SDL_Texture* newtexture = nullptr;
    SDL_Surface* loadSurface = IMG_Load(path.c_str());
    if(loadSurface == nullptr){
        cout << "Unable to load image " << path << " SDL_image Error: " << IMG_GetError() << endl;
    }
    else{
        newtexture = SDL_CreateTextureFromSurface(renderer, loadSurface);
        if( newtexture == nullptr){
            cout << "Unable to create texture from " << path << " SDL Error: " << SDL_GetError() << endl;
        }
        SDL_FreeSurface(loadSurface);
    }
    return newtexture;
}

void drawPixel(SDL_Renderer* renderer, int x, int y) {
    SDL_RenderDrawPoint(renderer, x, y);
}

void shootBullet() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastFireTime >= FIRE_RATE) {
        bullets.push_back({ 210, FirstCoordinates + 45, true });
        lastFireTime = currentTime;
    }
}

void updateBullets() {
    for (auto& bullet : bullets) {
        if (bullet.active) {
            bullet.x += SPEEDBULLET;
            if (bullet.x > SCREEN_WIDTH) {
                bullet.active = false;
            }
        }
    }
    bullets.erase(remove_if(bullets.begin(), bullets.end(),[](Bullet& b) { return !b.active; }), bullets.end());
}


void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                drawPixel(renderer, centerX + x, centerY + y);
            }
        }
    }
}

void renderBullets(SDL_Renderer* renderer) {
    for (auto& bullet : bullets) {
        if (bullet.active) {
            if (SDL_GetTicks() % 500 < 250) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Vàng
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); // Cam
            }
            drawCircle(renderer, bullet.x, bullet.y, RADIUS);
        }
    }
}

void renderImage(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Texture* background = loadTexture("character_cropped_preview_rev_1.png", renderer);
    SDL_Rect dstrect = {50, FirstCoordinates, 160, 120};
    SDL_RenderCopy(renderer, background, NULL, &dstrect);
    renderBullets(renderer);

    SDL_RenderPresent(renderer);
}

void updatestatus(SDL_Renderer* renderer){
    SDL_Event event;

    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        FirstCoordinates -= SPEED;
                        break;
                    case SDLK_DOWN:
                        FirstCoordinates += SPEED;
                        break;
                }
                if(FirstCoordinates > SCREEN_HEIGHT){
                    FirstCoordinates = 30;
                }
                else if(FirstCoordinates < 0){
                    FirstCoordinates = 430;
                }
            }
        }
        shootBullet();
        updateBullets();
        renderImage(renderer);
        SDL_Delay(16);
    }
}

int main(int argc, char* agrv[])
{
    SDL_Window* window = initSDL();
    SDL_Renderer* renderer = createRenderer(window);

    updatestatus(renderer);

    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}
