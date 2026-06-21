#include "Game.h"
#include "UI/UI.h"

#include <raylib.h>

Game::Game()
{

}

void Game::Update(float dt)
{
    // depens on state which screen to update
    switch (state)
    {
        case State::MENU:
            UpdateMenu(dt);
            break;

        case State::PLAYING:
            world.Update(dt);

            if (world.IsPlayerDead())
                state = State::GAMEOVER;
            break;

        case State::GAMEOVER:
            UpdateGameOver(dt);
            break;
    }
}

void Game::Draw()
{
    // depending on state of game whhich to draw/show
    switch (state)
    {
        case State::MENU:
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawMenu();
            EndDrawing();
            break;

        case State::PLAYING:
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // =========================
            // WORLD SPACE - UI
            // =========================
            BeginMode2D(world.GetCamera());
            world.Draw();
            EndMode2D();

            // =========================
            // HUD (SCREEN SPACE) draw on screen UI here
            // =========================
            //
            UI::DrawXPBar(
                world.GetPlayer().GetXP(),
                world.GetPlayer().GetXPToNextLevel(),
                world.GetPlayer().GetLevel()
            );


            if (world.GetSpawner().ShouldShowWaveText())
            {
                const char* text = TextFormat("WAVE %d STARTING", world.GetSpawner().GetWave());

                int fontSize = 30;
                int screenWidth = GetScreenWidth();

                int textWidth = MeasureText(text, fontSize);

                int x = (screenWidth - textWidth) / 2;
                int y = 20;

                DrawText(text, x, y, fontSize, GRAY);
            }

            EndDrawing();
            break;
        }

        case State::GAMEOVER:
            BeginDrawing();
            ClearBackground(RAYWHITE);

            world.Draw();
            DrawGameOver();

            EndDrawing();
            break;
    }
}

void Game::UpdateMenu(float dt)
{
    // menu to game = playing
    if (IsKeyPressed(KEY_ENTER))
        state = State::PLAYING;
}

void Game::DrawMenu()
{
    DrawText("ROLI", 600, 300, 40, BLACK);
    DrawText("Press ENTER to start", 550, 360, 20, DARKGRAY);
}

void Game::UpdateGameOver(float dt)
{
    // gameover to play again = r
    if (IsKeyPressed(KEY_R))
    {
        world.Reset();
        state = State::PLAYING;
    }
}

void Game::DrawGameOver()
{
    DrawText("GAME OVER", 550, 300, 40, RED);
    DrawText("Press R to restart", 520, 360, 20, DARKGRAY);
}
