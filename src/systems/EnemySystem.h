#pragma once

#include <vector>
#include <memory>
#include "entities/Enemy.h"
#include "Spawner.h"
#include "xp/XPOrb.h"

class EnemySystem
{
public:
    void Update(
        float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        Spawner& spawner,
        Vector2 playerPos,
        float worldW,
        float worldH,
        std::vector<XPOrb>& xpOrbs
    );

    void RemoveDead(
        std::vector<std::unique_ptr<Enemy>>& enemies,
        std::vector<XPOrb>& xpOrbs
    );

    Enemy* GetClosestEnemy(
        std::vector<std::unique_ptr<Enemy>>& enemies,
        Vector2 position
    );
};
