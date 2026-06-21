#pragma once

#include <raylib.h>

class Weapon
{
public:
    virtual ~Weapon() = default;

    virtual void Update(float dt, Vector2 playerPos, Vector2 aimDir) = 0;
    virtual void Draw() const = 0;
    virtual void HandleCollisions(class Enemy& enemy) = 0;
};
