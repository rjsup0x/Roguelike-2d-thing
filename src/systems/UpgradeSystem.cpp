#include "UpgradeSystem.h"
#include "World.h"
#include "weapons/OrbitalWeapon.h"
#include <UpgradeType.h>
#include <memory>

void UpgradeSystem::Enter(World& world)
{
    world.levelUpActive = true;
    world.options.clear();

    world.options.push_back({UpgradeType::OrbitalWeapon, "Orbital Weapon"});
    world.options.push_back({UpgradeType::MaxHealth, "Max Health +20"});
    world.options.push_back({UpgradeType::Damage, "Damage +1"});
}

void UpgradeSystem::Apply(World& world, const int index)
{
    if (index < 0 || index >= static_cast<int>(world.options.size()))
        return;

    switch (world.options[index].type)
    {
        case UpgradeType::OrbitalWeapon:
            world.GetPlayer().AddWeapon(std::make_unique<OrbitalWeapon>());
            break;

        case UpgradeType::MaxHealth:
            world.GetPlayer().IncreaseMaxHealth(20);
            break;

        case UpgradeType::Damage:
            world.GetPlayer().IncreaseDamage(1);
            break;
    }

    world.levelUpActive = false;
}
