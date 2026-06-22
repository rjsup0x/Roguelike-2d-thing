#pragma once

class World;

class UpgradeSystem
{
public:
    void Enter(World& world);
    void Apply(World& world, int index);
};
