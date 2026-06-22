#include "EnemySystem.h"
#include <algorithm>

void EnemySystem::Update(
    float dt,
    std::vector<Enemy>& enemies,
    Spawner& spawner,
    Vector2 playerPos,
    float worldW,
    float worldH,
    std::vector<XPOrb>& xpOrbs)
{
    spawner.Update(dt, enemies, worldW, worldH);

    for (auto& e : enemies)
        e.Update(dt, playerPos);
}

void EnemySystem::RemoveDead(
    std::vector<Enemy>& enemies,
    std::vector<XPOrb>& xpOrbs)
{
    for (auto& e : enemies)
    {
        if (e.isDead())
        {
            xpOrbs.emplace_back(e.GetPos(), 10); // XP DROP FIX
        }
    }

    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](Enemy& e)
            {
                return e.isDead();
            }),
        enemies.end()
    );
}
