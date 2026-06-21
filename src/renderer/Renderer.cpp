#include "Renderer.h"
#include "animation/Animation.h"
#include <raymath.h>

void Renderer::DrawAnimatedTexture(
    const Texture2D& tex,
    const Animation& animation,
    Vector2 position,
    float scale,
    float rotation,
    Color tint,
    bool flipX)
{
    Rectangle source =
        animation.GetSourceRect(tex);

    if (flipX)
    {
        source.width *= -1;
    }

    Rectangle dest =
    {
        position.x,
        position.y,
        fabsf(source.width) * scale,
        source.height * scale
    };

    Vector2 origin =
    {
        dest.width * 0.5f,
        dest.height * 0.5f
    };

    DrawTexturePro(
        tex,
        source,
        dest,
        origin,
        rotation,
        tint
    );
}
