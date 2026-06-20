#pragma once

#include <raylib.h>

class AssetManager
{
public:
    static void Init();
    static void Shutdown();

    static Texture2D PlayerTex;
    static Texture2D EnemyTex;
    static Texture2D BulletTex;
};
