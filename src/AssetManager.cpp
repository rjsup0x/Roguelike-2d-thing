#include "AssetManager.h"

#include <raylib.h>

namespace
{
    // All asset paths were previously hardcoded absolute paths tied to one
    // machine (/Users/ry/projects/roli/...). Centralizing the root here
    // means there's exactly one place to change if the project moves, and
    // it can be swapped for a relative/CWD-based path or
    // GetApplicationDirectory() later without touching every line below.
    constexpr const char* kAssetRoot = "/Users/ry/projects/roli/src/assets";
}

void AssetManager::Load()
{
    // init all assets

    // menu background
    textures["menu_background"] = LoadTexture((std::string(kAssetRoot) + "/textures/backgroundImg.png").c_str());

    // characters
    textures["player"] = LoadTexture((std::string(kAssetRoot) + "/textures/Player.png").c_str());
    textures["enemy"]  = LoadTexture((std::string(kAssetRoot) + "/textures/Enemy.png").c_str());
    // test char

    // weapons
    textures["arrow_weapon"] = LoadTexture((std::string(kAssetRoot) + "/textures/Arrow.png").c_str());

    // Orbital weapon reuses the bullet sprite (confirmed) — no dedicated
    // orbital art yet. Same file gets loaded into two separate texture
    // slots below ("orbital_weapon" and "bullet_weapon"); that's fine,
    // each is just its own GPU handle to the same image data.
    textures["orbital_weapon"] = LoadTexture((std::string(kAssetRoot) + "/textures/Bullet.png").c_str());

    // Bullet::Draw() looks up "bullet_weapon" — loaded separately from
    // "orbital_weapon" above so each weapon type can get its own texture
    // later without the two being tangled together.
    textures["bullet_weapon"] = LoadTexture((std::string(kAssetRoot) + "/textures/Bullet.png").c_str());

    // coin texture
    textures["coin"] = LoadTexture((std::string(kAssetRoot) + "/textures/Coin.png").c_str());

    // sounds
    // hit arrow sound
    sounds["arrow_hit"] = LoadSound((std::string(kAssetRoot) + "/sounds/ArrowHit.wav").c_str());

    // orb pickup sound
    sounds["orb_pickup"] = LoadSound((std::string(kAssetRoot) + "/sounds/Pick_Up.wav").c_str());

    // menu sound
    music["menu_music"] = LoadMusicStream((std::string(kAssetRoot) + "/sounds/Menu_Music.ogg").c_str());    // world

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
