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

private:
    Vector2 position;
    Vector2 velocity;

    float speed;

    int health;
    int maxHealth;
    std::vector<std::unique_ptr<Weapon>> weapons;
};
