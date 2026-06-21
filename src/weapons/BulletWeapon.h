#pragma once

#include "weapons/Weapon.h"

#include <vector>

// bulletWeapon is inhereting weapon
class BulletWeapon : public Weapon
{
public:
    BulletWeapon();

    // overriding from weapon
    void Update(float dt, Vector2 playerPos, Vector2 aimDir) override;
    void Draw() const override;

    // check collisions weapon vs enemy
    void HandleCollisions(class Enemy& enemy) override;

private:
    // bullet data for bullet weapon
    struct Bullet
    {
        Vector2 pos;
        Vector2 vel;

        int damage = 10;

        Vector2 GetPos() const { return pos; }
        float GetRadius() const { return 5.0f; }
        int GetDamage() const { return damage; }
    };

    std::vector<Bullet> bullets;

    float fireRate;
    float fireTimer;
};
