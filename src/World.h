#pragma once

#include <vector>
#include <raylib.h>
#include <raymath.h>

#include "entities/Player.h"
#include "entities/Enemy.h"
#include "Spawner.h"
#include "weapons/Weapon.h"
#include "xp/XPOrb.h"
#include <vector>

class World
{
public:
    World();

    void Update(float dt);
    void Draw();

    void Reset();
    bool IsPlayerDead() const;

    Camera2D& GetCamera();

    Player& GetPlayer();
    const Player& GetPlayer() const;

    int GetPlayerHealth() const;
    int GetPlayerMaxHealth() const;

    Spawner& GetSpawner();

private:
    struct WorldBounds
    {
        float width;
        float height;
    };

    WorldBounds bounds;
    Camera2D camera;

    Player player;
    std::vector<Enemy> enemies;

    Spawner spawner;

    void HandleCollisions();
    void HandleEnemySeparation();
    void RemoveDeadEnemies();

    // xp things
    std::vector<XPOrb> xpOrbs;
};
