#include "World.h"
#include "systems/UpgradeSystem.h"

#include <raymath.h>

World::World()
{
    constexpr const char* kTestMapPath = "/Users/ry/projects/roli/src/assets/maps/TestMap.tmj";

    bounds.width = 3008.0f;
    bounds.height = 2016.0f;

    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.offset = {640, 360};
    camera.target = {0, 0};

    player.SetLevelUpCallback([this](int level)
    {
        OnPlayerLevelUp(level);
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

    player.SetLevelUpCallback([this](int level)
    {
        OnPlayerLevelUp(level);
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
    constexpr float AUTO_TARGET_RADIUS = 500.0f;

    const Enemy* target =
        TargetingSystem::FindClosestEnemy(
            player,
            enemies,
            AUTO_TARGET_RADIUS
        );

    Vector2 aimDir{0.0f, 0.0f};

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

    EnemySystem::Update(deltaTime, enemies, spawner, player.GetPos(), bounds.width, bounds.height, xpOrbs);

    camera.target = player.GetPos();

    float hw = camera.offset.x / camera.zoom;
    float hh = camera.offset.y / camera.zoom;

    camera.target.x = Clamp(camera.target.x, hw, bounds.width - hw);
    camera.target.y = Clamp(camera.target.y, hh, bounds.height - hh);

    CombatSystem::Update(player, enemies);

    CollisionSystem::SeparateEnemies(enemies);

    EnemySystem::RemoveDead(enemies, xpOrbs);

    XPSystem::Update(deltaTime, xpOrbs, player);
}

void World::Draw() const {
    map.Draw();

    for (auto& e : enemies)
        e->Draw();

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

void World::OnPlayerLevelUp(int level)
{
    EnterLevelUp();
}

void World::EnterLevelUp()
{
    levelUpActive = true;
    UpgradeSystem::Enter(*this);
}

void World::ApplyUpgrade(int index)
{
    UpgradeSystem::Apply(*this, index);
}
