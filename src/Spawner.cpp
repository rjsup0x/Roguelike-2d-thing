#include "Spawner.h"
#include <cstddef>
#include <raymath.h>
#include <raylib.h>

Spawner::Spawner()
    : wave{},
    waveActive{false},
    showWaveText{false},
    waveTextVisible{false},
    blinkTimer{0.0f},
    blinkCount{},
    maxBlinks{6}
{

}

int Spawner::GetWave() const
{
    return wave;
}

bool Spawner::ShouldShowWaveText() const
{
    return showWaveText && waveTextVisible;
}

void Spawner::Update(float dt, std::vector<Enemy>& enemies, float worldWidth, float worldHeight)
{
    // -------------------------
    // BLINK SYSTEM
    // -------------------------
    if (showWaveText)
    {
        blinkTimer -= dt;

        if (blinkTimer <= 0.0f)
        {
            blinkTimer = 0.25f;

            // toggle visibility
            waveTextVisible = !waveTextVisible;

            // how many times it has blinked
            blinkCount++;

            if (blinkCount >= maxBlinks)
            {
                // stop when blinked # of times (should be 6)
                showWaveText = false;
                waveTextVisible = false;
            }
        }
    }

    // -------------------------
    // WAVE LOGIC
    // -------------------------
    if (!waveActive)
    {
        // if wave not active and no enemies
        if (enemies.empty())
        {
            // strat the wave produce enemies
            StartWave(enemies, worldWidth, worldHeight);
        }
        return;
    }

    if (enemies.empty())
    {
        waveActive = false;
    }
}

void Spawner::StartWave(std::vector<Enemy>& enemies, float worldWidth, float worldHeight)
{
    // add waves
    wave++;
    waveActive = true;

    // reset blink system
    showWaveText = true;
    waveTextVisible = true;
    blinkTimer = 0.3f;
    blinkCount = 0;

    int enemyCount{10};
    int health{50};
    float speed{100.0f};

    // wave logic for enemies
    if (wave == 1)
    {
        // enemies 1
        enemyCount = 10;
        health = 50;
        speed = 90.0f;
    }
    else if (wave == 2)
    {
        // enemies 2
        enemyCount = 25;
        health = 80;
        speed = 110.0f;
    }
    else
    {
        // enemies 3
        enemyCount = 30;
        health = 120;
        speed = 130.0f;
    }

    // for all enemies
    for (size_t i{}; i < enemyCount; ++i)
    {
        // get reandom side to spawn on
        int side = GetRandomValue(0, 3);

        Vector2 pos;

        // depending on side position the enemy there
        if (side == 0)
            pos = { (float)GetRandomValue(0, (int)worldWidth), -32 };
        else if (side == 1)
            pos = { (float)GetRandomValue(0, (int)worldWidth), worldHeight + 32 };
        else if (side == 2)
            pos = { -32, (float)GetRandomValue(0, (int)worldHeight) };
        else
            pos = { worldWidth + 32, (float)GetRandomValue(0, (int)worldHeight) };

        Enemy e(pos);
        e.SetStats(health, speed);

        enemies.push_back(e);
    }
}
