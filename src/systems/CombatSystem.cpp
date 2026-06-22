#include "CombatSystem.h"
#include <raylib.h>

void CombatSystem::Update(Player& player, std::vector<Enemy>& enemies)
{
    // weapon vs enemy
    for (auto& weapon : player.GetWeapons())
    {
        for (auto& enemy : enemies)
        {
            weapon->HandleCollisions(enemy);
        }
    }

    // enemy touches player
    for (auto& enemy : enemies)
    {
        if (CheckCollisionCircles(
            player.GetPos(),
            player.GetRadius(),
            enemy.GetPos(),
            enemy.GetRadius()))
        {
            player.TakeDamage(1);
        }
    }
}
