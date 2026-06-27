#include "Renderer.h"
#include "animation/Animation.h"
#include <raymath.h>

void Renderer::DrawAnimatedTexture(
    const Texture2D& tex,
    const Animation& animation,
    const Vector2 position,
    const float scale,
    const float rotation,
    const Color tint,
    const bool flipX)
{
    Rectangle source =
        animation.GetSourceRect(tex);

    if (flipX)
    {
        source.width *= -1;
    }

    const Rectangle dest =
    {
        position.x,
        position.y,
        fabsf(source.width) * scale,
        source.height * scale
    };

    const Vector2 origin =
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
