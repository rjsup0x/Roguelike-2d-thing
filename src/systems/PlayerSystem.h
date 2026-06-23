#pragma once

#include <raylib.h>
#include "entities/Player.h"

class PlayerSystem
{
public:
    static void Update(Player& player, float dt, Vector2 aimDir, float worldWidth, float worldHeight);

    static Vector2 GetClampedPosition(const Player& player, float worldWidth, float worldHeight);
};
