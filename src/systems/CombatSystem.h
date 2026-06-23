#pragma once

#include <vector>
#include "entities/Enemy.h"
#include "entities/Player.h"

class CombatSystem
{
public:
    static void Update(
        Player& player,
        const std::vector<std::unique_ptr<Enemy>>& enemies
    );
};
