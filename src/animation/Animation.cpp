#include "Animation.h"

Animation::Animation(
    int frameCount,
    int rowCount,
    float frameTime)
{
    this->frameCount = frameCount;
    this->rowCount = rowCount;
    this->frameTime = frameTime;

    currentFrame = 0;
    currentRow = 0;
    timer = 0.0f;
}

void Animation::Update(float dt)
{
    timer += dt;

    if (timer >= frameTime)
    {
        timer = 0.0f;

        currentFrame++;

        if (currentFrame >= frameCount)
            currentFrame = 0;
    }
}

void Animation::SetRow(int row)
{
    currentRow = row;
}

Rectangle Animation::GetSourceRect(const Texture2D& tex) const
{
    float frameWidth =
        (float)tex.width / (float)frameCount;

    float frameHeight =
        (float)tex.height / (float)rowCount;

    return {
        currentFrame * frameWidth,
        currentRow * frameHeight,
        frameWidth,
        frameHeight
    };
}
