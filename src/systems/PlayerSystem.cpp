#include "PlayerSystem.h"
#include <raymath.h>

void PlayerSystem::Update(Player& player, float dt, Vector2 aimDir, float worldWidth, float worldHeight)
{
    player.Update(dt, aimDir);

    Vector2 p = player.GetPos();
    p.x = Clamp(p.x, 0.0f, worldWidth);
    p.y = Clamp(p.y, 0.0f, worldHeight);
    player.SetPos(p);
}

Vector2 PlayerSystem::GetClampedPosition(const Player& player, float worldWidth, float worldHeight)
{
    Vector2 p = player.GetPos();
    p.x = Clamp(p.x, 0.0f, worldWidth);
    p.y = Clamp(p.y, 0.0f, worldHeight);
    return p;
}
