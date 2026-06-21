#pragma once

#include <raylib.h>

class Weapon
{
public:
    virtual ~Weapon() = default;

    // virtual functions can be overriden
    // therefore different weapons inheriting this class can oevrride their update and draw
    virtual void Update(float dt, Vector2 playerPos, Vector2 aimDir) = 0;
    virtual void Draw() const = 0;

    // weapon colliding with stuff
    virtual void HandleCollisions(class Enemy& enemy) = 0;
};
