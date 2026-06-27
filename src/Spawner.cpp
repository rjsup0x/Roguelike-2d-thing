#include "Spawner.h"
#include "entities/BatEnemy.h"

#include <iterator> // std::size

namespace
{
    struct WaveConfig
    {
        int enemyCount;
        int health;
        float speed;
    };

    // Wave configs, in order. The last entry is reused for any wave beyond
    // the table's length, matching the previous "else" fallback behavior
    // (wave 4+ used enemyCount=20, health=120, speed=130.0f).
    // Previously this was an if/else-if ladder in StartWave() — adding a
    // new wave meant adding another branch. Now it's just another row.
    constexpr WaveConfig kWaveConfigs[] = {
        {10, 50,  90.0f},  // wave 1
        {10, 80,  110.0f}, // wave 2
        {15, 100, 130.0f}, // wave 3
        {20, 120, 130.0f}, // wave 4+ (fallback)
    };

    constexpr int kWaveConfigCount = static_cast<int>(std::size(kWaveConfigs));

    const WaveConfig& GetWaveConfig(int wave)
    {
        // wave is 1-based; clamp to the last entry for any wave beyond
        // the table (same fallback behavior as the original else branch).
        int index = wave - 1;

        if (index < 0)
            index = 0;

        if (index >= kWaveConfigCount)
            index = kWaveConfigCount - 1;

        return kWaveConfigs[index];
    }
}

Spawner::Spawner()
{
    wave = 0;
    waveActive = false;
    showWaveText = false;
    waveTextVisible = false;
    blinkTimer = 0.0f;
    blinkCount = 0;
    maxBlinks = 6;
}

int Spawner::GetWave() const
{
    return wave;
}

bool Spawner::ShouldShowWaveText() const
{
    return showWaveText && waveTextVisible;
}

void Spawner::Update(
    float dt,
    std::vector<std::unique_ptr<Enemy>>& enemies,
    float worldWidth,
    float worldHeight)
{
    if (showWaveText)
    {
        blinkTimer -= dt;

        if (blinkTimer <= 0.0f)
        {
            blinkTimer = 0.25f;
            waveTextVisible = !waveTextVisible;
            blinkCount++;

            if (blinkCount >= maxBlinks)
            {
                showWaveText = false;
                waveTextVisible = false;
            }
        }
    }

    if (!waveActive)
    {
        if (enemies.empty())
        {
            StartWave(enemies, worldWidth, worldHeight);
        }
        return;
    }

    if (enemies.empty())
        waveActive = false;
}

void Spawner::StartWave(
    std::vector<std::unique_ptr<Enemy>>& enemies,
    float worldWidth,
    float worldHeight)
{
    wave++;
    waveActive = true;

    showWaveText = true;
    waveTextVisible = true;
    blinkTimer = 0.3f;
    blinkCount = 0;

    const WaveConfig& config = GetWaveConfig(wave);

    for (int i = 0; i < config.enemyCount; i++)
    {
        int side = GetRandomValue(0, 3);

        Vector2 pos;

        if (side == 0)
            pos = { static_cast<float>(GetRandomValue(0, static_cast<int>(worldWidth))), -32 };
        else if (side == 1)
            pos = { static_cast<float>(GetRandomValue(0, static_cast<int>(worldWidth))), worldHeight + 32 };
        else if (side == 2)
            pos = { -32, static_cast<float>(GetRandomValue(0, static_cast<int>(worldHeight))) };
        else
            pos = { worldWidth + 32, static_cast<float>(GetRandomValue(0, static_cast<int>(worldHeight))) };

        // init enemies
        auto enemy = std::make_unique<BatEnemy>(pos);
        // set the enemies stats
        enemy->SetStats(config.health, config.speed);

        // move enemies into the array
        enemies.push_back(std::move(enemy));
    }
}

void Spawner::Reset()
{
    wave = 0;
    waveActive = false;

    showWaveText = false;
    waveTextVisible = false;

    blinkTimer = 0.0f;
    blinkCount = 0;
}
