#include "AssetManager.h"
#include <raylib.h>

void AssetManager::Load()
{
    // init all assets

    // menu background
    textures["menu_background"] = LoadTexture("/Users/ry/projects/roli/src/assets/backgroundImg.png");
    // characters
    textures["player"] = LoadTexture("/Users/ry/projects/roli/src/assets/Player.png");
    textures["enemy"]  = LoadTexture("/Users/ry/projects/roli/src/assets/Enemy.png");
    // weapons
    textures["bullet_weapon"] = LoadTexture("/Users/ry/projects/roli/src/assets/Bullet.png");
    textures["orbital_weapon"] = LoadTexture("/Users/ry/projects/roli/src/assets/Bullet.png");
    // world

    // anything else
}

void AssetManager::Unload()
{
    // remove all assets from memory
    for (auto& [name, tex] : textures)
    {
        UnloadTexture(tex);
    }
    textures.clear();
}

const Texture2D& AssetManager::GetTexture(const std::string& name)
{
    return textures.at(name);
}

std::unordered_map<std::string, Texture2D> AssetManager::textures;
