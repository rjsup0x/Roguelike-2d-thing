#include "Spawner.h"
#include <raymath.h>
#include "entities/BatEnemy.h"

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

    int enemyCount = 10;
    int health = 50;
    float speed = 100.0f;

    if (wave == 1)
    {
        enemyCount = 10;
        health = 50;
        speed = 90.0f;
    }
    else if (wave == 2)
    {
        enemyCount = 25;
        health = 80;
        speed = 110.0f;
    }
    else
    {
        enemyCount = 30;
        health = 120;
        speed = 130.0f;
    }

    for (int i = 0; i < enemyCount; i++)
    {
        int side = GetRandomValue(0, 3);

        Vector2 pos;

        if (side == 0)
            pos = { (float)GetRandomValue(0, (int)worldWidth), -32 };
        else if (side == 1)
            pos = { (float)GetRandomValue(0, (int)worldWidth), worldHeight + 32 };
        else if (side == 2)
            pos = { -32, (float)GetRandomValue(0, (int)worldHeight) };
        else
            pos = { worldWidth + 32, (float)GetRandomValue(0, (int)worldHeight) };

        auto e = std::make_unique<BatEnemy>(pos);
        e->SetStats(health, speed);

        enemies.push_back(std::move(e));
    }
}
