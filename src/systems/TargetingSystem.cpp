#include "TargetingSystem.h"

#include "entities/Enemy.h"
#include "entities/Player.h"

#include <raymath.h>
#include <cfloat>

const Enemy* TargetingSystem::FindClosestEnemy(
    const Player& player,
    const std::vector<Enemy>& enemies,
    float radius
) const
{
    const Enemy* closestEnemy = nullptr;

    float closestDistanceSq = FLT_MAX;
    float radiusSq = radius * radius;

    Vector2 playerPos = player.GetPos();

    for (const auto& enemy : enemies)
    {
        float distSq =
            Vector2DistanceSqr(
                playerPos,
                enemy.GetPos()
            );

        if (distSq > radiusSq)
            continue;

        if (distSq < closestDistanceSq)
        {
            closestDistanceSq = distSq;
            closestEnemy = &enemy;
        }
    }

    return closestEnemy;
}
