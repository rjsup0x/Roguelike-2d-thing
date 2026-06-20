#pragma once

#include "weapons/Weapon.h"
#include "AssetManager.h"

#include <vector>
#include <algorithm>

class BulletWeapon : public Weapon
{
public:
    BulletWeapon();

    void Update(float dt, Vector2 playerPos, Vector2 aimDir) override;
    void Draw() const override;

    void HandleCollisions(class Enemy& enemy) override;

private:
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
