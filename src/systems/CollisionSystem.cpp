#include "CollisionSystem.h"
#include <raymath.h>

void CollisionSystem::SeparateEnemies(
    const std::vector<std::unique_ptr<Enemy>>& enemies)
{
    constexpr float minDist = 30.0f;

    for (size_t i = 0; i < enemies.size(); i++)
    {
        for (size_t j = i + 1; j < enemies.size(); j++)
        {
            Vector2 dir = Vector2Subtract(enemies[i]->GetPos(), enemies[j]->GetPos());
            float dist = Vector2Length(dir);

            if (dist < minDist && dist > 0.0f)
            {
                Vector2 push = Vector2Scale(Vector2Normalize(dir), (minDist - dist) * 0.5f);

                enemies[i]->SetPos(Vector2Add(enemies[i]->GetPos(), push));
                enemies[j]->SetPos(Vector2Subtract(enemies[j]->GetPos(), push));
            }
        }
    }
}
