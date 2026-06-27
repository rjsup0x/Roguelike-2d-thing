#include "TargetingSystem.h"
#include "entities/Enemy.h"
#include "entities/Player.h"
#include <raymath.h>

const Enemy* TargetingSystem::FindClosestEnemy(
    const Player& player,
    const std::vector<std::unique_ptr<Enemy>>& enemies,
    const float radius)
{
    const Enemy* closest = nullptr;
    float bestDist = radius * radius;

    for (const auto& enemy : enemies)
    {
        if (const float d = Vector2DistanceSqr(player.GetPos(), enemy->GetPos()); d < bestDist)
        {
            bestDist = d;
            closest = enemy.get();
        }
    }

    return closest;
}
