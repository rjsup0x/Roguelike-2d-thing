#pragma once

#include <raylib.h>
#include <raymath.h>

// camera screen to world space conversion
inline Vector2 ScreenToWorld(const Camera2D &camera, const Vector2 mouse)
{
    // transform from screenspace to world space
    const Matrix mat = GetCameraMatrix2D(camera);
    const Matrix inv = MatrixInvert(mat);
    return Vector2Transform(mouse, inv);
}
