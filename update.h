#ifndef UPDATE_H
#define UPDATE_H

#include "defs.h"

void shootBullet() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastFireTime >= FIRE_RATE) {
        bullets.push_back({210, FirstCoordinates + 45, true});
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

    for (auto& bullet : bullets) {
        if (bullet.active) {
            for (auto it = activeViruses.begin(); it != activeViruses.end();) {
                int dx = bullet.x - it->x;
                int dy = bullet.y - it->y;
                if (dx * dx + dy * dy <= it->Size * it->Size) {
                    it->hp -= damage;
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

