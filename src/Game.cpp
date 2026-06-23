#include "Game.h"
#include "AssetManager.h"
#include "UI/UI.h"

#include <raylib.h>
#include <string>

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

            // world timer
            UI::DrawTimer(world);

            // =========================
            // SPACE - UI
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
                int screenHeight = GetScreenHeight();

                int textWidth = MeasureText(text, fontSize);

                int x{(screenWidth - textWidth) / 2};
                int y{(screenHeight / 4)};

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
    int screenW = GetScreenWidth();

        const int buttonW = 280;
        const int buttonH = 70;

        Rectangle playButton =
        {
            (screenW - buttonW) / 2.0f,
            280,
            (float)buttonW,
            (float)buttonH
        };

        Rectangle settingsButton =
        {
            (screenW - buttonW) / 2.0f,
            370,
            (float)buttonW,
            (float)buttonH
        };

        Rectangle exitButton =
        {
            (screenW - buttonW) / 2.0f,
            460,
            (float)buttonW,
            (float)buttonH
        };

        Vector2 mouse = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (CheckCollisionPointRec(mouse, playButton))
            {
                state = State::PLAYING;
            }

            if (CheckCollisionPointRec(mouse, settingsButton))
            {
                // Add settings screen later
            }

            if (CheckCollisionPointRec(mouse, exitButton))
            {
                shouldExit = true;
            }
        }
}

void Game::DrawMenu()
{
    // get texture
    const Texture2D& Menu_Background = AssetManager::GetTexture("menu_background");

    // draw texture as screen size
    DrawTexturePro(
      Menu_Background,
      {0, 0, (float)Menu_Background.width, (float)Menu_Background.height},
      {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
      {0, 0},
      0.0f,
      WHITE
    );

    // fade living over the top of it
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.35f));

    // set up buttons
    int screenWidth{GetScreenWidth()};

    const int buttonWidth{280};
    const int buttonHeight{70};

    // playe button
    Rectangle playButton =
    {
        (screenWidth - buttonWidth) / 2.0f,
        280,
        (float)buttonWidth,
        (float)buttonHeight,
    };

    // settings
    Rectangle settingsButton =
    {
        (screenWidth - buttonWidth) / 2.0f,
        370,
        (float)buttonWidth,
        (float)buttonHeight,
    };

    Rectangle exitButton =
    {
        (screenWidth - buttonWidth) / 2.0f,
        460,
        (float)buttonWidth,
        (float)buttonHeight,
    };

    const char* title = "ROLI";

    int titleSize = 72;

    int titleWidth =
        MeasureText(title, titleSize);

    DrawText(
           title,
           (screenWidth - titleWidth) / 2,
           120,
           titleSize,
           WHITE
       );

    // draw to ui
    UI::DrawMenuButton(playButton, "Play");
    UI::DrawMenuButton(settingsButton, "Settings");
    UI::DrawMenuButton(exitButton, "Exit");
}

void Game::UpdateGameOver(float dt)
{
    // get mouse pos for use of mouse
    Vector2 mouse = GetMousePosition();

    // buttons
    Rectangle restartButton =
    {
        (float)GetScreenWidth() / 2 - 260,
        350,
        220,
        70
    };

    Rectangle menuButton =
    {
        (float)GetScreenWidth() / 2 + 40,
        350,
        220,
        70
    };

    // check if mouse collides with buttons
    if (CheckCollisionPointRec(mouse, restartButton))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            world.Reset();
            state = State::PLAYING;
        }
    }

    if (CheckCollisionPointRec(mouse, menuButton))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            world.Reset();
            state = State::MENU;
        }
    }
}

void Game::DrawGameOver()
{
    // d'know if i want a texture on gameover screen yet
    // const Texture2D& Menu_Background = AssetManager::GetTexture("menu_background");

    // // draw texture as screen size
    // DrawTexturePro(
    //   Menu_Background,
    //   {0, 0, (float)Menu_Background.width, (float)Menu_Background.height},
    //   {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
    //   {0, 0},
    //   0.0f,
    //   WHITE
    // );

    // fade background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.35f));

    // get size of screen
    int screenWidth{GetScreenWidth()};
    int screenHeight{GetScreenHeight()};

    // set up button sizing
    const int buttonWidth{280};
    const int buttonHeight{70};

    // set up title + sizing
    const char* title = "GameOver";
    int titleSize = 60;
    int titleWidth = MeasureText(title, titleSize);

    // draw title with sizings
    DrawText(
           title,
           (screenWidth - titleWidth) / 2,
           140,
           titleSize,
           RED
    );

    int totalSeconds = (int)world.GetSurvivalTime();

    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    DrawText(
        TextFormat("Survived %02d:%02d", minutes, seconds),
        (screenWidth - MeasureText("Survived 00:00", 24)) / 2,
        220,
        24,
        BLACK
    );

    // add buttons
    // restart button
    Rectangle restartButton =
    {
        (float)screenWidth / 2 - 260,
        350,
        220,
        70
    };

    // back to menu
    Rectangle menuButton =
    {
        (float)screenWidth / 2 + 40,
        350,
        220,
        70
    };

    // draw to ui
    UI::DrawMenuButton(restartButton, "Retry");
    UI::DrawMenuButton(menuButton, "Menu");
}

bool Game::ShouldExit() const
{
    return shouldExit;
}
