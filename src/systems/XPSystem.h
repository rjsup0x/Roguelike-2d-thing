#pragma once

#include <vector>
#include "xp/XPOrb.h"
#include "entities/Player.h"

class XPSystem
{
public:
    void Update(float dt, std::vector<XPOrb>& orbs, Player& player);
};
