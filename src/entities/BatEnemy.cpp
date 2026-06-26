#include "BatEnemy.h"
#include "utils/MovementUtils.h"
#include <raymath.h>

#include "utils/MovementUtils.h"

BatEnemy::BatEnemy(Vector2 startPos)
    : Enemy(startPos)
{
}

void BatEnemy::UpdateAI(float dt, Vector2 playerPos)
{
    // Enemy::Update() already ran shared systems (freeze/hitflash/damage
    // numbers/knockback) and already guarantees this is only called while
    // freezeTimer <= 0, so no manual checks are needed here — this is
    // purely "what makes a bat a bat": chase the player directly.

    if (Vector2 direction = Vector2Subtract(playerPos, position); Vector2Length(direction) > 0.0f)
    {
        direction = Vector2Normalize(direction);

        position.x += direction.x * speed * dt;
        position.y += direction.y * speed * dt;

        facingDirection = DirectionFromVelocity(direction, facingDirection);
    }

    animationState = AnimationState::Walk;
    animation.SetState(animationState, facingDirection);
    animation.Update(dt);
}
