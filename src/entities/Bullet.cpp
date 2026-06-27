#include "Bullet.h"
#include "AssetManager.h"
#include "animation/Animation.h"
#include "animation/AnimationState.h"
#include "renderer/Renderer.h"
#include "utils/MovementUtils.h"

#include <raylib.h>
#include <raymath.h>

// bullet constructor
Bullet::Bullet(Vector2 startPos, Vector2 direction, Vector2 playerVelocity, int damage)
    : position{startPos},
      damage{damage},
      speed{600.0f},
      animation{kFrameCount, kRowCount, kFrameTime}
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

    // update animation
    if (Vector2Length(velocity) > 0.0f)
    {
        animationState = AnimationState::Walk;
        facingDirection = DirectionFromVelocity(velocity, facingDirection);
    }
    else
    {
        // otherwise idle
        animationState = AnimationState::Idle;
    }

    animation.SetState(animationState, facingDirection);
    animation.Update(deltaTime);
}

void Bullet::Draw() const
{
    // bullet texture
    const Texture2D& BulletWeaponTexture = AssetManager::GetTexture("bullet_weapon");

    Renderer::DrawAnimatedTexture(
        BulletWeaponTexture,
        animation,
        position,
        0.0f,
        0.0
    );
}

int Bullet::GetDamage() const
{
    return damage;
}

Vector2 Bullet::GetPos() const { return position; }

float Bullet::GetRadius() { return BulletRadius; }

// check if the bullet goes of world space
bool Bullet::IsOffScreen() const
{
    return position.x < -200 ||
           position.y < -200 ||
           position.x > 3200 ||
           position.y > 2200;
}
