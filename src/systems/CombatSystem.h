#pragma once

#include <vector>
#include "entities/Enemy.h"
#include "entities/Player.h"

class CombatSystem
{
public:
    void Update(Player& player, std::vector<Enemy>& enemies);
};
