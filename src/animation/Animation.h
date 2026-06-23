#pragma once

#include <animation/AnimationState.h>

#include <raylib.h>

class Animation
{
public:
    Animation(
        int frameCount = 1,
        int rowCount = 1,
        float frameTime = 0.15f
    );

    void Update(float dt);

    void SetRow(int row);

    void SetState(AnimationState state, Direction dir);

    [[nodiscard]] Rectangle GetSourceRect(const Texture2D& tex) const;

private:
    int currentFrame;
    int currentRow;

    int frameCount;
    int rowCount;

    float frameTime;
    float timer;
};
