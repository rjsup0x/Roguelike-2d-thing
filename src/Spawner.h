#pragma once

#include <vector>
#include "entities/Enemy.h"
#include <raylib.h>

class Spawner
{
public:
    Spawner();

    void Update(float dt, std::vector<Enemy>& enemies, float worldWidth, float worldHeight);

    int GetWave() const;
    bool ShouldShowWaveText() const;

private:
    int wave = 0;
    bool waveActive = false;

    // wave text system
    bool showWaveText = false;
    bool waveTextVisible = false;

    float blinkTimer = 0.0f;
    int blinkCount = 0;
    int maxBlinks = 6;

    void StartWave(std::vector<Enemy>& enemies, float worldWidth, float worldHeight);
};
