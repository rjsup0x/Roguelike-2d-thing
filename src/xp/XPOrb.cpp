#include "XPOrb.h"
#include <raylib.h>
#include <raymath.h>

XPOrb::XPOrb(Vector2 pos, int value)
{
    position = pos;
    this->value = value;
    velocity = {0, 0};
}

void XPOrb::Update(float dt, Vector2 playerPos)
{
    Vector2 dir = Vector2Subtract(playerPos, position);
    float dist = Vector2Length(dir);

    // attraction radius (Vampire Survivors feel)
    if (dist < 120.0f)
    {
        dir = Vector2Normalize(dir);
        velocity = Vector2Add(velocity, Vector2Scale(dir, 600.0f * dt));
    }

    position = Vector2Add(position, Vector2Scale(velocity, dt));

    // auto collect
    if (dist < 10.0f)
        collected = true;
}

void XPOrb::Draw() const
{
    DrawCircleV(position, 4, GREEN);
}

bool XPOrb::IsCollected() const
{
    return collected;
}

int XPOrb::GetValue() const
{
    return value;
}
