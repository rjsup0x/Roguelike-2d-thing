#include "EnemySystem.h"

void EnemySystem::Update(
    const float deltaTime,
    std::vector<std::unique_ptr<Enemy>>& enemies,
    Spawner& spawner,
    Vector2 playerPos,
    float worldW,
    float worldH,
    std::vector<XPOrb>& xpOrbs)
{
    spawner.Update(deltaTime, enemies, worldW, worldH);

    for (auto& e : enemies)
        e->Update(deltaTime, playerPos);
}

void EnemySystem::RemoveDead(
    std::vector<std::unique_ptr<Enemy>>& enemies,
    std::vector<XPOrb>& xpOrbs)
{
    for (auto& e : enemies)
    {
        if (e->isDead())
        {
            xpOrbs.emplace_back(e->GetPos(), 10);
        }
    }

    std::erase_if(enemies,
                  [](const std::unique_ptr<Enemy>& e)
                  {
                      return e->isDead();
                  });
}

