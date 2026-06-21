#pragma once

#include <raylib.h>

class AssetManager
{
public:
    static void Init();
    static void Shutdown();

    // assets
    static Texture2D PlayerTex;
    static Texture2D EnemyTex;
    static Texture2D BulletTex;
    static Texture2D OrbitalTex;
};
