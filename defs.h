#ifndef DEFS_H
#define DEFS_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>

using namespace std;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Hi, robot";
const int SPEED = 200;
const int FIRE_RATE = 300;
const int SCROLL_SPEED = 2;
int SPEEDBULLET = 40;
int RADIUS = 5;
int VIRUS_COUNT = 5;
int FirstCoordinates = 30;
int virusspeed = 10;
int damage = 50;
int bgX = 0;
int bgWidth, bgHeight;
int barWidth = 50;
int barHeight = 5;
bool gameOver = false;
int selected = 0;
int fps = 60;
int volume = 60;

struct Bullet {
    int x, y;
    bool active;
};

struct Virus {
    int x, y, Size, hp;
    int hps;
    SDL_Texture* viruses;
};

vector<Bullet> bullets;
vector<Virus> viruses;
vector<Virus> activeViruses;

unsigned int lastFireTime = 0;
unsigned int lastVirusSpawnTime = 0;
unsigned int currentTime = 0;
unsigned int realtime = 0;

SDL_Texture* background = nullptr;
SDL_Texture* character = nullptr;
SDL_Texture* menu = nullptr;
SDL_Texture* menu1 = nullptr;
SDL_Texture* menu2 = nullptr;

Mix_Music* Music = nullptr;
Mix_Chunk* gunshot = nullptr;
Mix_Chunk* hitImpact = nullptr;
Mix_Chunk* enemyDeath = nullptr;

#endif
