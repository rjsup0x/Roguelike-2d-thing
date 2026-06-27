#include "TreeEnemy.h"

#include <raymath.h>

#include "utils/MovementUtils.h"

TreeEnemy::TreeEnemy(const Vector2 startPos)
    : Enemy(startPos)
{

}

void TreeEnemy::UpdateAI(const float deltaTime, const Vector2 playerPos)
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