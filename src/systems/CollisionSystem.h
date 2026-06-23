#pragma once

#include <vector>
#include "entities/Enemy.h"

class CollisionSystem
{
public:
    static void SeparateEnemies(
        const std::vector<std::unique_ptr<Enemy>>& enemies
    );
};
