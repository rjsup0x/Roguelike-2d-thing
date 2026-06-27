#pragma once

#include <raylib.h>

#include <memory>
#include <vector>
#include <functional>

#include "weapons/Weapon.h"
#include "animation/Animation.h"
#include "animation/AnimationState.h"

class Player
{
public:
    Player();

    void Update(float dt, Vector2 aimDirection);
    void Draw() const;

    [[nodiscard]] Vector2 GetPos() const;
    void SetPos(Vector2 newPos);

    static float GetRadius() ;

    [[nodiscard]] int GetHealth() const;
    [[nodiscard]] int GetMaxHealth() const;

    void TakeDamage(int amount);
    [[nodiscard]] bool isDead() const;

    [[nodiscard]] Vector2 GetVelocity() const;

    // only inspects weapons doesnt change them
    [[nodiscard]] const std::vector<std::unique_ptr<Weapon>>& GetWeapons() const;
    // to add weapons
    void AddWeapon(std::unique_ptr<Weapon> weapon);

    void AddXP(int amount);
    [[nodiscard]] int GetLevel() const;
    [[nodiscard]] int GetXP() const;
    [[nodiscard]] int GetXPToNextLevel() const;

    void SetLevelUpCallback(std::function<void(int)> callback);

    void IncreaseMaxHealth(int amount);
    void IncreaseDamage(int amount);

    [[nodiscard]] int GetDamageBonus() const { return damageBonus; }

    [[nodiscard]] int GetDamage() const;

private:
    Vector2 position{};
    Vector2 velocity{};
    float scale{1.5f};

    float speed{};
    static constexpr float Radius{16.0f};

    int health{};
    int maxHealth{};

    std::vector<std::unique_ptr<Weapon>> weapons;

    int xp{};
    int level{1};
    int xpToNextLevel{100};

    static constexpr int32_t kFrameCount{4};
    static constexpr int32_t kRowCount{8};
    static constexpr float kFrameTime{0.12f};
    Animation animation;
    AnimationState animationState {AnimationState::Idle};

    Direction facingDirection {Direction::Down};

    std::function<void(int)> onLevelUp;

    int baseDamage{10};
    int damageBonus{};
};
