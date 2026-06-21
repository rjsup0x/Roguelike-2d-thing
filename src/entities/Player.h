#pragma once

#include <raylib.h>
#include <raymath.h>

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

    void Update(float dt, Vector2 aimDir);
    void Draw() const;

    Vector2 GetPos() const;
    void SetPos(Vector2 newPos);

    float GetRadius() const;

    int GetHealth() const;
    int GetMaxHealth() const;

    void TakeDamage(int amount);
    bool isDead() const;

    Vector2 GetVelocity() const;

    std::vector<std::unique_ptr<Weapon>>& GetWeapons();

    void AddXP(int amount);
    int GetLevel() const;
    int GetXP() const;
    int GetXPToNextLevel() const;

    void SetLevelUpCallback(std::function<void(int)> callback);

    void IncreaseMaxHealth(int amount);
    void IncreaseDamage(int amount);

    int GetDamageBonus() const { return damageBonus; }

    int GetDamage() const;

private:
    Vector2 position;
    Vector2 velocity;

    float speed;

    int health;
    int maxHealth;

    std::vector<std::unique_ptr<Weapon>> weapons;

    int xp = 0;
    int level = 1;
    int xpToNextLevel = 100;

    Animation animation;

    AnimationState animationState = AnimationState::Idle;

    Direction facingDirection = Direction::Down;

    std::function<void(int)> onLevelUp;

    int baseDamage = 10;
    int damageBonus = 0;
};
