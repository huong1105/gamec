#ifndef PREPARE_H
#define PREPARE_H

#include "defs.h"

void prepareVirus() {
    SDL_Color colors[] = {{255, 0, 0, 255}, {0, 255, 0, 255}, {0, 0, 255, 255}};
    int sizes[] = {40, 50, 60};
    int hps[] = {100, 200, 300};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < VIRUS_COUNT; j++) {
            viruses.push_back({SCREEN_WIDTH - 50, positions[rand() % 3], sizes[i], hps[i], colors[i]});
        }
    }
}

void spawnVirus() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastVirusSpawnTime >= 2000 && !viruses.empty()) {
        lastVirusSpawnTime = currentTime;
        int index = rand() % viruses.size();
        activeViruses.push_back(viruses[index]);
        viruses.erase(viruses.begin() + index);
    }
}

#endif
