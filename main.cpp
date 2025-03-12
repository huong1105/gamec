#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <algorithm>
#include <set>
#include <ctime>
using namespace std;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Hi, robot";
const int SPEED = 200;
const int RADIUS = 3;
const int SPEEDBULLET = 30;
const int FIRE_RATE = 150;
const int VIRUS_COUNT = 9;
int FirstCoordinates = 30;
bool gameOver = false;

struct Bullet {
    int x, y;
    bool active;
};

struct Virus {
    int x, y, Size, hp;
    SDL_Color color;
};

vector<Bullet> bullets;
vector<Virus> viruses;
vector<Virus> activeViruses;
vector<int> positions = {60, 260, 460};

Uint32 lastFireTime = 0;
Uint32 lastVirusSpawnTime = 0;

void logErrorAndExit(const char* msg, const char* error) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}

SDL_Window* initSDL(){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        logErrorAndExit("SDL_Init", SDL_GetError());
    }
    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN /* || SDL_WINDOW_FULLSCREEN_DESKTOP */ );

    if(window == nullptr)   logErrorAndExit("CreateWindow", SDL_GetError());
    if (TTF_Init() == -1) {
            logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error: ",
                             TTF_GetError());
        }

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

void quitSDL(SDL_Window* window, SDL_Renderer* renderer) {
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture* loadTexture(const string& path, SDL_Renderer* renderer) {
    SDL_Surface* loadSurface = IMG_Load(path.c_str());
    if (!loadSurface) {
        cout << "Unable to load image " << path << " SDL_image Error: " << IMG_GetError() << endl;
        return nullptr;
    }
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadSurface);
    SDL_FreeSurface(loadSurface);
    return newTexture;
}

void drawPixel(SDL_Renderer* renderer, int x, int y) {
    SDL_RenderDrawPoint(renderer, x, y);
}

void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) drawPixel(renderer, centerX + x, centerY + y);
        }
    }
}

void renderBullets(SDL_Renderer* renderer) {
    for (const auto& bullet : bullets) {
        if (bullet.active) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            drawCircle(renderer, bullet.x, bullet.y, RADIUS);
        }
    }
}

void shootBullet() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastFireTime >= FIRE_RATE) {
        bullets.push_back({210, FirstCoordinates + 45, true});
        lastFireTime = currentTime;
    }
}

void updateViruses() {
    for (auto& virus : activeViruses) {
        virus.x -= 2;
        if (virus.x <= 50) {
            gameOver = true;
            return;
        }
    }
}

void renderViruses(SDL_Renderer* renderer) {
    for (const auto& virus : activeViruses) {
        SDL_SetRenderDrawColor(renderer, virus.color.r, virus.color.g, virus.color.b, 255);
        drawCircle(renderer, virus.x, virus.y, virus.Size);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect healthBar = {virus.x - virus.Size, virus.y + virus.Size + 5, (virus.hp * virus.Size) / 100, 5};
        SDL_RenderFillRect(renderer, &healthBar);
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

    for (auto& bullet : bullets) {
        if (bullet.active) {
            for (auto it = activeViruses.begin(); it != activeViruses.end();) {
                int dx = bullet.x - it->x;
                int dy = bullet.y - it->y;
                if (dx * dx + dy * dy <= it->Size * it->Size) {
                    it->hp -= 20;
                    bullet.active = false;
                    if (it->hp <= 0) {
                        it = activeViruses.erase(it);
                        if (activeViruses.empty()) return;
                    }
                    else {
                        ++it;
                    }
                    break;
                }
                else {
                    ++it;
                }
            }
        }
    }

    bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return !b.active; }), bullets.end());
}


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

void renderImage(SDL_Renderer* renderer) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Texture* background = loadTexture("character_cropped_preview_rev_1.png", renderer);
    SDL_Rect dstrect = {50, FirstCoordinates, 160, 120};
    SDL_RenderCopy(renderer, background, NULL, &dstrect);
    renderBullets(renderer);
    renderViruses(renderer);
    SDL_RenderPresent(renderer);
}

void prepareVirus() {

    SDL_Color colors[] = {{255, 0, 0, 255}, {0, 255, 0, 255}, {0, 0, 255, 255}};
    int sizes[] = {40, 50, 60};
    int hps[] = {100, 200, 300};

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < VIRUS_COUNT; j++){
            viruses.push_back({ SCREEN_WIDTH - 50, positions[rand() % 3], sizes[i], hps[i], colors[i] });
        }
    }
}

void spawnVirus() {
    Uint32 currentTime = SDL_GetTicks();

    if (currentTime - lastVirusSpawnTime >= 2000) { // Mỗi 2 giây
        lastVirusSpawnTime = currentTime;

        if (!viruses.empty()) {
            int index = rand() % viruses.size();
            Virus virus = viruses[index];


            viruses.erase(viruses.begin() + index);

            activeViruses.push_back(virus);
        }
    }
}

bool showMenu(SDL_Renderer* renderer) {
    SDL_Texture* menuTexture = loadTexture("menu.png", renderer);
    if (!menuTexture) return false;

    SDL_Event e;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) return false;

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x, y = e.button.y;

                if (x > 310 && x < 540 && y > 170 && y < 250) {
                    running = false;
                }

                if (x > 650 && x < 920 && y > 170 && y < 250) {
                    return false;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, menuTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(menuTexture);
    return true;
}

void gameLoop(SDL_Renderer* renderer) {
    bool running = true;
    while (running) {
        handleEvents(running);

        if (!gameOver) {
            shootBullet();
            updateBullets();
            updateViruses();
            spawnVirus();
            renderImage(renderer);
        } else {
            // Hiển thị màn hình "Game Over"
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Tạo thông báo "Game Over"
            SDL_Color textColor = {255, 0, 0, 255}; // Màu đỏ
            TTF_Font* font = TTF_OpenFont("arial.ttf", 48);
            if (!font) {
                cout << "Failed to load font: " << TTF_GetError() << endl;
                return;
            }
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Game Over", textColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 200, 100};

            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_RenderPresent(renderer);

            // Giải phóng bộ nhớ
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
            TTF_CloseFont(font);

            SDL_Delay(2000); // Hiển thị "Game Over" trong 2 giây
            return; // Dừng game ngay lập tức
        }

        SDL_Delay(16);
    }
}


int main(int argc, char* argv[]) {
    srand(time(0));
    SDL_Window* window = initSDL();
    SDL_Renderer* renderer = createRenderer(window);
    prepareVirus();
    if (!showMenu(renderer)) {
        cout << "End!";
        quitSDL(window, renderer);
    }
    gameLoop(renderer);
    quitSDL(window, renderer);
    return 0;
}
