#pragma once

#include <vector>
#include <memory>
#include "entities/Enemy.h"
#include "Spawner.h"
#include "xp/XPOrb.h"

class EnemySystem
{
public:
    static void Update(
        float deltaTime,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        Spawner& spawner,
        Vector2 playerPos,
        float worldW,
        float worldH,
        std::vector<XPOrb>& xpOrbs
    );

    static void RemoveDead(
        std::vector<std::unique_ptr<Enemy>>& enemies,
        std::vector<XPOrb>& xpOrbs
    );
};
