#include "Game.h"
#include "AssetManager.h"
#include "UI/UI.h"

#include <raylib.h>

#include <string>

// ----------------------------------------------------------------------------
//                             GAME CONSTRUCTOR
// ----------------------------------------------------------------------------
Game::Game()
{
    // stop "esc" closing window
    SetExitKey(0);
}

// ----------------------------------------------------------------------------
//                        UPDATING THE GAME
// ----------------------------------------------------------------------------
void Game::Update(float deltaTime)
{
    UpdateMusicSystem();

    switch (state)
    {
        case State::MENU:
            UpdateMenu(deltaTime);
            break;

        case State::PLAYING: {
            if (!world.IsLevelUpActive())
                world.Update(deltaTime);

            if (world.IsLevelUpActive())
            {
                UI::HandleLevelUpInput(world);
                UI::DrawLevelUp(world);
            }

            if (world.IsPlayerDead())
                state = State::GAME_OVER;

            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P))
                state = State::PAUSE;

            break;
        }

        case State::PAUSE:
            UpdatePause(deltaTime);
            break;

        case State::GAME_OVER:
            UpdateGameOver(deltaTime);
            break;
    }
}

// ----------------------------------------------------------------------------
//                         DRAWING THE GAME
// ----------------------------------------------------------------------------
void Game::Draw()
{
    // depending on state of game which to draw/show
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
            ClearBackground(BLACK);

            // =========================
            // SPACE - UI
            // =========================
            BeginMode2D(world.GetCamera());
            world.Draw();
            EndMode2D();

            // world timer
            UI::DrawTimer(world);

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

                constexpr int fontSize{30};
                const int screenWidth = GetScreenWidth();
                const int screenHeight = GetScreenHeight();

                const int textWidth = MeasureText(text, fontSize);

                const int x{(screenWidth - textWidth) / 2};
                const int y{(screenHeight / 4)};

                DrawText(text, x, y, fontSize, BLACK);
            }

            // =========================
            // UPGRADE UI (CENTER PANEL)
            // =========================
            if (world.IsLevelUpActive())
            {
                const auto time = static_cast<float>(GetTime());
                const float pulse = 1.0f + sinf(time * 6.0f) * 0.03f;

                const int screenW = GetScreenWidth();
                const int screenH = GetScreenHeight();

                constexpr int panelW{600};
                constexpr int panelH{250};

                const Rectangle panel = {
                    static_cast<float>(screenW - panelW) * 0.5f,
                    static_cast<float>(screenH - panelH) * 0.5f,
                    static_cast<float>(panelW),
                    static_cast<float>(panelH)
                };

                DrawRectangleRec(panel, Fade(BLACK, 0.65f));
                DrawRectangleLinesEx(panel, 2, WHITE);

                DrawText(
                    "LEVEL UP!",
                    static_cast<int>(panel.x + 20.0f),
                    static_cast<int>(panel.y + 15.0f),
                    28,
                    WHITE
                );

                constexpr int boxW{160};
                constexpr int spacing{20};

                const float startX = panel.x + static_cast<float>(panelW - (boxW * 3 + spacing * 2)) * 0.5f;

                const float y = panel.y + 80.0f;

                for (int i = 0; i < 3; i++)
                {
                    constexpr int boxH{100};
                    const float x = startX + static_cast<float>(i * (boxW + spacing));

                    const float w = static_cast<float>(boxW) * pulse;
                    const float h = static_cast<float>(boxH) * pulse;

                    const float ox = (w - boxW) * 0.5f;
                    const float oy = (h - boxH) * 0.5f;

                    const Rectangle rect = {
                        x - ox,
                        y - oy,
                        w,
                        h
                    };

                    const bool hovered = (world.hoveredUpgradeIndex == i);

                    const Color base = hovered ? YELLOW : DARKGRAY;
                    const Color border = hovered ? YELLOW : WHITE;

                    DrawRectangleRec(rect, base);
                    DrawRectangleLinesEx(rect, 2, border);

                    DrawRectangleRec(rect, Fade(WHITE, hovered ? 0.12f : 0.08f));

                    DrawText(
                        world.options[i].name,
                        static_cast<int>(rect.x) + 10,
                        static_cast<int>(rect.y) + 30,
                        12,
                        WHITE
                    );

                    DrawText(
                        TextFormat("[ %d ]", i + 1),
                        static_cast<int>(rect.x) + 10,
                        static_cast<int>(rect.y) + 65,
                        12,
                        GRAY
                    );
                }
            }

            EndDrawing();
            break;
        }

        case State::PAUSE:
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawPause();
            EndDrawing();
            break;

        case State::GAME_OVER:
            BeginDrawing();
            ClearBackground(RAYWHITE);

            world.Draw();
            DrawGameOver();

            EndDrawing();
            break;
    }
}

