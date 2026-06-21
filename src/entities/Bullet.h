#pragma once

#include <raylib.h>
#include <raymath.h>

#include "animation/Animation.h"
#include "animation/AnimationState.h"

class Bullet
{
public:
    Bullet(Vector2 startPos, Vector2 direction, Vector2 playerVelocity);

    void Update(float deltaTime);
    void Draw() const;

    Vector2 GetPos() const;
    float GetRadius() const;
    int GetDamage() const;
    bool IsOffScreen() const;

private:
    Vector2 position;
    Vector2 velocity;

    float speed;

    // anims
    Animation animation;

    AnimationState animationState = AnimationState::Idle;

    Direction facingDirection = Direction::Down;
};
