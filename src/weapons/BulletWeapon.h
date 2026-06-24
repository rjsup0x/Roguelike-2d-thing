#pragma once

#include "weapons/Weapon.h"

#include <vector>

class BulletWeapon : public Weapon
{
public:
    BulletWeapon();

    void Update(float deltaTime, Vector2 playerPos, Vector2 aimDirection) override;
    void Draw() const override;
    void HandleCollisions(class Enemy& enemy) override;

private:
    struct Bullet
    {
        Vector2 pos{};
        Vector2 vel{};
        Vector2 direction{};


        [[nodiscard]] Vector2 GetPos() const { return pos; }
        [[nodiscard]] float GetRadius() const { return 5.0f; }
    };

    std::vector<Bullet> bullets;

    float fireRate{};
    float fireTimer{};
};
