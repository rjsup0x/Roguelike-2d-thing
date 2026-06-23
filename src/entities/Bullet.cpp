#include "Bullet.h"
#include "AssetManager.h"
#include "animation/Animation.h"
#include "animation/AnimationState.h"
#include "renderer/Renderer.h"

#include <raylib.h>
#include <raymath.h>

// bullet constructor
Bullet::Bullet(Vector2 startPos, Vector2 direction, Vector2 playerVelocity, int damage)
    : position{startPos},
      damage{damage},
      speed{600.0f},
      animation{5, 1, 0.12f}
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

        if (fabs(velocity.x) > fabs(velocity.y))
        {
            // is facing right of left
            if (velocity.x > 0.0f)
                facingDirection = Direction::Right;
            else
                facingDirection = Direction::Left;
        }
        else
        {
            // is facing up or down
            if (velocity.y > 0.0f)
                facingDirection = Direction::Down;
            else
                facingDirection = Direction::Up;
        }
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
        1.0f,
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
