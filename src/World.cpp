#include "World.h"
#include "AssetManager.h"

#include <raymath.h>

World::World()
{
    // init world bounds - seperate from window bounds
    bounds.width = 3000.0f;
    bounds.height = 2000.0f;

    // init camera in world
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.offset = {640, 360};
    camera.target = {0, 0};
}

void World::Reset()
{
    // on resert clear all enemies and make new player
    enemies.clear();
    player = Player();
}

bool World::IsPlayerDead() const
{
    // check if player is dead (bool dead or nah)
    return player.isDead();
}

void World::Update(float dt)
{
    // -------------------------
    // AIM DIRECTION
    // -------------------------
    // mouse pos from screen/window to world coords
    Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
    // aim from player to mouse pos
    Vector2 aimDir = Vector2Subtract(mouseWorld, player.GetPos());

    // normalize aim movements
    if (Vector2Length(aimDir) > 0.0f)
        aimDir = Vector2Normalize(aimDir);

    // -------------------------
    // PLAYER UPDATE
    // -------------------------
    player.Update(dt, aimDir);

    // clamp player in world space
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

    // clamp camera to target
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
    // for all orbs
    for (auto& orb : xpOrbs)
    {
        // update orbs
        orb.Update(dt, player.GetPos());

        // if orb picked up
        if (orb.IsCollected())
        {
            // update player xp
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

    // draw bounds limites
    DrawRectangleLines(0, 0, bounds.width, bounds.height, GRAY);

    player.Draw();

    // draw all enemies in the array
    for (Enemy& e : enemies)
        e.Draw();

    // draw all orbs in the array
    for (const auto& orb : xpOrbs)
        orb.Draw();

    EndMode2D();
}

void World::HandleCollisions()
{
    // -------------------------
    // WEAPON vs ENEMY COLLISIONS
    // -------------------------
    // for all weapons available to player
    for (auto& weapon : player.GetWeapons())
    {
        // for all enemies in the array
        for (Enemy& enemy : enemies)
        {
            // check if weapon is colliding with enemy
            weapon->HandleCollisions(enemy);
        }
    }

    // -------------------------
    // PLAYER TOUCH DAMAGE
    // -------------------------
    // for all enemies in enemy array
    for (Enemy& enemy : enemies)
    {
        // check if enemy collides with player
        if (CheckCollisionCircles(
            player.GetPos(),
            player.GetRadius(),
            enemy.GetPos(),
            enemy.GetRadius()))
        {
            // if collides - player takes damage
            player.TakeDamage(1);
        }
    }
}

void World::RemoveDeadEnemies()
{
    // for all enemies
    for (int i = 0; i < (int)enemies.size(); i++)
    {
        // if any of those enemeis are dead
        if (enemies[i].isDead())
        {
            // drop xp
            xpOrbs.emplace_back(enemies[i].GetPos(), 10);

            // erase enemy from array
            enemies.erase(enemies.begin() + i);
            i--;
        }
    }
}

void World::HandleEnemySeparation()
{
    // distance enemies can be from eachother
    const float minDist = 28.0f;

    // for all enemies until end of arr
    for (size_t i = 0; i < enemies.size(); i++)
    {
        // and backwards
        for (size_t j = i + 1; j < enemies.size(); j++)
        {
            // check distance between them
            Vector2 dir = Vector2Subtract(enemies[i].GetPos(), enemies[j].GetPos());
            // distance bewteen them
            float dist = Vector2Length(dir);

            // check they are the correct distance away
            if (dist < minDist && dist > 0.0f)
            {
                // if not move them away - never let them be close
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
