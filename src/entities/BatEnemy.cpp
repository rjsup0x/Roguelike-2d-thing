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

    if (Vector2 direction = Vector2Subtract(playerPos, position); Vector2Length(direction) > 0.0f)
    {
        direction = Vector2Normalize(direction);

        position.x += direction.x * speed * dt;
        position.y += direction.y * speed * dt;

        // optional facing
        if (fabs(direction.x) > fabs(direction.y))
            facingDirection = (direction.x > 0) ? Direction::Right : Direction::Left;
        else
            facingDirection = (direction.y > 0) ? Direction::Down : Direction::Up;
    }

    animationState = AnimationState::Walk;
    animation.SetState(animationState, facingDirection);
    animation.Update(dt);
}
