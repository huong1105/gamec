#ifndef DEFS_H
#define DEFS_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>

using namespace std;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Hi, robot";
const int SPEED = 200;
const int RADIUS = 5;
const int SPEEDBULLET = 30;
const int FIRE_RATE = 150;
const int VIRUS_COUNT = 1;
int FirstCoordinates = 30;
int virusspeed = 5;
int damage = 20;
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

#endif
