#include "World.h"
#include "systems/UpgradeSystem.h"

#include <raymath.h>

World::World()
{
    bounds.width = 3000.0f;
    bounds.height = 2000.0f;

    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.offset = {640, 360};
    camera.target = {0, 0};

    player.SetLevelUpCallback([this](int level)
    {
        OnPlayerLevelUp(level);
    });
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

void World::Update(float dt)
{
    if (levelUpActive)
        return;

    survivalTime += dt;

    constexpr float AUTO_TARGET_RADIUS = 350.0f;

    const Enemy* target =
        targetingSystem.FindClosestEnemy(
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

    playerSystem.Update(player, dt, aimDir, bounds.width, bounds.height);

    enemySystem.Update(dt, enemies, spawner, player.GetPos(), bounds.width, bounds.height, xpOrbs);

    camera.target = player.GetPos();

    float hw = camera.offset.x / camera.zoom;
    float hh = camera.offset.y / camera.zoom;

    camera.target.x = Clamp(camera.target.x, hw, bounds.width - hw);
    camera.target.y = Clamp(camera.target.y, hh, bounds.height - hh);

    combatSystem.Update(player, enemies);

    collisionSystem.SeparateEnemies(enemies);

    enemySystem.RemoveDead(enemies, xpOrbs);

    xpSystem.Update(dt, xpOrbs, player);
}

void World::Draw()
{
    for (auto& e : enemies)
        e->Draw();

    for (auto& orb : xpOrbs)
        orb.Draw();

    player.Draw();
}

// ---------------- GETTERS ----------------

Camera2D& World::GetCamera() { return camera; }
Player& World::GetPlayer() { return player; }
const Player& World::GetPlayer() const { return player; }

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
    upgradeSystem.Enter(*this);
}

void World::ApplyUpgrade(int index)
{
    upgradeSystem.Apply(*this, index);
}
