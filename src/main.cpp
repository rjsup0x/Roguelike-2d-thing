#include <raylib.h>

#include "Game.h"
#include "AssetManager.h"

int main()
{
    constexpr int screenWidth{1280};
    constexpr int screenHeight{720};
    const auto title = "roli";

    InitWindow(screenWidth, screenHeight, title);
    SetTargetFPS(60);

    InitAudioDevice();

    // init assets
    AssetManager::Load();

    // init game instance
    Game game;

    while (!WindowShouldClose() && !game.ShouldExit())
    {
        // get delta time
        const float deltaTime = GetFrameTime();

        // update game state
        game.Update(deltaTime);
        game.Draw();
    }

    // deinitialize assets
    AssetManager::Unload();
    CloseAudioDevice();
    CloseWindow();
}
