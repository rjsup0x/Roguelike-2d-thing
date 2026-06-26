#include "ArrowWeapon.h"
#include "entities/Enemy.h"
#include "AssetManager.h"

#include <raymath.h>
#include <raylib.h>

ArrowWeapon::ArrowWeapon()
    : Weapon{10},
      fireRate{0.15}
{

}

void ArrowWeapon::Update(float deltaTime, Vector2 playerPos, Vector2 aimDirection)
{
    fireTimer -= deltaTime;

        bool hasTarget =
            Vector2LengthSqr(aimDirection) > 0.001f;

        // only shoot if target exists
        if (hasTarget && fireTimer <= 0.0f)
        {
            fireTimer = fireRate;

            Vector2 direction = Vector2Normalize(aimDirection);
            bullets.push_back({
                playerPos,
                Vector2Scale(aimDirection, 600.0f),
                direction
            });
        }

        // always update existing bullets
        for (auto& [pos, vel, direction] : bullets)
        {
            pos = Vector2Add(
                pos,
                Vector2Scale(vel, deltaTime)
            );
        }

        // Off-screen bounds were previously -200/3000/-200/2000, which is
        // SMALLER than the actual world bounds (3008 x 2016, set in
        // World.cpp). That meant arrows could be erased while still
        // inside the playable area near the world's right/bottom edge.
        // Widened to give comfortable margin beyond the world bounds
        // before despawning (matching the margin Bullet::IsOffScreen()
        // already uses, for consistency).
        bullets.erase(
            std::ranges::remove_if(
                bullets,
                [](const Bullet& b)
                {
                    return b.pos.x < -200 ||
                        b.pos.x > 3200 ||
                            b.pos.y < -200 ||
                                b.pos.y > 2200;
                }).begin(),
                bullets.end()
        );
}

void ArrowWeapon::Draw() const
{
    // draw bulletweapon texture
    const Texture2D& ArrowWeaponTexture = AssetManager::GetTexture("arrow_weapon");

    // for all bullets
    for (const auto& b : bullets)
    {
        // rotate bullets based on direction its facing
        float rotation = atan2(b.direction.y, b.direction.x) * RAD2DEG;

        // the source of the bullet
        Rectangle source{
            0.0f,
            0.0f,
            static_cast<float>(ArrowWeaponTexture.width),
            static_cast<float>(ArrowWeaponTexture.height)
        };

        // where it lives
        Rectangle dest{
            b.pos.x,
            b.pos.y,
            static_cast<float>(ArrowWeaponTexture.width),
            static_cast<float>(ArrowWeaponTexture.height)
        };

        Vector2 origin{
            ArrowWeaponTexture.width / 2.0f,
            ArrowWeaponTexture.height / 2.0f
        };

        DrawTexturePro(
            ArrowWeaponTexture,
            source,
            dest,
            origin,
            rotation,
            WHITE
        );
    }
}

void ArrowWeapon::HandleCollisions(Enemy& enemy)
{
    // for all bullets
    for (size_t i{}; i < bullets.size(); ++i)
    {
        // check if they collide with anything ever
        if (CheckCollisionCircles(
            bullets[i].GetPos(),
            bullets[i].GetRadius(),
            enemy.GetPos(),
            Enemy::GetRadius()))
        {
            // do bullets hit enemies
            Vector2 hitDir = Vector2Normalize(
                Vector2Subtract(enemy.GetPos(), bullets[i].GetPos())
            );

            // if so enemey takes damage
            enemy.TakeDamage(damage, hitDir);

            // if hit enemy play sound
            PlaySound(
                AssetManager::GetSound("arrow_hit")
            );

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
