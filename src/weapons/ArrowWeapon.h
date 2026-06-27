#pragma once

#include "weapons/Weapon.h"

#include <vector>

class ArrowWeapon : public Weapon
{
public:
    ArrowWeapon();

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
        static float GetRadius() { return 5.0f; }
    };

    std::vector<Bullet> bullets;

    float fireRate{};
    float fireTimer{};
};
