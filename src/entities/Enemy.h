#pragma once

#include "raylib.h"
#include <vector>
#include <raymath.h>

#include "animation/Animation.h"
#include "animation/AnimationState.h"

struct DamageNumber
{
    int value{};
    Vector2 pos{};
    float timer{};
    float alpha{};
};

class Enemy
{
public:
    Enemy(Vector2 startPos);
    virtual ~Enemy() = default;

    virtual void Update(float dt, Vector2 playerPos);
    virtual void Draw() const;

    // shared API
    Vector2 GetPos() const;
    void SetPos(Vector2 newPos);

    float GetRadius() const;

    void TakeDamage(int amount, Vector2 hitDir);
    bool isDead() const;

    int GetHealth() const;
    void SetStats(int hp, float spd);

protected:
    // shared helper for subclasses
    void UpdateCommon(float dt);

protected:
    Vector2 position{};
    Vector2 velocity{};

    float speed{100.0f};

    static constexpr float Radius{16.0f};

    int health{50};
    int maxHealth{50};

    float hitFlashTimer{};
    float freezeTimer{};

    std::vector<DamageNumber> damageNumbers;

    Animation animation;
    AnimationState animationState{AnimationState::Idle};

    Direction facingDirection{Direction::Down};
};
