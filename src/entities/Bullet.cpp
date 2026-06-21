#include "Bullet.h"
#include "AssetManager.h"
#include "animation/Animation.h"
#include "animation/AnimationState.h"
#include "renderer/Renderer.h"

#include <raylib.h>
#include <raymath.h>

// bullet constructor
Bullet::Bullet(Vector2 startPos, Vector2 direction, Vector2 playerVelocity, int damage)
    : position(startPos),
      speed(600.0f),
      animation(5, 1, 0.12f),
      damage(damage)
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
            if (velocity.x > 0)
                facingDirection = Direction::Right;
            else
                facingDirection = Direction::Left;
        }
        else
        {
            // is facing up or down
            if (velocity.y > 0)
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

    // update anim state
    int row = 0;

    switch (animationState)
    {
        case AnimationState::Idle:

            switch (facingDirection)
            {
                case Direction::Down:  row = 0; break;
                case Direction::Left:  row = 1; break;
                case Direction::Right: row = 2; break;
                case Direction::Up:    row = 3; break;
            }

            break;

        case AnimationState::Walk:

            switch (facingDirection)
            {
                case Direction::Down:  row = 4; break;
                case Direction::Left:  row = 5; break;
                case Direction::Right: row = 6; break;
                case Direction::Up:    row = 7; break;
            }

            break;

        case AnimationState::Attack:

            switch (facingDirection)
            {
                case Direction::Down:  row = 8; break;
                case Direction::Left:  row = 9; break;
                case Direction::Right: row = 10; break;
                case Direction::Up:    row = 11; break;
            }

            break;

        case AnimationState::Hurt:

            switch (facingDirection)
            {
                case Direction::Down:  row = 12; break;
                case Direction::Left:  row = 13; break;
                case Direction::Right: row = 14; break;
                case Direction::Up:    row = 15; break;
            }

            break;

        case AnimationState::Death:

            row = 16;
            break;
    }

    animation.SetRow(row);
    animation.Update(deltaTime);
}

void Bullet::Draw() const
{
    Renderer::DrawAnimatedTexture(
        AssetManager::BulletTex,
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

float Bullet::GetRadius() const { return 5.0f; }

// check if the bullet goes of world space
bool Bullet::IsOffScreen() const
{
    return position.x < -200 ||
           position.y < -200 ||
           position.x > 3200 ||
           position.y > 2200;
}
