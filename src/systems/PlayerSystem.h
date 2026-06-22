#pragma once

#include <raylib.h>
#include "entities/Player.h"

class PlayerSystem
{
public:
    void Update(Player& player, float dt, Vector2 aimDir, float worldWidth, float worldHeight);

    Vector2 GetClampedPosition(const Player& player, float worldWidth, float worldHeight);
};
