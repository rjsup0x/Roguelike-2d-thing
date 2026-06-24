#pragma once

#include "World.h"

class Game
{
public:
    [[nodiscard]] bool ShouldExit() const;
    bool shouldExit{};

    Game();

    void Update(float dt);
    void Draw();

private:
    // controlles which "screen" to show
    enum class State
    {
        MENU,
        PLAYING,
        GAMEOVER,
    };

    // init screen as menu
    State state = State::MENU;
    State previousState = State::MENU;

    bool menuMusicPlaying = false;

    // init the world
    World world;

    void UpdateMenu(float dt);
    static void DrawMenu();

    void UpdateGameOver(float dt);
    void DrawGameOver() const;

    // void DrawPlaying();
};
