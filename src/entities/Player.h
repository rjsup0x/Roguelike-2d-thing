#pragma once

#include <raylib.h>
#include <raymath.h>

#include <memory>
#include <vector>
#include "weapons/Weapon.h"

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

    // xp functions
    void AddXP(int amount);
    int GetLevel() const;
    int GetXP() const;
    int GetXPToNextLevel() const;

private:
    Vector2 position;
    Vector2 velocity;

    float speed;

    int health;
    int maxHealth;
    // weapons is an array of weapon type
    // unique pointer meaning player can ony controll one weapon type uniquely
    std::vector<std::unique_ptr<Weapon>> weapons;

    // xp things
    int xp = 0;
    int level = 1;
    int xpToNextLevel = 100;
};
