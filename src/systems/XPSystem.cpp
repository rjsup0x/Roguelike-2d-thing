#include "XPSystem.h"

#include "AssetManager.h"
// #include <algorithm>

void XPSystem::Update(float dt, std::vector<XPOrb>& orbs, Player& player)
{
    for (auto& orb : orbs)
    {
        orb.Update(dt, player.GetPos());

        if (orb.IsCollected())
        {
            player.AddXP(orb.GetValue());

            PlaySound(
                AssetManager::GetSound("orb_pickup")
            );
        }
    }

    std::erase_if(orbs,
                  [](const XPOrb& o)
                  {
                      return o.IsCollected();
                  });
}
