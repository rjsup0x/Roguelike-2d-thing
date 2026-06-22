#include "XPSystem.h"
#include <algorithm>

void XPSystem::Update(float dt, std::vector<XPOrb>& orbs, Player& player)
{
    for (auto& orb : orbs)
    {
        orb.Update(dt, player.GetPos());

        if (orb.IsCollected())
        {
            player.AddXP(orb.GetValue());
        }
    }

    orbs.erase(
        std::remove_if(orbs.begin(), orbs.end(),
            [](const XPOrb& o)
            {
                return o.IsCollected();
            }),
        orbs.end()
    );
}
