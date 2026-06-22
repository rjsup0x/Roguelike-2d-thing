#pragma once

#include <vector>

class Enemy;
class Player;

class TargetingSystem
{
public:
    const Enemy* FindClosestEnemy(
        const Player& player,
        const std::vector<Enemy>& enemies,
        float radius
    ) const;
};
