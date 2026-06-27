#pragma once

#include <raylib.h>

class XPOrb
{
public:
    XPOrb(Vector2 pos, int value);

    void Update(float deltaTime, Vector2 playerPos);
    void Draw() const;

    [[nodiscard]] bool IsCollected() const;

    [[nodiscard]] int GetValue() const;

    float spinTime{0.0f};

private:
    Vector2 position{};
    Vector2 velocity{};

    int value;
    bool collected = false;
};
