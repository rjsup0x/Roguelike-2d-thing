#pragma once

#include <raylib.h>
#include <raymath.h>

class XPOrb
{
public:
    XPOrb(Vector2 pos, int value);

    void Update(float dt, Vector2 playerPos);
    void Draw() const;

    bool IsCollected() const;

    int GetValue() const;

private:
    Vector2 position;
    Vector2 velocity;

    int value;
    bool collected = false;
};
