#pragma once

#include "World.h"
#include "UI/UI.h"

class Game
{
public:
    Game();

    void Update(float dt);
    void Draw();

private:
    // controlles which "screen" to show
    enum class State
    {
        MENU,
        PLAYING,
        GAMEOVER
    };

    // init screen as menu
    State state = State::MENU;

    // init the world
    World world;

    void UpdateMenu(float dt);
    void DrawMenu();

    void UpdateGameOver(float dt);
    void DrawGameOver();

    void DrawPlaying();
};
