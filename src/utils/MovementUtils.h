#pragma once

#include "animation/AnimationState.h"

#include <raylib.h>
#include <cmath>

// Shared helper for entities that pick a facing Direction from a velocity
// vector. Previously this exact branching logic was duplicated in
// Player::Update, Bullet::Update, and BatEnemy::Update — any fix to one
// had to be copy-pasted into the others. Centralizing it here means a bug
// fix or behavior tweak (e.g. diagonal-facing bias) only needs to happen
// once.
//
// `fallback` is returned when velocity is effectively zero (default Down,
// matching the previous per-class behavior).
inline Direction DirectionFromVelocity(Vector2 velocity, Direction fallback = Direction::Down)
{
    constexpr float kEpsilon = 0.0001f;

    if (fabsf(velocity.x) < kEpsilon && fabsf(velocity.y) < kEpsilon)
        return fallback;

    if (fabsf(velocity.x) > fabsf(velocity.y))
        return (velocity.x > 0.0f) ? Direction::Right : Direction::Left;

    return (velocity.y > 0.0f) ? Direction::Down : Direction::Up;
}
