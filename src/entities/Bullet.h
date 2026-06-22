#pragma once

#include <raylib.h>
#include <raymath.h>

#include "animation/Animation.h"
#include "animation/AnimationState.h"

class Bullet
{
public:
    Bullet(Vector2 startPos, Vector2 direction, Vector2 playerVelocity, int damage);

    void Update(float deltaTime);
    void Draw() const;

    Vector2 GetPos() const;
    float GetRadius() const;
    bool IsOffScreen() const;

    int GetDamage() const;

private:
    Vector2 position{};
    Vector2 velocity{};

    int damage{};

    float speed{};

    // anims
    Animation animation;

    AnimationState animationState{AnimationState::Idle};

    Direction facingDirection{Direction::Down};
};
