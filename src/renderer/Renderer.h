#pragma once

#include <raylib.h>

class Animation;

class Renderer
{
public:
    static void DrawAnimatedTexture(
        const Texture2D& tex,
        const Animation& animation,
        Vector2 position,
        float scale = 1.0f,
        float rotation = 0.0f,
        Color tint = WHITE,
        bool flipX = false
    );
};
