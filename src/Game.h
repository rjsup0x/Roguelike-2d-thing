#pragma once

#include "World.h"

class Game
{
public:
    [[nodiscard]] bool ShouldExit() const;
    bool shouldExit{};

    Game();


    void Update(float deltaTime);

    void Draw();

private:
    // controls which "screen" to show
    enum class State
    {
        MENU,
        PLAYING,
        GAME_OVER,
        PAUSE
    };

    // init screen as menu
    State state = State::MENU;

    Music currentMusic{};
    State lastMusicState = State::GAME_OVER;
    void UpdateMusicSystem();

    // init the world
    World world;

    void UpdateMenu(float deltaTime);
    static void DrawMenu();

    void UpdateGameOver(float deltaTime);
    void DrawGameOver() const;

    static void DrawPause();
    void UpdatePause(float deltaTime);

    // void DrawPlaying();
};
