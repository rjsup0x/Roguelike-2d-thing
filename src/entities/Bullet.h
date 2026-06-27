#pragma once

#include <raylib.h>

#include <cstdint>

#include "animation/Animation.h"
#include "animation/AnimationState.h"

class Bullet
{
public:
    Bullet(Vector2 startPos, Vector2 direction, Vector2 playerVelocity, int damage);

    void Update(float deltaTime);
    void Draw() const;

    [[nodiscard]] Vector2 GetPos() const;
    static float GetRadius() ;
    [[nodiscard]] bool IsOffScreen() const;

    [[nodiscard]] int GetDamage() const;

private:
    Vector2 position{};
    Vector2 velocity{};

    int damage{};

    float speed{};

    static constexpr float BulletRadius{5.0f};

    // animations
    static constexpr int32_t kFrameCount{5};
    static constexpr  int32_t kRowCount{1};
    static constexpr float kFrameTime{0.12f};
    Animation animation;
    AnimationState animationState{AnimationState::Idle};

    Direction facingDirection{Direction::Down};
};
