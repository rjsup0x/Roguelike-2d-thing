#pragma once

#include <vector>
#include "entities/Enemy.h"

class CollisionSystem
{
public:
    void SeparateEnemies(
        std::vector<std::unique_ptr<Enemy>>& enemies
    );
};
