#include <raylib.h>
#include "Game.h"
#include "AssetManager.h"

int main(void)
{
    InitWindow(1280, 720, "roli");
    SetTargetFPS(60);

    // init assets
    AssetManager::Init();

    // init game instance
    Game game;

    while (!WindowShouldClose())
    {
        // get deltatime
        float dt = GetFrameTime();

        // update game state
        game.Update(dt);
        game.Draw();
    }

    // deinit assets
    AssetManager::Shutdown();
    CloseWindow();
}
