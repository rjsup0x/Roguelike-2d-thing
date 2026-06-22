#pragma once

#include "raylib.h"

#include <raymath.h>
#include <vector>

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

    void Update(float deltaTime, Vector2 playerPos);
    void Draw() const;

    Vector2 GetPos() const;
    void SetPos(Vector2 newPos);

    float GetRadius() const;

    void TakeDamage(int amount, Vector2 hitDir);
    bool isDead() const;

    int GetHealth() const;
    void SetStats(int hp, float spd);

private:
    Vector2 position{};
    Vector2 velocity{};
    float scale{1.0f};
    float roatation{};

    float speed{};

    static constexpr float Radius{16.0f};

    int health{};
    int maxHealth{};

    float separationRadius{28.0f};

    float rotation{};
    bool facingLeft{};

    float hitFlashTimer{};
    float freezeTimer{};

    std::vector<DamageNumber> damageNumbers;

    // anims
    Animation animation;
    AnimationState animationState{AnimationState::Idle};

    Direction facingDirection{Direction::Down};
};
