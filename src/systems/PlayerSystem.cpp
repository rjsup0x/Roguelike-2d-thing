#include "PlayerSystem.h"
#include <raymath.h>

namespace
{
    // Checks whether the player's bounding circle overlaps a solid tile at
    // `pos`. Sampling the 4 cardinal points of the circle (not just the
    // center) catches walls the player's edge has touched even though
    // their center point hasn't crossed the tile boundary yet — important
    // since the player has a real radius (Player::GetRadius()), not just
    // a point.
    bool OverlapsSolidTile(const TileMap& map, Vector2 pos, float radius)
    {
        return map.IsSolidAtWorldPos({pos.x - radius, pos.y}) ||
               map.IsSolidAtWorldPos({pos.x + radius, pos.y}) ||
               map.IsSolidAtWorldPos({pos.x, pos.y - radius}) ||
               map.IsSolidAtWorldPos({pos.x, pos.y + radius}) ||
               map.IsSolidAtWorldPos(pos);
    }
}

void PlayerSystem::Update(Player& player, const float deltaTime, const Vector2 aimDirection, const float worldWidth, const float worldHeight, const TileMap& map)
{
    const Vector2 posBeforeMove = player.GetPos();

    player.Update(deltaTime, aimDirection);

    auto [x, y] = player.GetPos();
    const float radius = Player::GetRadius();

    // Resolve wall collision per-axis: try X movement alone, then Y
    // movement alone. This lets the player slide along a wall when
    // moving diagonally into it, rather than being stopped dead the
    // instant either axis touches a wall.
    Vector2 resolved = posBeforeMove;

    if (const Vector2 tryX = { x, posBeforeMove.y }; !OverlapsSolidTile(map, tryX, radius))
        resolved.x = x;

    if (const Vector2 tryY = { resolved.x, y }; !OverlapsSolidTile(map, tryY, radius))
        resolved.y = y;

    Vector2 p = resolved;
    p.x = Clamp(p.x, 0.0f, worldWidth);
    p.y = Clamp(p.y, 0.0f, worldHeight);
    player.SetPos(p);
}

Vector2 PlayerSystem::GetClampedPosition(const Player& player, const float worldWidth, const float worldHeight)
{
    Vector2 p = player.GetPos();
    p.x = Clamp(p.x, 0.0f, worldWidth);
    p.y = Clamp(p.y, 0.0f, worldHeight);
    return p;
}
