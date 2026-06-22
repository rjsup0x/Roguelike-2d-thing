#include "TargetingSystem.h"
#include "entities/Enemy.h"
#include "entities/Player.h"
#include <raymath.h>
#include <cfloat>

const Enemy* TargetingSystem::FindClosestEnemy(
    const Player& player,
    const std::vector<std::unique_ptr<Enemy>>& enemies,
    float radius) const
{
    const Enemy* closest = nullptr;
    float bestDist = radius * radius;

    for (const auto& e : enemies)
    {
        float d = Vector2DistanceSqr(player.GetPos(), e->GetPos());

        if (d < bestDist)
        {
            bestDist = d;
            closest = e.get();
        }
    }

    return closest;
}
