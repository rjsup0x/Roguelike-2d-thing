#include "World.h"
#include "systems/UpgradeSystem.h"

#include <raymath.h>

#include "systems/CollisionSystem.h"
#include "systems/CombatSystem.h"
#include "systems/EnemySystem.h"
#include "systems/PlayerSystem.h"
#include "systems/TargetingSystem.h"
#include "systems/XPSystem.h"

World::World()
{
    constexpr auto kTestMapPath = "/Users/ry/projects/roli/src/assets/maps/TestMap.tmj";

    bounds.width = 3008.0f;
    bounds.height = 2016.0f;

    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.offset = {640, 360};
    camera.target = {player.GetPos().x, player.GetPos().y};

    player.SetLevelUpCallback([this](const int level)
    {
        OnPlayerLevelUp();
    });

    // load map/level
    map.LoadFromFile(kTestMapPath);
}

void World::Reset()
{
    enemies.clear();
    xpOrbs.clear();

    survivalTime = {0.0f};

    player = Player();

    player.SetLevelUpCallback([this](const int level)
    {
        OnPlayerLevelUp();
    });

    spawner.Reset();

    levelUpActive = false;
    hoveredUpgradeIndex = -1;
    options.clear();
}

float World::GetSurvivalTime() const
{
    return survivalTime;
}

bool World::IsPlayerDead() const
{
    return player.isDead();
}

void World::Update(float deltaTime)
{
    if (levelUpActive)
        return;

    survivalTime += deltaTime;

    // distance between player and enemy before player shoots
    constexpr float AUTO_TARGET_RADIUS{500.0f};

    const Enemy* target =
        TargetingSystem::FindClosestEnemy(
            player,
            enemies,
            AUTO_TARGET_RADIUS
        );

    constexpr float aimDirX{0.0f};
    constexpr float aimDirY{0.0f};

    Vector2 aimDir{aimDirX, aimDirY};

    if (target)
    {
        aimDir =
            Vector2Normalize(
                Vector2Subtract(
                    target->GetPos(),
                    player.GetPos()
                )
            );
    }

    PlayerSystem::Update(player, deltaTime, aimDir, bounds.width, bounds.height, map);

    EnemySystem::Update(deltaTime, enemies, spawner, player.GetPos(), bounds.width, bounds.height);

    camera.target = player.GetPos();

    const float hw = camera.offset.x / camera.zoom;
    const float hh = camera.offset.y / camera.zoom;

    camera.target.x = Clamp(camera.target.x, hw, bounds.width - hw);
    camera.target.y = Clamp(camera.target.y, hh, bounds.height - hh);

    CombatSystem::Update(player, enemies);

    CollisionSystem::SeparateEnemies(enemies);

    EnemySystem::RemoveDead(enemies, xpOrbs);

    XPSystem::Update(deltaTime, xpOrbs, player);
}

void World::Draw() const {
    map.Draw();

    for (auto& enemy : enemies)
        enemy->Draw();

    for (auto& orb : xpOrbs)
        orb.Draw();

    player.Draw();
}

// ---------------- GETTERS ----------------

Camera2D& World::GetCamera() { return camera; }
Player& World::GetPlayer() { return player; }

int World::GetPlayerHealth() const { return player.GetHealth(); }
int World::GetPlayerMaxHealth() const { return player.GetMaxHealth(); }

Spawner& World::GetSpawner() { return spawner; }

// ---------------- LEVEL / UPGRADES ----------------

void World::OnPlayerLevelUp()
{
    EnterLevelUp();
}

void World::EnterLevelUp()
{
    levelUpActive = true;
    UpgradeSystem::Enter(*this);
}

void World::ApplyUpgrade(const int index)
{
    UpgradeSystem::Apply(*this, index);
}
