#include "BatEnemy.h"
#include "utils/MovementUtils.h"
#include <raymath.h>

BatEnemy::BatEnemy(Vector2 startPos)
    : Enemy(startPos)
{
}

void BatEnemy::UpdateAI(float deltaTime, Vector2 playerPos)
{
    // Enemy::Update() already ran shared systems (freeze/hit flash/damage
    // numbers/knockback) and already guarantees this is only called while
    // freezeTimer <= 0, so no manual checks are needed here — this is
    // purely "what makes a bat": chase the player directly.

    if (Vector2 direction = Vector2Subtract(playerPos, position); Vector2Length(direction) > 0.0f)
    {
        direction = Vector2Normalize(direction);

        position.x += direction.x * speed * deltaTime;
        position.y += direction.y * speed * deltaTime;

        facingDirection = DirectionFromVelocity(direction, facingDirection);
    }

    animationState = AnimationState::Walk;
    animation.SetState(animationState, facingDirection);
    animation.Update(deltaTime);
}
