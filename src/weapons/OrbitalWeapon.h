#pragma once

#include "weapons/Weapon.h"
#include "AssetManager.h"

#include <raylib.h>
#include <vector>

class OrbitalWeapon : public Weapon
{
    public:
        OrbitalWeapon();

        // overriding from weapon
        void Update(float dt, Vector2 playerPos, Vector2 aimDir) override;
        void Draw() const override;

        // check collisions weapon vs enemy
        void HandleCollisions(class Enemy& enemy) override;
    private:
        float orbitRadius;
        float rotationSpeed;
        float currentAngle;

        Vector2 playerPos;

        std::vector<Vector2> orbitalPositions;
};
