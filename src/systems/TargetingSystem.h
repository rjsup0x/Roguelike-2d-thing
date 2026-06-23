#pragma once

#include <vector>
#include <memory>

class Enemy;
class Player;

class TargetingSystem
{
public:
        static  const Enemy* FindClosestEnemy(
        const Player& player,
        const std::vector<std::unique_ptr<Enemy>>& enemies,
        float radius
    ) ;
};
