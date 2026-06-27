#pragma once

#include "raylib.h"
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
    virtual ~Enemy() = default;

    // Runs shared systems (freeze timer, hit flash, damage numbers,
    // knockback integration) every frame, then calls UpdateAI() for
    // subclass-specific behavior — but only when not frozen, so each
    // enemy type doesn't need to re-check freezeTimer itself.
    // This is intentionally NOT virtual anymore: subclasses customize
    // behavior via UpdateAI(), not by overriding Update() wholesale,
    // so the shared systems can never accidentally be skipped or
    // duplicated by a subclass.
    void Update(float dt, Vector2 playerPos);

    virtual void Draw() const;

    // shared API
    [[nodiscard]] Vector2 GetPos() const;
    void SetPos(Vector2 newPos);

    static float GetRadius() ;

    void TakeDamage(int amount, Vector2 hitDir);
    [[nodiscard]] bool isDead() const;

    [[nodiscard]] int GetHealth() const;
    void SetStats(int hp, float spd);

protected:
    // Subclass-specific AI/movement/animation logic. Called from Update()
    // once per frame, only while the enemy is not frozen (freezeTimer <= 0).
    // This is where BatEnemy (and future enemy types) implement what makes
    // them unique, without needing to duplicate the freeze/hitflash/
    // knockback/damage-number handling that's shared by all enemies.
    virtual void UpdateAI(float dt, Vector2 playerPos) = 0;

    Vector2 position{};
    Vector2 velocity{};

    float speed{100.0f};

    static constexpr float Radius{16.0f};

    int health{50};
    int maxHealth{50};

    float hitFlashTimer{};
    float freezeTimer{};

    std::vector<DamageNumber> damageNumbers;

    // animation things - only used for enemies
    static constexpr int32_t kFrameCount{8};
    static constexpr int32_t kRowCount{1};
    static constexpr float_t kFrameTime{0.12f};
    Animation animation;
    AnimationState animationState{AnimationState::Idle};

    Direction facingDirection{Direction::Down};
};
