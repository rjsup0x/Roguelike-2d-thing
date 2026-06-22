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
            // toggle pause via world flag
            if (!world.IsLevelUpActive())
            {
                world.Update(dt);
            }

            if (world.IsLevelUpActive())
            {
                UI::HandleLevelUpInput(world);
                UI::DrawLevelUp(world);
            }

            if (world.IsPlayerDead())
                state = State::GAMEOVER;

            if (IsKeyPressed(KEY_M))
                state = State::MENU;

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
            UI::DrawXPBar(
                world.GetPlayer().GetXP(),
                world.GetPlayer().GetXPToNextLevel(),
                world.GetPlayer().GetLevel()
            );

            if (world.GetSpawner().ShouldShowWaveText())
            {
                const char* text = TextFormat("WAVE %d STARTING", world.GetSpawner().GetWave());

                int fontSize{30};
                int screenWidth = GetScreenWidth();

                int textWidth = MeasureText(text, fontSize);

                int x{(screenWidth - textWidth) / 2};
                int y{20};

                DrawText(text, x, y, fontSize, GRAY);
            }

            // =========================
            // UPGRADE UI (CENTER PANEL)
            // =========================
            if (world.IsLevelUpActive())
            {
                float t = GetTime();
                float pulse = 1.0f + sinf(t * 6.0f) * 0.03f;

                int screenW = GetScreenWidth();
                int screenH = GetScreenHeight();

                int panelW{600};
                int panelH{250};

                Rectangle panel = {
                    (screenW - panelW) * 0.5f,
                    (screenH - panelH) * 0.5f,
                    (float)panelW,
                    (float)panelH
                };

                DrawRectangleRec(panel, Fade(BLACK, 0.65f));
                DrawRectangleLinesEx(panel, 2, WHITE);

                DrawText("LEVEL UP!", panel.x + 20, panel.y + 15, 28, WHITE);

                int boxW{160};
                int boxH{100};
                int spacing{20};

                float startX = panel.x + (panelW - (boxW * 3 + spacing * 2)) * 0.5f;
                float y = panel.y + 80;

                for (int i = 0; i < 3; i++)
                {
                    float x = startX + i * (boxW + spacing);

                    float w = boxW * pulse;
                    float h = boxH * pulse;

                    float ox = (w - boxW) * 0.5f;
                    float oy = (h - boxH) * 0.5f;

                    Rectangle rect = {
                        x - ox,
                        y - oy,
                        w,
                        h
                    };

                    bool hovered = (world.hoveredUpgradeIndex == i);

                    Color base = hovered ? DARKGRAY : DARKGRAY;
                    Color border = hovered ? YELLOW : WHITE;

                    DrawRectangleRec(rect, base);
                    DrawRectangleLinesEx(rect, 2, border);

                    DrawRectangleRec(rect, Fade(WHITE, hovered ? 0.12f : 0.08f));

                    DrawText(
                        world.options[i].name,
                        (int)rect.x + 10,
                        (int)rect.y + 30,
                        12,
                        WHITE
                    );

                    DrawText(
                        TextFormat("[ %d ]", i + 1),
                        (int)rect.x + 10,
                        (int)rect.y + 65,
                        12,
                        GRAY
                    );
                }
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
