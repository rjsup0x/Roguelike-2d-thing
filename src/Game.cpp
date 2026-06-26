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
void Game::Update(float dt)
{
    UpdateMusicSystem();

    switch (state)
    {
        case State::MENU:
            UpdateMenu(dt);
            break;

        case State::PLAYING: {
            if (!world.IsLevelUpActive())
                world.Update(dt);

            if (world.IsLevelUpActive())
            {
                UI::HandleLevelUpInput(world);
                UI::DrawLevelUp(world);
            }

            if (world.IsPlayerDead())
                state = State::GAMEOVER;

            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P))
                state = State::PAUSE;

            break;
        }

        case State::PAUSE:
            UpdatePause(dt);
            break;

        case State::GAMEOVER:
            UpdateGameOver(dt);
            break;
    }
}

// ----------------------------------------------------------------------------
//                         DRAWING THE GAME
// ----------------------------------------------------------------------------
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

                int fontSize{30};
                int screenWidth = GetScreenWidth();
                int screenHeight = GetScreenHeight();

                int textWidth = MeasureText(text, fontSize);

                int x{(screenWidth - textWidth) / 2};
                int y{(screenHeight / 4)};

                DrawText(text, x, y, fontSize, BLACK);
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
                    static_cast<float>(panelW),
                    static_cast<float>(panelH)
                };

                DrawRectangleRec(panel, Fade(BLACK, 0.65f));
                DrawRectangleLinesEx(panel, 2, WHITE);

                DrawText("LEVEL UP!", panel.x + 20, panel.y + 15, 28, WHITE);

                constexpr int boxW{160};
                int spacing{20};

                float startX = panel.x + (panelW - (boxW * 3 + spacing * 2)) * 0.5f;
                float y = panel.y + 80;

                for (int i = 0; i < 3; i++)
                {
                    int boxH{100};
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

                    Color base = hovered ? YELLOW : DARKGRAY;
                    Color border = hovered ? YELLOW : WHITE;

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

        case State::GAMEOVER:
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
void Game::UpdateMenu(float dt)
{
    int screenW = GetScreenWidth();

        constexpr int buttonW = 280;
        constexpr int buttonH = 70;

        Rectangle playButton =
        {
            (screenW - buttonW) / 2.0f,
            280,
            static_cast<float>(buttonW),
            static_cast<float>(buttonH)
        };

        Rectangle settingsButton =
        {
            (screenW - buttonW) / 2.0f,
            370,
            static_cast<float>(buttonW),
            static_cast<float>(buttonH)
        };

        Rectangle exitButton =
        {
            (screenW - buttonW) / 2.0f,
            460,
            static_cast<float>(buttonW),
            static_cast<float>(buttonH)
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
    int screenWidth{GetScreenWidth()};

    constexpr int buttonWidth{280};
    constexpr int buttonHeight{70};

    // playe button
    Rectangle playButton =
    {
        (screenWidth - buttonWidth) / 2.0f,
        280,
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight),
    };

    // settings
    Rectangle settingsButton =
    {
        (screenWidth - buttonWidth) / 2.0f,
        370,
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight),
    };

    Rectangle exitButton =
    {
        (screenWidth - buttonWidth) / 2.0f,
        460,
        static_cast<float>(buttonWidth),
        static_cast<float>(buttonHeight),
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

// ----------------------------------------------------------------------------
//                          UPDATE THE GAMEOVER
// ----------------------------------------------------------------------------
void Game::UpdateGameOver(float dt)
{
    // get mouse pos for use of mouse
    Vector2 mouse = GetMousePosition();

    // buttons
    Rectangle restartButton =
    {
        static_cast<float>(GetScreenWidth()) / 2 - 260,
        350,
        220,
        70
    };

    Rectangle menuButton =
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
//                       DRAW THE GAMEOVER SCREEN
// ----------------------------------------------------------------------------
void Game::DrawGameOver() const {
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
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.75f));

    // get size of screen
    int screenWidth{GetScreenWidth()};
    int screenHeight{GetScreenHeight()};

    // set up button sizing
    constexpr int buttonWidth{220};
    constexpr int buttonHeight{70};

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

    int totalSeconds = static_cast<int>(world.GetSurvivalTime());

    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    DrawText(
        TextFormat("Survived %02d:%02d", minutes, seconds),
        (screenWidth - MeasureText("Survived 00:00", 24)) / 2,
        220,
        24,
        WHITE
    );

    // add buttons
    // restart button
    Rectangle restartButton =
    {
        static_cast<float>(screenWidth) / 2 - 260,
        350,
        buttonWidth,
        buttonHeight
    };

    // back to menu
    Rectangle menuButton =
    {
        static_cast<float>(screenWidth) / 2 + 40,
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
    int screenWidth{GetScreenWidth()};

    constexpr int buttonWidth{280};
    constexpr int buttonHeight{70};

    Rectangle resumeButton =
    {
        (GetScreenWidth() - buttonWidth) / 2.0f,
        280,
        (float)buttonWidth,
        (float)buttonHeight
    };

    Rectangle restartButton =
    {
        (GetScreenWidth() - buttonWidth) / 2.0f,
        370,
        (float)buttonWidth,
        (float)buttonHeight
    };

    Rectangle menuButton =
    {
        (GetScreenWidth() - buttonWidth) / 2.0f,
        460,
        (float)buttonWidth,
        (float)buttonHeight
    };

    const char* title = "PAUSED";

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
    Vector2 mouse = GetMousePosition();

    // buttons
    constexpr int buttonWidth{280};
    constexpr int buttonHeight{70};

    Rectangle resumeButton =
    {
        (GetScreenWidth() - buttonWidth) / 2.0f,
        280,
        (float)buttonWidth,
        (float)buttonHeight
    };

    Rectangle restartButton =
    {
        (GetScreenWidth() - buttonWidth) / 2.0f,
        370,
        (float)buttonWidth,
        (float)buttonHeight
    };

    Rectangle menuButton =
    {
        (GetScreenWidth() - buttonWidth) / 2.0f,
        460,
        (float)buttonWidth,
        (float)buttonHeight
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
