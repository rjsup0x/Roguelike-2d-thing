#pragma once

#include <raylib.h>

class Weapon
{
public:
    explicit Weapon(int damage)
        : damage{damage}
    {

    }

    virtual ~Weapon() = default;

    virtual void Update(float dt, Vector2 playerPos, Vector2 aimDir) = 0;
    virtual void Draw() const = 0;
    virtual void HandleCollisions(class Enemy& enemy) = 0;

    // weapon holds the base damage numbers
    virtual void SetDamage(int newDamage)
    {
        damage = newDamage;
    }

    int GetDamage() const
    {
        return damage;
    }

protected:
    int damage{};
};
