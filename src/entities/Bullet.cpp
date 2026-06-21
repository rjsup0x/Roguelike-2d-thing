#include "Bullet.h"
#include "AssetManager.h"

#include <raylib.h>
#include <raymath.h>

// bullet constructor
Bullet::Bullet(Vector2 startPos, Vector2 direction, Vector2 playerVelocity)
    : position(startPos),
      speed(600.0f)
{
    Vector2 dirNorm = Vector2Normalize(direction);

    velocity = Vector2Add(
        Vector2Scale(dirNorm, speed),
        playerVelocity
    );
}

void Bullet::Update(float deltaTime)
{
    // update bullet position constant time
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
}

void Bullet::Draw() const
{
    // add texture to bullets as size
    Vector2 size = {
        (float)AssetManager::BulletTex.width,
        (float)AssetManager::BulletTex.height
    };

    // draw the bullet with texture as pos attributes
    DrawTextureV(
        AssetManager::BulletTex,
        {
            position.x - size.x / 2.0f,
            position.y - size.y / 2.0f
        },
        WHITE
    );
}

Vector2 Bullet::GetPos() const { return position; }

float Bullet::GetRadius() const { return 5.0f; }

int Bullet::GetDamage() const { return 10; }

// check if the bullet goes of world space
bool Bullet::IsOffScreen() const
{
    return position.x < -200 ||
           position.y < -200 ||
           position.x > 3200 ||
           position.y > 2200;
}
