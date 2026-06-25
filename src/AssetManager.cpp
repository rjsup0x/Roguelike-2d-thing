#include "AssetManager.h"

#include <raylib.h>

void AssetManager::Load()
{
    // init all assets

    // menu background
    textures["menu_background"] = LoadTexture("/Users/ry/projects/roli/src/assets/textures/backgroundImg.png");

    // characters
    textures["player"] = LoadTexture("/Users/ry/projects/roli/src/assets/textures/Player.png");
    textures["enemy"]  = LoadTexture("/Users/ry/projects/roli/src/assets/textures/Enemy.png");
    // test char

    // weapons
    textures["arrow_weapon"] = LoadTexture("/Users/ry/projects/roli/src/assets/textures/Arrow.png");
    textures["orbital_weapon"] = LoadTexture("/Users/ry/projects/roli/src/assets/textures/Bullet.png");

    // coin texture
    textures["coin"] = LoadTexture("/Users/ry/projects/roli/src/assets/textures/Coin.png");

    // sounds
    // hit arrow sound
    sounds["arrow_hit"] = LoadSound("/Users/ry/projects/roli/src/assets/sounds/ArrowHit.wav");

    // orb pickup sound
    sounds["orb_pickup"] = LoadSound("/Users/ry/projects/roli/src/assets/sounds/Pick_Up.wav");

    // menu sound
    music["menu_music"] = LoadMusicStream("/Users/ry/projects/roli/src/assets/sounds/Menu_Music.ogg");    // world

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

    // unload sounds
    for (auto& [name, sound] : sounds)
    {
        UnloadSound(sound);
    }
    sounds.clear();
}

const Texture2D& AssetManager::GetTexture(const std::string& name)
{
    return textures.at(name);
}

const Sound& AssetManager::GetSound(
    const std::string& name)
{
    return sounds.at(name);
}

const Music& AssetManager::GetMusic(const std::string& name)
{
    return music.at(name);
}

std::unordered_map<std::string, Texture2D> AssetManager::textures;
std::unordered_map<std::string, Sound> AssetManager::sounds;
std::unordered_map<std::string, Music> AssetManager::music;

