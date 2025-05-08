#ifndef UPDATE_H
#define UPDATE_H

#include "defs.h"

void shootBullet(Mix_Chunk* gunshot, Mix_Chunk* enemyDeath, Mix_Chunk* hitImpact) {
    currentTime = SDL_GetTicks();
    if (currentTime - lastFireTime >= FIRE_RATE) {
        bullets.push_back({210, FirstCoordinates + 45, true});
        lastFireTime = currentTime;
        playgunshot(gunshot);
    }
}

void updateBullets() {
    for (auto& bullet : bullets) {
        if (bullet.active) {
            bullet.x += SPEEDBULLET;
            if (bullet.x > SCREEN_WIDTH) {
                bullet.active = false;
            }
            for (auto it = activeViruses.begin(); it != activeViruses.end();) {
                int dx = bullet.x - (it->x + it->Size / 2);
                int dy = bullet.y - (it->y + it->Size / 2);
                if (4*(dx * dx + dy * dy) <= it->Size * it->Size) {
                    playhitImpact(hitImpact);
                    it->hp -= damage;
                    bullet.active = false;
                    if (it->hp <= 0) {
                        playenemyDeath(enemyDeath);
                        if (it->Size == 200) {
                            int box = rand() % 3;
                            realtime = SDL_GetTicks();

                            if (box == 0) {
                                damage = 75;
                                RADIUS = 8;
                            }
                            else if (box == 2) {
                                virusspeed = 6;
                            }
                            else{
                                SPEEDBULLET = 55;
                            }
                        }
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

    bullets.erase(
        remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b) { return !b.active; }),
        bullets.end()
    );
}

void updateViruses() {
    for (auto& virus : activeViruses) {
        virus.x -= virusspeed;
        if (virus.x <= 50) {
            gameOver = true;
            return;
        }
    }
}

#endif

