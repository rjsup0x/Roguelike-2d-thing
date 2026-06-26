#pragma once

#include <unordered_map>
#include <string>
#include <raylib.h>

class AssetManager
{
public:
    static void Load();
    static void Unload();

    static const Texture2D& GetTexture(const std::string& name);
    static const Sound& GetSound(const std::string& name);
    static const Music& GetMusic(const std::string& name);

private:
    static std::unordered_map<std::string, Texture2D> textures;
    static std::unordered_map<std::string, Sound> sounds;
    static std::unordered_map<std::string, Music> music;
};
