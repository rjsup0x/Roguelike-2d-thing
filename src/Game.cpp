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
        {
            world.Update(dt);

            if (world.IsLevelUpActive())
                state = State::LEVEL_UP_SELECT;

            if (world.IsPlayerDead())
                state = State::GAMEOVER;

            break;
        }
        case State::LEVEL_UP_SELECT:
        {
            if (IsKeyPressed(KEY_ONE))
            {
                world.ApplyUpgrade(0);
                state = State::PLAYING;
            }
            if (IsKeyPressed(KEY_TWO))
            {
                world.ApplyUpgrade(1);
                state = State::PLAYING;
            }

            if (IsKeyPressed(KEY_THREE))
            {
                world.ApplyUpgrade(2);
                state = State::PLAYING;
            }
            break;
        }

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

        case State::LEVEL_UP_SELECT:
        {
            BeginDrawing();
            ClearBackground(BLACK);

            DrawText("LEVEL UP!", 520, 80, 30, WHITE);

            for (int i = 0; i < 3; i++)
            {
                int x = 300 + i * 250;
                int y = 250;

                DrawRectangle(x, y, 200, 120, GRAY);
                DrawRectangleLines(x, y, 200, 120, WHITE);

                DrawText(world.options[i].name, x + 20, y + 50, 15, WHITE);

                DrawText(TextFormat("Press %d", i + 1),
                         x + 40, y + 90, 10, GRAY);
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
