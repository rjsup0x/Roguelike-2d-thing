#pragma once

#include <raylib.h>
#include <raymath.h>

// camera screen to world space conversion
inline Vector2 ScreenToWorld(Camera2D camera, Vector2 mouse)
{
    // transform from screenspace to world space
    Matrix mat = GetCameraMatrix2D(camera);
    Matrix inv = MatrixInvert(mat);
    return Vector2Transform(mouse, inv);
}
