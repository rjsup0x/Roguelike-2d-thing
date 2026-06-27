#include "EnemySystem.h"

void EnemySystem::Update(
    const float deltaTime,
    std::vector<std::unique_ptr<Enemy>> &enemies,
    Spawner &spawner,
    const Vector2 playerPos,
    const float worldW,
    const float worldH)
{
    spawner.Update(deltaTime, enemies, worldW, worldH);

    for (const auto& e : enemies)
        e->Update(deltaTime, playerPos);
}

void EnemySystem::RemoveDead(
    std::vector<std::unique_ptr<Enemy>>& enemies,
    std::vector<XPOrb>& xpOrbs)
{
    for (const auto& enemy : enemies)
    {
        if (enemy->isDead())
        {
            xpOrbs.emplace_back(enemy->GetPos(), 10);
        }
    }

    std::erase_if(enemies,
                  [](const std::unique_ptr<Enemy>& e)
                  {
                      return e->isDead();
                  });
}

