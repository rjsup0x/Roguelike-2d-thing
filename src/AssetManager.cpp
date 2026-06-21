#include "AssetManager.h"
#include <raylib.h>

// define static textures
Texture2D AssetManager::PlayerTex = {};
Texture2D AssetManager::EnemyTex = {};
Texture2D AssetManager::BulletTex = {};
Texture2D AssetManager::OrbitalTex = {};

void AssetManager::Init()
{
    // init all assets
    PlayerTex = LoadTexture("/Users/ry/projects/roli/src/assets/Player.png");
    EnemyTex  = LoadTexture("/Users/ry/projects/roli/src/assets/Enemy.png");
    BulletTex = LoadTexture("/Users/ry/projects/roli/src/assets/Bullet.png");
    OrbitalTex = LoadTexture("/Users/ry/projects/roli/src/assets/Bullet.png");
}

void AssetManager::Shutdown()
{
    // remove all assets from memory
    UnloadTexture(PlayerTex);
    UnloadTexture(EnemyTex);
    UnloadTexture(BulletTex);
    UnloadTexture(OrbitalTex);
}
