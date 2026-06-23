#include "CombatSystem.h"
#include "entities/Enemy.h"
#include <raylib.h>

void CombatSystem::Update(
    Player& player,
    const std::vector<std::unique_ptr<Enemy>>& enemies)
{
    // weapon vs enemy
    for (auto& weapon : player.GetWeapons())
    {
        for (auto& enemy : enemies)
        {
            weapon->HandleCollisions(*enemy);
        }
    }

    // enemy touches player
    for (auto& enemy : enemies)
    {
        if (CheckCollisionCircles(
            player.GetPos(),
            player.GetRadius(),
            enemy->GetPos(),
            enemy->GetRadius()))
        {
            player.TakeDamage(1);
        }
    }
}
