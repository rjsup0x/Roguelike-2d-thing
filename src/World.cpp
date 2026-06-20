#include "World.h"
// #include "CameraUtils.h"
#include <raymath.h>

World::World()
{
    // init the world space bounds
    bounds.width = 3000.0f;
    bounds.height = 2000.0f;

    // init the camera set up to player entity
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.offset = {640, 360};
    camera.target = {0, 0};
}

void World::Reset()
{
    // reset the world by clearing all curretn state
    enemies.clear();
    bullets.clear();

    player = Player();

    fireTimer = 0.0f;
}

bool World::IsPlayerDead() const
{
    return player.isDead();
}

void World::Update(float dt)
{
    // update the player in the world
    player.Update(dt);

    // get the position of the player entity
    Vector2 p = player.GetPos();
    p.x = Clamp(p.x, 0.0f, bounds.width);
    p.y = Clamp(p.y, 0.0f, bounds.height);
    player.SetPos(p);

    // init and update the enemy spawner
    spawner.Update(dt, enemies, bounds.width, bounds.height);

    // for all enemies update them
    for (Enemy& e : enemies)
        e.Update(dt, player.GetPos());

    // add the camera to the player pos
    camera.target = player.GetPos();

    // init the camera things like zoom offset, claping it to player etc
    float hw = camera.offset.x / camera.zoom;
    float hh = camera.offset.y / camera.zoom;

    camera.target.x = Clamp(camera.target.x, hw, bounds.width - hw);
    camera.target.y = Clamp(camera.target.y, hh, bounds.height - hh);

    fireTimer -= dt;

    // follow mouse logic
    // get mouse pos in world space instead of screen
    Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
    //
    Vector2 dir = Vector2Subtract(mouseWorld, player.GetPos());

    if (Vector2Length(dir) > 0.0f)
        dir = Vector2Normalize(dir);

    // firing logic
    if (fireTimer <= 0.0f)
    {
        bullets.emplace_back(
            player.GetPos(),
            dir,
            player.GetVelocity()
        );

        fireTimer = 1.0f / fireRate;
    }

    // update all bulelts in the array
    for (Bullet& b : bullets)
        b.Update(dt);

    HandleCollisions();
    RemoveDeadEnemies();
    RemoveOffScreenBullets();
}

void World::Draw()
{
    BeginMode2D(camera);

    // world space outlines
    DrawRectangleLines(0, 0, bounds.width, bounds.height, GRAY);

    // draw the player
    player.Draw();

    // draw all enemies in the array
    for (Enemy& e : enemies)
        e.Draw();

    // draw all bullets
    for (Bullet& b : bullets)
        b.Draw();

    EndMode2D();
}

void World::HandleCollisions()
{
    // collision with bullets
    for (int i = 0; i < (int)bullets.size(); i++)
    {
        bool bulletRemoved = false;

        for (int j = 0; j < (int)enemies.size(); j++)
        {
            // if enemy and bullets collide
            if (CheckCollisionCircles(
                    bullets[i].GetPos(),
                    bullets[i].GetRadius(),
                    enemies[j].GetPos(),
                    enemies[j].GetRadius()))
            {
                // then take damage
                enemies[j].TakeDamage(bullets[i].GetDamage());

                // remove bullet if it collides
                bullets.erase(bullets.begin() + i);
                bulletRemoved = true;
                break;
            }
        }

        if (bulletRemoved)
            i--;
    }

    for (Enemy& enemy : enemies)
    {
        // enemy collides with player
        if (CheckCollisionCircles(
                player.GetPos(),
                player.GetRadius(),
                enemy.GetPos(),
                enemy.GetRadius()))
        {
            // player takes damage
            player.TakeDamage(1);
        }
    }
}

void World::RemoveDeadEnemies()
{
    for (int i = 0; i < (int)enemies.size(); i++)
    {
        // if enemy has been killed by collisions hitting until health = 0
        if (enemies[i].isDead())
        {
            // then remove enemy
            enemies.erase(enemies.begin() + i);
            i--;
        }
    }
}

void World::RemoveOffScreenBullets()
{
    for (int i = 0; i < (int)bullets.size(); i++)
    {
        // if bullets leave world cords
        if (bullets[i].IsOffScreen())
        {
            // remove them
            bullets.erase(bullets.begin() + i);
            i--;
        }
    }
}

// return camara data
Camera2D& World::GetCamera()
{
    return camera;
}

// return player data
const Player& World::GetPlayer() const
{
    return player;
}

// return curr player health data
int World::GetPlayerHealth() const
{
    return player.GetHealth();
}

// return player max health data
int World::GetPlayerMaxHealth() const
{
    return player.GetMaxHealth();
}

Spawner& World::GetSpawner()
{
    return spawner;
}
