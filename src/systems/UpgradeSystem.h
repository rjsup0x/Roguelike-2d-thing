#pragma once

class World;

class UpgradeSystem
{
public:
    static void Enter(World& world);
    static void Apply(World& world, int index);
};
