#ifndef PREPARE_H
#define PREPARE_H

#include "defs.h"

void prepareVirus(SDL_Renderer* renderer) {
    SDL_Texture* Virus[] = {loadTexture("1.png", renderer),
                            loadTexture("2.png", renderer),
                            loadTexture("3.png", renderer)};
    int positions[] = {15, 200, 400};
    int hps[] = {100, 200, 300};
    int Size[] = {140, 170, 200};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < VIRUS_COUNT; j++) {
            viruses.push_back({SCREEN_WIDTH - 50, positions[rand() % 3], Size[i], hps[i], hps[i], Virus[i]});
        }
    }
}

void spawnVirus() {
    int currentTime = SDL_GetTicks();
    if (currentTime - lastVirusSpawnTime >= 1700 && !viruses.empty()) {
        lastVirusSpawnTime = currentTime;
        int index = rand() % viruses.size();
        activeViruses.push_back(viruses[index]);
        viruses.erase(viruses.begin() + index);
    }
}

#endif
