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

    Music currentMusic{};

    // Tracks which state the music system last reacted to, so we know
    // when we've transitioned into/out of MENU and should swap tracks.
    // Previously this lived as a `static State lastMusicState` local
    // inside UpdateMusicSystem(), which worked but meant it wasn't
    // actually part of Game's visible state — moved here as a real
    // member instead. (The old `musicState` member was unused/shadowed
    // by that local and has been removed; same for the unused
    // `menuMusicStarted` flag and the `previousState` member, which was
    // written every frame but never read anywhere.)
    State lastMusicState = State::GAMEOVER;

    void UpdateMusicSystem();

    // init the world
    World world;

    void UpdateMenu(float dt);
    static void DrawMenu();

    void UpdateGameOver(float dt);
    void DrawGameOver() const;

    // void DrawPlaying();
};
