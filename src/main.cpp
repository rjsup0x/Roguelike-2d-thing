#include <raylib.h>
#include "Game.h"
#include "AssetManager.h"

int main()
{
    InitWindow(1280, 720, "roli");
    SetTargetFPS(60);

    // init assets
    AssetManager::Init();

    Game game;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        game.Update(dt);
        game.Draw();
    }

    AssetManager::Shutdown();
    CloseWindow();
}
