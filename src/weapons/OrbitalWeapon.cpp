#include "OrbitalWeapon.h"
#include "entities/Enemy.h"
#include "AssetManager.h"
#include "weapons/Weapon.h"

#include <raylib.h>
#include <raymath.h>

OrbitalWeapon::OrbitalWeapon()
    : Weapon{15},
      orbitRadius{80.0f},
      rotationSpeed{180.0f}
{
    orbitalPositions.resize(4);
}

void OrbitalWeapon::Update(float deltaTime, Vector2 playerPos, Vector2 aimDir)
{
    // 0 += 180 * dt
    // spin 180 degrees * delta time
    currentAngle += rotationSpeed * deltaTime;

    //
    float baseAngle = currentAngle * DEG2RAD;

    for (int i = 0; i < 4; i++)
    {
        const float angle = baseAngle + ((PI / 2.0f) * static_cast<float>(i));

        orbitalPositions[i] =
        {
            playerPos.x + cosf(angle) * orbitRadius,
            playerPos.y + sinf(angle) * orbitRadius
        };
    }
}

void OrbitalWeapon::Draw() const
{
    // TEMP CIRCLES
    /*
    for (const auto& pos : orbitalPositions)
    {
        DrawCircleV(pos, 12.0f, PURPLE);
    }
    */

    // TEXTURE for orbital weapon
    const Texture2D& OrbitalTexture = AssetManager::GetTexture("orbital_weapon");

    Vector2 size =
    {
        static_cast<float>(OrbitalTexture.width),
        static_cast<float>(OrbitalTexture.height)
    };

    for (const auto& pos : orbitalPositions)
    {
        DrawTextureV(
            OrbitalTexture,
            {
                pos.x - size.x / 2.0f,
                pos.y - size.y / 2.0f
            },
            WHITE
        );
    }
}


void OrbitalWeapon::HandleCollisions(Enemy& enemy)
{
    for (const auto& pos : orbitalPositions)
    {
        if (CheckCollisionCircles(
                pos,
                12.0f,
                enemy.GetPos(),
                Enemy::GetRadius()))
        {
            Vector2 hitDir = Vector2Normalize(
                Vector2Subtract(
                    enemy.GetPos(),
                    pos
                )
            );

            // when that weapon collides with enemy they take the damage
            enemy.TakeDamage(damage, hitDir); }
    }
}
