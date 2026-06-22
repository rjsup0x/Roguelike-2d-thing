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
#include "UpgradeType.h"
#include <vector>

struct UpgradeOption
{
    UpgradeType type{};
    const char* name{};
};

class World
{
public:
    int hoveredUpgradeIndex{-1};

    // world constructor
    World();

    // update world
    void Update(float dt);
    // draw into world
    void Draw();

    // reset the world
    void Reset();
    // check if player in the world is dead
    bool IsPlayerDead() const;

    // get the camera instance used in the world
    Camera2D& GetCamera();

    // get the player instance in the world
    Player& GetPlayer();
    const Player& GetPlayer() const;

    // get health atrtibutes of player in wolrd
    int GetPlayerHealth() const;
    int GetPlayerMaxHealth() const;

    // get spawner instance in world
    Spawner& GetSpawner();

    // update system
    bool levelUpActive = false;
    std::vector<UpgradeOption> options;

    void OnPlayerLevelUp(int level);
    void EnterLevelUp();
    void ApplyUpgrade(int index);

    bool IsLevelUpActive() const { return levelUpActive; }

private:
    // seperate world bounds to screen bounds
    struct WorldBounds
    {
        float width{};
        float height{};
    };

    // init world bounds instance
    WorldBounds bounds;

    // init camera instance
    Camera2D camera;

    // init player instance
    Player player;
    // init enemies instance as array of enemies
    std::vector<Enemy> enemies;

    // init spawner instance
    Spawner spawner;

    // check collisons in world space
    void HandleCollisions();
    // ensure enemies in world space dont clutter
    void HandleEnemySeparation();
    // if enemy health < 0 remove them from world space
    void RemoveDeadEnemies();

    // init xpObrs instance as vector or orbs
    std::vector<XPOrb> xpOrbs;
};
