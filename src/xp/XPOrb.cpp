#include "XPOrb.h"
#include <raylib.h>
#include <raymath.h>

#include "AssetManager.h"

XPOrb::XPOrb(const Vector2 pos, const int value)
{
    this->position = pos;
    this->value = value;
    velocity = {0, 0};
}

void XPOrb::Update(const float deltaTime, const Vector2 playerPos)
{
    Vector2 dir = Vector2Subtract(playerPos, position);
    const float dist = Vector2Length(dir);

    // attraction radius (Vampire Survivors feel)
    if (dist < 120.0f)
    {
        dir = Vector2Normalize(dir);
        velocity = Vector2Add(velocity, Vector2Scale(dir, 600.0f * deltaTime));
    }

    position = Vector2Add(position, Vector2Scale(velocity, deltaTime));

    // auto collect
    if (dist < 10.0f)
        collected = true;
}

void XPOrb::Draw() const
{
    const Texture2D& coin = AssetManager::GetTexture("coin");

    const Rectangle src = {
        0.0f, 0.0f,
        static_cast<float>(coin.width),
        static_cast<float>(coin.height)
    };

    const Rectangle dest = {
        position.x,
        position.y,
        24.0f,
        24.0f
    };

    constexpr Vector2 origin = { 12.0f, 12.0f };

    DrawTexturePro(
        coin,
        src,
        dest,
        origin,
        0.0f,
        WHITE
    );
}

bool XPOrb::IsCollected() const
{


    return collected;
}

int XPOrb::GetValue() const
{
    return value;
}
