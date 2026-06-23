#include "BulletWeapon.h"
#include "entities/Enemy.h"
#include "AssetManager.h"

#include <raymath.h>
#include <raylib.h>

BulletWeapon::BulletWeapon()
    : Weapon{10},
      fireRate{0.15},
      fireTimer{0.0f}
{

}

void BulletWeapon::Update(float dt, Vector2 playerPos, Vector2 aimDir)
{
    fireTimer -= dt;

        bool hasTarget =
            Vector2LengthSqr(aimDir) > 0.001f;

        // only shoot if target exists
        if (hasTarget && fireTimer <= 0.0f)
        {
            fireTimer = fireRate;

            bullets.push_back({
                playerPos,
                Vector2Scale(aimDir, 600.0f)
            });
        }

        // always update existing bullets
        for (auto& b : bullets)
        {
            b.pos = Vector2Add(
                b.pos,
                Vector2Scale(b.vel, dt)
            );
        }

        bullets.erase(
            std::ranges::remove_if(bullets
                                   ,
                                   [](const Bullet& b)
                                   {
                                       return b.pos.x < -200 ||
                                              b.pos.x > 3000 ||
                                              b.pos.y < -200 ||
                                              b.pos.y > 2000;
                                   }).begin(),
            bullets.end()
        );
}

void BulletWeapon::Draw() const
{
    // draw bulletweapon texture
    const Texture2D& BulletWeaponTexture = AssetManager::GetTexture("bullet_weapon");
    Vector2 size = {
        static_cast<float>(BulletWeaponTexture.width),
        static_cast<float>(BulletWeaponTexture.height)
    };

    // for all bullets
    for (const auto& b : bullets)
    {
        // draw them with texture and location
        DrawTextureV(
            BulletWeaponTexture,
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
    for (size_t i{}; i < bullets.size(); ++i)
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

            // replace with
            // auto it = bullets.begin();

            // while (it != bullets.end())
            // {
            //     if (collision)
            //     {
            //         it = bullets.erase(it);
            //     }
            //     else
            //     {
            //         ++it;
            //     }
            // }
        }
    }
}