// ----------------------------------------------------------------------------
//                      UPDATING THE MENU
// ----------------------------------------------------------------------------
void Game::UpdateMenu(float deltaTime)
{
    const int screenW = GetScreenWidth();

        constexpr int buttonW = 280;
        constexpr int buttonH = 70;

        const Rectangle playButton =
        {
            static_cast<float>(screenW - buttonW) / 2.0f,
            280,
            static_cast<float>(buttonW),
            static_cast<float>(buttonH)
        };

        const Rectangle settingsButton =
        {
            static_cast<float>(screenW - buttonW) / 2.0f,
            370,
            static_cast<float>(buttonW),
            static_cast<float>(buttonH)
        };

        const Rectangle exitButton =
        {
            static_cast<float>(screenW - buttonW) / 2.0f,
            460,
            static_cast<float>(buttonW),
            static_cast<float>(buttonH)
        };

        const Vector2 mouse = GetMousePosition();

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

// ----------------------------------------------------------------------------
//                         DRAWING THE MENU
// ----------------------------------------------------------------------------
void Game::DrawMenu()
{
    // get texture
    const Texture2D& Menu_Background = AssetManager::GetTexture("menu_background");

    // draw texture as screen size
    DrawTexturePro(
      Menu_Background,
      {0, 0, static_cast<float>(Menu_Background.width), static_cast<float>(Menu_Background.height)},
      {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())},
      {0, 0},
      0.0f,
      WHITE
    );

    // fade living over the top of it
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.35f));

    // set up buttons
    const int screenWidth{GetScreenWidth()};

    constexpr int buttonWidth{280};
    constexpr int buttonHeight{70};

    // player button
    const Rectangle playButton =
    {
        static_cast<float>(screenWidth - buttonWidth) / 2.0f,
        280,
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight),
    };

    // settings
    const Rectangle settingsButton =
    {
        static_cast<float>(screenWidth - buttonWidth) / 2.0f,
        370,
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight),
    };

    const Rectangle exitButton =
    {
        static_cast<float>(screenWidth - buttonWidth) / 2.0f,
        460,
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight),
    };

    const auto title = "ROLI";

    constexpr int titleSize{72};

    const int titleWidth = MeasureText(title, titleSize);

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

// ----------------------------------------------------------------------------
//                          UPDATE THE GAME_OVER
// ----------------------------------------------------------------------------
void Game::UpdateGameOver(float deltaTime)
{
    // get mouse pos for use of mouse
    const Vector2 mouse = GetMousePosition();

    // buttons
    const Rectangle restartButton =
    {
        static_cast<float>(GetScreenWidth()) / 2.0f - 260.0f,
        350.0f,
        220.0f,
        70.0f
    };

    const Rectangle menuButton =
    {
        static_cast<float>(GetScreenWidth()) / 2 + 40,
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

// ----------------------------------------------------------------------------
//                       DRAW THE GAME_OVER SCREEN
// ----------------------------------------------------------------------------
void Game::DrawGameOver() const {
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
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.75f));

    // get size of screen
    const int screenWidth{GetScreenWidth()};

    // set up button sizing
    constexpr int buttonWidth{220};
    constexpr int buttonHeight{70};

    // set up title + sizing
    const auto title = "GameOver";
    constexpr int titleSize{60};
    const int titleWidth = MeasureText(title, titleSize);

    // draw title with sizing
    DrawText(
           title,
           (screenWidth - titleWidth) / 2,
           140,
           titleSize,
           RED
    );

    const int totalSeconds = static_cast<int>(world.GetSurvivalTime());

    const int minutes = totalSeconds / 60;
    const int seconds = totalSeconds % 60;

    DrawText(
        TextFormat("Survived %02d:%02d", minutes, seconds),
        (screenWidth - MeasureText("Survived 00:00", 24)) / 2,
        220,
        24,
        WHITE
    );

    // add buttons
    // restart button
    const Rectangle restartButton =
    {
        static_cast<float>(GetScreenWidth()) / 2 - 260,
        350,
        220,
        70
    };

    // back to menu
    const Rectangle menuButton =
    {
        static_cast<float>(GetScreenWidth()) / 2 + 40,
        350,
        buttonWidth,
        buttonHeight
    };

    // draw to ui
    UI::DrawMenuButton(restartButton, "Retry");
    UI::DrawMenuButton(menuButton, "Menu");
}

