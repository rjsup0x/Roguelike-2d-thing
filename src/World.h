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

#include <memory>

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
    void Draw() const;

    [[nodiscard]] float GetSurvivalTime() const;
    void Reset();
    [[nodiscard]] bool IsPlayerDead() const;

    Camera2D& GetCamera();
    [[nodiscard]] Player& GetPlayer();
    // const Player& GetPlayer() const;

    [[nodiscard]] int GetPlayerHealth() const;
    [[nodiscard]] int GetPlayerMaxHealth() const;

    Spawner& GetSpawner();

    [[nodiscard]] bool IsLevelUpActive() const { return levelUpActive; }

    void OnPlayerLevelUp(int level);
    void EnterLevelUp();
    void ApplyUpgrade(int index);

private:
    float survivalTime{};
    struct WorldBounds
    {
        float width{};
        float height{};
    };

    WorldBounds bounds;
    Camera2D camera{};

    Player player;
    std::vector<std::unique_ptr<Enemy>> enemies;
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
