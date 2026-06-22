#pragma once

#include <vector>
#include <raylib.h>

#include "entities/Player.h"
#include "entities/Enemy.h"
#include "Spawner.h"
#include "systems/UpgradeSystem.h"
#include "xp/XPOrb.h"
#include "UpgradeType.h"

#include "systems/PlayerSystem.h"
#include "systems/EnemySystem.h"
#include "systems/CombatSystem.h"
#include "systems/XPSystem.h"
#include "systems/CollisionSystem.h"
#include "systems/TargetingSystem.h"

class UpgradeSystem;

struct UpgradeOption
{
    UpgradeType type{};
    const char* name{};
};

class World
{
public:
    bool levelUpActive = false;
    std::vector<UpgradeOption> options;
    int hoveredUpgradeIndex{-1};

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

    bool IsLevelUpActive() const { return levelUpActive; }

    void OnPlayerLevelUp(int level);
    void EnterLevelUp();
    void ApplyUpgrade(int index);

private:
    struct WorldBounds
    {
        float width{};
        float height{};
    };

    WorldBounds bounds;
    Camera2D camera;

    Player player;
    std::vector<Enemy> enemies;
    std::vector<XPOrb> xpOrbs;

    Spawner spawner;

    // systems
    PlayerSystem playerSystem;
    EnemySystem enemySystem;
    CombatSystem combatSystem;
    XPSystem xpSystem;
    CollisionSystem collisionSystem;
    UpgradeSystem upgradeSystem;
    TargetingSystem targetingSystem;
};
