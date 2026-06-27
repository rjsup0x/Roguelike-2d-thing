#pragma once

#include <vector>
#include "xp/XPOrb.h"
#include "entities/Player.h"

class XPSystem
{
public:
    static void Update(float deltaTime, std::vector<XPOrb>& orbs, Player& player);
};
