#include "Spawner.h"
#include "entities/BatEnemy.h"

#include <iterator> // std::size

#include "entities/TreeEnemy.h"

namespace EnemySetup
{
    struct WaveConfig
    {
        int enemyCount;
        int health;
        float speed;
    };

    // determine the types of enemies possible
    enum class EnemyType
    {
        Bat,
        Tree
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

    constexpr int kWaveConfigCount = std::size(kWaveConfigs);

    const WaveConfig& GetWaveConfig(const int wave)
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
    const float deltaTime,
    std::vector<std::unique_ptr<Enemy>>& enemies,
    const float worldWidth,
    const float worldHeight)
{
    if (showWaveText)
    {
        blinkTimer -= deltaTime;

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
    const float worldWidth,
    const float worldHeight)
{
    wave++;
    waveActive = true;

    showWaveText = true;
    waveTextVisible = true;
    blinkTimer = 0.3f;
    blinkCount = 0;

    // configure wave stats for enemy
    const auto&[enemyCount, health, speed] = EnemySetup::GetWaveConfig(wave);

    // for all enemies
    for (int i = 0; i < enemyCount; i++)
    {
        // chose which side to start them
        const int side = GetRandomValue(0, 3);

        Vector2 pos;

        if (side == 0)
            pos = { static_cast<float>(GetRandomValue(0, static_cast<int>(worldWidth))), -32 };
        else if (side == 1)
            pos = { static_cast<float>(GetRandomValue(0, static_cast<int>(worldWidth))), worldHeight + 32 };
        else if (side == 2)
            pos = { -32, static_cast<float>(GetRandomValue(0, static_cast<int>(worldHeight))) };
        else
            pos = { worldWidth + 32, static_cast<float>(GetRandomValue(0, static_cast<int>(worldHeight))) };

        // determine what type of enemies are availabe per wave
        std::vector<EnemySetup::EnemyType> availableEnemies;

        if (wave < 3)
        {
            // waves 1-2: bats only
            availableEnemies = {EnemySetup::EnemyType::Bat };
        }
        else if (wave < 5)
        {
            // waves 3-4: bats and trees
            availableEnemies = {
                EnemySetup::EnemyType::Bat,
                EnemySetup::EnemyType::Tree
            };
        }
        else
        {
            // wave 5+: trees only
            availableEnemies = {EnemySetup::EnemyType::Tree };
        }

        // pick a random enemy from the available pool
        const EnemySetup::EnemyType type = availableEnemies[
                GetRandomValue(
                    0,
                    static_cast<int>(availableEnemies.size()) - 1
                )
            ];

        // match enemy type and create it
        switch (type)
        {
            case EnemySetup::EnemyType::Bat:
            {
                auto enemy = std::make_unique<BatEnemy>(pos);
                enemy->SetStats(health, speed);
                enemies.push_back(std::move(enemy));
                break;
            }

            case EnemySetup::EnemyType::Tree:
            {
                auto enemy = std::make_unique<TreeEnemy>(pos);
                enemy->SetStats(health, speed);
                enemies.push_back(std::move(enemy));
                break;
            }
        }

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
