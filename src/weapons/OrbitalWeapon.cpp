#include "OrbitalWeapon.h"
#include "entities/Enemy.h"
#include "AssetManager.h"

OrbitalWeapon::OrbitalWeapon()
    : orbitRadius(80.0f),
      rotationSpeed(180.0f),
      currentAngle(0.0f)
{
    // each type of weapon inits its own damage which overrides (weapon.h) damage
    damage = 15;
    orbitalPositions.resize(4);
}

void OrbitalWeapon::Update(float deltaTime, Vector2 playerPos, Vector2 aimDir)
{
    // 0 += 180 * dt
    // spin 180 degrees * deltatime
    currentAngle += rotationSpeed * deltaTime;

    //
    float baseAngle = currentAngle * DEG2RAD;

    for (int i = 0; i < 4; i++)
    {
        float angle = baseAngle + ((PI / 2.0f) * i);

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

    // TEXTURE VERSION
    Vector2 size =
    {
        (float)AssetManager::OrbitalTex.width,
        (float)AssetManager::OrbitalTex.height
    };

    for (const auto& pos : orbitalPositions)
    {
        DrawTextureV(
            AssetManager::OrbitalTex,
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
                enemy.GetRadius()))
        {
            Vector2 hitDir = Vector2Normalize(
                Vector2Subtract(
                    enemy.GetPos(),
                    pos
                )
            );

            // when that weapon collides with enemy they take the damage
            enemy.TakeDamage(damage, hitDir);        }
    }
}
