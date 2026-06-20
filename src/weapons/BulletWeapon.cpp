#include "BulletWeapon.h"
#include "entities/Enemy.h"
#include <raymath.h>
#include <raylib.h>

BulletWeapon::BulletWeapon()
{
    fireRate = 0.15f;
    fireTimer = 0.0f;
}

void BulletWeapon::Update(float dt, Vector2 playerPos, Vector2 aimDir)
{
    fireTimer -= dt;

    if (fireTimer <= 0.0f)
    {
        fireTimer = fireRate;

        Vector2 dir = aimDir;

        if (Vector2Length(dir) > 0.0f)
            dir = Vector2Normalize(dir);

        bullets.push_back({
            playerPos,
            Vector2Scale(dir, 600.0f)
        });
    }

    for (auto& b : bullets)
        b.pos = Vector2Add(b.pos, Vector2Scale(b.vel, dt));

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
    Vector2 size = {
        (float)AssetManager::BulletTex.width,
        (float)AssetManager::BulletTex.height
    };

    for (const auto& b : bullets)
    {
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
    for (int i = 0; i < (int)bullets.size(); i++)
    {
        if (CheckCollisionCircles(
            bullets[i].GetPos(),
            bullets[i].GetRadius(),
            enemy.GetPos(),
            enemy.GetRadius()))
        {
            Vector2 hitDir = Vector2Normalize(
                Vector2Subtract(enemy.GetPos(), bullets[i].GetPos())
            );

            enemy.TakeDamage(bullets[i].GetDamage(), hitDir);

            bullets.erase(bullets.begin() + i);
            i--;
        }
    }
}