// ----------------------------------------------------------------------------
//                             MUSIC SYSTEM UPDATE
// ----------------------------------------------------------------------------
void Game::UpdateMusicSystem()
{
    // if the game is in menu or pause state
    if (state == State::MENU || state == State::PAUSE)
    {
        // otherwise dont play or reset music
        if (lastMusicState != State::MENU)
        {
            StopMusicStream(currentMusic);

            currentMusic = AssetManager::GetMusic("menu_music");
            PlayMusicStream(currentMusic);

            lastMusicState = State::MENU;
        }

        UpdateMusicStream(currentMusic);
    }
    else
    {
        if (lastMusicState == State::MENU)
        {
            StopMusicStream(currentMusic);
            lastMusicState = state;
        }
    }
}

// ----------------------------------------------------------------------------
//                     DRAW PAUSE SCREEN
// ----------------------------------------------------------------------------
void Game::DrawPause()
{
    // get texture
    const Texture2D& Menu_Background = AssetManager::GetTexture("menu_background");

    // draw texture as screen size
    DrawTexturePro(
      Menu_Background,
      {0, 0, static_cast<float>(Menu_Background.width), static_cast<float>(Menu_Background.height)},
      {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())},
      {0, 0},
      0.0f,
      WHITE
    );

    // fade living over the top of it
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.35f));

    // set up buttons
    const int screenWidth{GetScreenWidth()};

    constexpr int buttonWidth{280};
    constexpr int buttonHeight{70};

    const Rectangle resumeButton =
    {
        static_cast<float>(GetScreenWidth() - buttonWidth) / 2.0f,
        280,
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    const Rectangle restartButton =
    {
        static_cast<float>(GetScreenWidth() - buttonWidth) / 2.0f,
        370,
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    const Rectangle menuButton =
    {
        static_cast<float>(GetScreenWidth() - buttonWidth) / 2.0f,
        460,
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    const auto title = "PAUSED";

    constexpr int titleSize{72};

    const int titleWidth = MeasureText(title, titleSize);

    DrawText(
           title,
           (screenWidth - titleWidth) / 2,
           120,
           titleSize,
           WHITE
       );

    // draw to ui
    UI::DrawMenuButton(resumeButton, "Resume");
    UI::DrawMenuButton(restartButton, "Restart");
    UI::DrawMenuButton(menuButton, "Back To Menu");
}

// ----------------------------------------------------------------------------
//                         UPDATE PAUSE SCREEN
// ----------------------------------------------------------------------------
void Game::UpdatePause(float deltaTime)
{
    // get mouse pos for use of mouse
    const Vector2 mouse = GetMousePosition();

    // buttons
    constexpr int buttonWidth{280};
    constexpr int buttonHeight{70};

    const Rectangle resumeButton =
    {
        static_cast<float>(GetScreenWidth() - buttonWidth) / 2.0f,
        280,
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    const Rectangle restartButton =
    {
        static_cast<float>(GetScreenWidth() - buttonWidth) / 2.0f,
        370,
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    const Rectangle menuButton =
    {
        static_cast<float>(GetScreenWidth() - buttonWidth) / 2.0f,
        460,
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight)
    };

    // check if mouse collides with buttons
    if (CheckCollisionPointRec(mouse, resumeButton))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            state = State::PLAYING;
        }
    }

    if (CheckCollisionPointRec(mouse, restartButton))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            world.Reset();
            state = State::PLAYING;
        }
    }

    if (CheckCollisionPointRec(mouse, menuButton)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            world.Reset();
            state = State::MENU;
        }
    }
}

// ----------------------------------------------------------------------------
//                        WHETHER TO EXIT THE GAME
// ----------------------------------------------------------------------------
bool Game::ShouldExit() const
{
    return shouldExit;
}
