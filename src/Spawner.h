#pragma once

#include <vector>
#include <memory>

#include "entities/Enemy.h"

class Spawner
{
public:
    Spawner();

    void Update(
        float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        float worldWidth,
        float worldHeight
    );

    void Reset();

    [[nodiscard]] int GetWave() const;
    [[nodiscard]] bool ShouldShowWaveText() const;

private:
    int wave{};
    bool waveActive{false};

    bool showWaveText{false};
    bool waveTextVisible{false};

    float blinkTimer{};
    int blinkCount{};
    int maxBlinks{6};

    void StartWave(
        std::vector<std::unique_ptr<Enemy>>& enemies,
        float worldWidth,
        float worldHeight
    );
};
