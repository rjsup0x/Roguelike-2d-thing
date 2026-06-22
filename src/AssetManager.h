#pragma once

#include <raylib.h>
#include <unordered_map>
#include <string>

class AssetManager
{
public:
    static void Load();
    static void Unload();

    static const Texture2D& GetTexture(const std::string& name);

private:
    static std::unordered_map<std::string, Texture2D> textures;
};
