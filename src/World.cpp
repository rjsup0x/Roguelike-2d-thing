#include "World.h"
#include "AssetManager.h"

#include <raymath.h>

World::World()
{
    bounds.width = 3000.0f;
    bounds.height = 2000.0f;

    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.offset = {640, 360};
    camera.target = {0, 0};
}

void World::Reset()
{
    enemies.clear();
    player = Player();
}

bool World::IsPlayerDead() const
{
    return player.isDead();
}

void World::Update(float dt)
{
    // -------------------------
    // AIM DIRECTION
    // -------------------------
    Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
    Vector2 aimDir = Vector2Subtract(mouseWorld, player.GetPos());

    if (Vector2Length(aimDir) > 0.0f)
        aimDir = Vector2Normalize(aimDir);

    // -------------------------
    // PLAYER UPDATE
    // -------------------------
    player.Update(dt, aimDir);

    Vector2 p = player.GetPos();
    p.x = Clamp(p.x, 0.0f, bounds.width);
    p.y = Clamp(p.y, 0.0f, bounds.height);
    player.SetPos(p);

    // -------------------------
    // ENEMIES UPDATE
    // -------------------------
    spawner.Update(dt, enemies, bounds.width, bounds.height);

    for (Enemy& e : enemies)
        e.Update(dt, player.GetPos());

    // -------------------------
    // CAMERA FOLLOW
    // -------------------------
    camera.target = player.GetPos();

    float hw = camera.offset.x / camera.zoom;
    float hh = camera.offset.y / camera.zoom;

    camera.target.x = Clamp(camera.target.x, hw, bounds.width - hw);
    camera.target.y = Clamp(camera.target.y, hh, bounds.height - hh);

    // -------------------------
    // COLLISION SYSTEM
    // -------------------------
    HandleEnemySeparation();
    HandleCollisions();
    RemoveDeadEnemies();

    // -------------------------
    // COLLECT ORBS
    // -------------------------
    for (auto& orb : xpOrbs)
    {
        orb.Update(dt, player.GetPos());

        if (orb.IsCollected())
        {
            player.AddXP(orb.GetValue());
        }
    }

    // remove collected
    xpOrbs.erase(
        std::remove_if(xpOrbs.begin(), xpOrbs.end(),
            [](const XPOrb& o)
            {
                return o.IsCollected();
            }),
        xpOrbs.end()
    );
}

void World::Draw()
{
    BeginMode2D(camera);

    DrawRectangleLines(0, 0, bounds.width, bounds.height, GRAY);

    player.Draw();

    for (Enemy& e : enemies)
        e.Draw();

    for (const auto& orb : xpOrbs)
        orb.Draw();

    EndMode2D();
}

void World::HandleCollisions()
{
    // -------------------------
    // WEAPON vs ENEMY COLLISIONS
    // -------------------------
    for (auto& weapon : player.GetWeapons())
    {
        for (Enemy& enemy : enemies)
        {
            weapon->HandleCollisions(enemy);
        }
    }

    // -------------------------
    // PLAYER TOUCH DAMAGE
    // -------------------------
    for (Enemy& enemy : enemies)
    {
        if (CheckCollisionCircles(
            player.GetPos(),
            player.GetRadius(),
            enemy.GetPos(),
            enemy.GetRadius()))
        {
            player.TakeDamage(1);
        }
    }
}

void World::RemoveDeadEnemies()
{
    for (int i = 0; i < (int)enemies.size(); i++)
    {
        // if deal drop xp
        if (enemies[i].isDead())
        {
            xpOrbs.emplace_back(enemies[i].GetPos(), 10); // drop XP

            enemies.erase(enemies.begin() + i);
            i--;
        }
    }
}

void World::HandleEnemySeparation()
{
    const float minDist = 28.0f;

    for (size_t i = 0; i < enemies.size(); i++)
    {
        for (size_t j = i + 1; j < enemies.size(); j++)
        {
            Vector2 dir = Vector2Subtract(enemies[i].GetPos(), enemies[j].GetPos());
            float dist = Vector2Length(dir);

            if (dist < minDist && dist > 0.0f)
            {
                Vector2 push = Vector2Scale(Vector2Normalize(dir), (minDist - dist) * 0.5f);

                enemies[i].SetPos(Vector2Add(enemies[i].GetPos(), push));
                enemies[j].SetPos(Vector2Subtract(enemies[j].GetPos(), push));
            }
        }
    }
}

// -------------------------
// GETTERS
// -------------------------
Camera2D& World::GetCamera()
{
    return camera;
}

Player& World::GetPlayer()
{
    return player;
}

const Player& World::GetPlayer() const
{
    return player;
}

int World::GetPlayerHealth() const
{
    return player.GetHealth();
}

int World::GetPlayerMaxHealth() const
{
    return player.GetMaxHealth();
}

Spawner& World::GetSpawner()
{
    return spawner;
}
