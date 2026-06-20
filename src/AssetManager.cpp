#include "AssetManager.h"

// define static textures
Texture2D AssetManager::PlayerTex = {};
Texture2D AssetManager::EnemyTex = {};
Texture2D AssetManager::BulletTex = {};

void AssetManager::Init()
{
    PlayerTex = LoadTexture("/Users/ry/projects/roli/src/assets/Player.png");
    EnemyTex  = LoadTexture("/Users/ry/projects/roli/src/assets/Enemy.png");
    BulletTex = LoadTexture("/Users/ry/projects/roli/src/assets/Bullet.png");
}

void AssetManager::Shutdown()
{
    UnloadTexture(PlayerTex);
    UnloadTexture(EnemyTex);
    UnloadTexture(BulletTex);
}
