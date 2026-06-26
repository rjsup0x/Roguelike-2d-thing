#pragma once

#include <raylib.h>
#include "entities/Player.h"
#include "TileMap.h"

class PlayerSystem
{
public:
    // `map` is used to resolve movement against solid tiles (walls) after
    // Player::Update() has applied raw input-driven movement. Position is
    // corrected per-axis so the player slides along a wall instead of
    // getting fully stopped when approaching it diagonally.
    static void Update(Player& player, float deltaTime, Vector2 aimDirection, float worldWidth, float worldHeight, const TileMap& map);

    static Vector2 GetClampedPosition(const Player& player, float worldWidth, float worldHeight);
};
