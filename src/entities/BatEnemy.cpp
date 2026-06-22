#include "BatEnemy.h"
#include <raymath.h>

BatEnemy::BatEnemy(Vector2 startPos)
    : Enemy(startPos)
{
}

void BatEnemy::Update(float dt, Vector2 playerPos)
{
    // run shared enemy systems first
    Enemy::Update(dt, playerPos);

    if (freezeTimer > 0.0f)
        return;

    // --- BAT AI (your old Enemy::Update AI part) ---

    Vector2 dir = Vector2Subtract(playerPos, position);

    if (Vector2Length(dir) > 0.0f)
    {
        dir = Vector2Normalize(dir);

        position.x += dir.x * speed * dt;
        position.y += dir.y * speed * dt;

        // optional facing
        if (fabs(dir.x) > fabs(dir.y))
            facingDirection = (dir.x > 0) ? Direction::Right : Direction::Left;
        else
            facingDirection = (dir.y > 0) ? Direction::Down : Direction::Up;
    }

    animationState = AnimationState::Walk;
    animation.SetState(animationState, facingDirection);
    animation.Update(dt);
}
