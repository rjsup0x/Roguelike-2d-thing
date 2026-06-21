#include "BulletWeapon.h"
#include "entities/Enemy.h"
#include "AssetManager.h"

#include <raymath.h>
#include <raylib.h>

BulletWeapon::BulletWeapon()
    : fireRate(0.15),
      fireTimer(0.0f)
{
    // Another weapon another update of (weapon.h) damage value
    // damage = 10;
}

void BulletWeapon::Update(float dt, Vector2 playerPos, Vector2 aimDir)
{
    fireTimer -= dt;

    // controlling fire timing
    if (fireTimer <= 0.0f)
    {
        fireTimer = fireRate;

        Vector2 dir = aimDir;

        // normalize the direction (straight shoot from when fired)
        if (Vector2Length(dir) > 0.0f)
            dir = Vector2Normalize(dir);

        // store playerpos and scale in bullets
        bullets.push_back({
            playerPos,
            Vector2Scale(dir, 600.0f),
        });
    }

    // for all bullets
    for (auto& b : bullets)
        b.pos = Vector2Add(b.pos, Vector2Scale(b.vel, dt));

    // removing bullets from array once shot
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b)
        {
            return b.pos.x < -200 || b.pos.x > 3000 ||
                   b.pos.y < -200 || b.pos.y > 2000;
        }),
        bullets.end());
}

void BulletWeapon::Draw() const
{
    // using asset manager to draw texture to bullet
    Vector2 size = {
        (float)AssetManager::BulletTex.width,
        (float)AssetManager::BulletTex.height
    };

    // for all bullets
    for (const auto& b : bullets)
    {
        // draw them with texture and location
        DrawTextureV(
            AssetManager::BulletTex,
            {
                b.pos.x - size.x / 2.0f,
                b.pos.y - size.y / 2.0f
            },
            WHITE
        );
    }
}

void BulletWeapon::HandleCollisions(Enemy& enemy)
{
    // for all bullets
    for (int i = 0; i < (int)bullets.size(); i++)
    {
        // check if they collide with anything ever
        if (CheckCollisionCircles(
            bullets[i].GetPos(),
            bullets[i].GetRadius(),
            enemy.GetPos(),
            enemy.GetRadius()))
        {
            // do bullets hit enemies
            Vector2 hitDir = Vector2Normalize(
                Vector2Subtract(enemy.GetPos(), bullets[i].GetPos())
            );

            // if so enemey takes damage
            enemy.TakeDamage(damage, hitDir);

            // remove the bullet from data
            bullets.erase(bullets.begin() + i);
            i--;
        }
    }
}
