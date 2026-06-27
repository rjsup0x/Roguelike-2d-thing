#include "UI.h"
#include <raylib.h>
#include <raymath.h>

// =========================
// WORLD SPACE HEALTH BAR
// =========================
void UI::DrawHealthBar(Vector2 worldPosition, const int health, const int maxHealth)
{
    constexpr float barWidth{6.0f};
    constexpr float barHeight{32.0f};

    const float healthPercent = Clamp(
        static_cast<float>(health) / static_cast<float>(maxHealth),
        0.0f,
        1.0f
    );

    // small gap from texture
    const Vector2 barPos = {
        worldPosition.x + -26.0f,
        worldPosition.y - barHeight * 0.5f
    };

    const float fillHeight{barHeight * healthPercent};

    const Color fillColor = {
        static_cast<unsigned char>(255 * (1.0f - healthPercent)),
        static_cast<unsigned char>(255 * healthPercent),
        0,
        255
    };

    // =========================
    // BAR BACKGROUND
    // =========================
    DrawRectangle(
        static_cast<int>(barPos.x),
        static_cast<int>(barPos.y),
        static_cast<int>(barWidth),
        static_cast<int>(barHeight),
        DARKGRAY
    );

    // =========================
    // BAR FILL
    // =========================
    DrawRectangle(
        static_cast<int>(barPos.x),
        static_cast<int>(barPos.y + barHeight - fillHeight),
        static_cast<int>(barWidth),
        static_cast<int>(fillHeight),
        fillColor
    );

    // =========================
    // WORLD TEXT
    // =========================

    const char* text = TextFormat("%d / %d", health, maxHealth);

    const int textWidth = MeasureText(text, fontSize);

    DrawText(
        text,
        static_cast<int>(barPos.x + (barWidth / 2) - (static_cast<float>(textWidth) / 2)),
        static_cast<int>(barPos.y - 14), // ABOVE the bar
        fontSize,
        RAYWHITE
    );
}

void UI::DrawXPBar(const int xp, const int maxXP, const int level)
{
    auto screenW = static_cast<float>(GetScreenWidth());
    auto screenH = static_cast<float>(GetScreenHeight());

    // =========================
    // HUD BACKGROUND PANEL
    // =========================
    constexpr float panelHeight{60.0f};
    const float panelY = screenH - panelHeight;

    DrawRectangle(
        0,
        panelY,
        screenW,
        panelHeight,
        Fade(BLACK, 0.9f)
    );

    // =========================
    // XP BAR
    // =========================
    const float barWidth = screenW - 200;
    constexpr float barHeight{12.0f};

    const float screenX = screenW * 0.5f - barWidth * 0.5f;

    // move bar down INTO the panel
    const float screenY = panelY + panelHeight * 0.5f - barHeight * 0.5f;

    float percent = static_cast<float>(xp) / static_cast<float>(maxXP);
    if (percent > 1.0f) percent = 1.0f;

    // background bar
    DrawRectangle(screenX, screenY, barWidth, barHeight, DARKGRAY);

    // fill bar
    DrawRectangle(screenX, screenY, barWidth * percent, barHeight, SKYBLUE);

    // =========================
    // TEXT
    // =========================

    // level (left inside panel)
    DrawText(
        TextFormat("LVL: %d", level),
        50,
        panelY + 25,
        12,
        WHITE
    );

    // next level (right side)
    DrawText(
        "NEXT LVL",
        screenW - 90,
        panelY + 25,
        12,
        WHITE
    );
}

void UI::HandleLevelUpInput(World& world)
{
    if (!world.IsLevelUpActive())
        return;

    const Vector2 mouse = GetMousePosition();

    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();

    constexpr int panelW{600};
    constexpr int panelH{250};

    Rectangle panel = {
        (screenW - panelW) * 0.5f,
        (screenH - panelH) * 0.5f,
        static_cast<float>(panelW),
        static_cast<float>(panelH)
    };

    constexpr int boxW{160};
    constexpr int spacing{20};

    const float startX = panel.x + (panelW - (boxW * 3 + spacing * 2)) * 0.5f;
    const float y = panel.y + 80;

    // make 3 boxes for upgrade option
    for (int i = 0; i < 3; i++)
    {
        constexpr int boxH{100};
        const Rectangle rect = {
            startX + i * (boxW + spacing),
            y,
            static_cast<float>(boxW),
            static_cast<float>(boxH)
        };

        if (CheckCollisionPointRec(mouse, rect))
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                world.ApplyUpgrade(i);
            }
        }
    }
}

void UI::DrawLevelUp(const World& world)
{
    if (!world.IsLevelUpActive())
        return;

    const auto time = static_cast<float>(GetTime());
    const float pulse = 1.0f + sinf(time * 6.0f) * 0.03f;

    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();

    constexpr int panelW{600};
    constexpr int panelH{250};

    Rectangle panel = {
        (screenW - panelW) * 0.5f,
        (screenH - panelH) * 0.5f,
        static_cast<float>(panelW),
        static_cast<float>(panelH)
    };

    // ONLY panel darkening (not full screen)
    DrawRectangleRec(panel, Fade(BLACK, 0.25f));
    DrawRectangleLinesEx(panel, 2, WHITE);

    DrawText("LEVEL UP!", panel.x + 20, panel.y + 15, 28, WHITE);

    constexpr int boxW{160};
    constexpr int spacing{20};

    const float startX = panel.x + (panelW - (boxW * 3 + spacing * 2)) * 0.5f;
    const float y = panel.y + 80;

    for (int i = 0; i < 3; i++)
    {
        constexpr int boxH{100};
        const float x = startX + i * (boxW + spacing);

        const float w = boxW * pulse;
        const float h = boxH * pulse;

        const float ox = (w - boxW) * 0.5f;
        const float oy = (h - boxH) * 0.5f;

        const Rectangle rect = {
            x - ox,
            y - oy,
            w,
            h
        };

        const bool hovered = CheckCollisionPointRec(GetMousePosition(), rect);

        const Color base = hovered ? YELLOW : Fade(DARKGRAY, 0.9f);

        DrawRectangleRec(rect, base);
        DrawRectangleLinesEx(rect, 2, hovered ? YELLOW : WHITE);

        // subtle highlight
        if (hovered)
            DrawRectangleRec(rect, base);

        DrawText(
            world.options[i].name,
            static_cast<int>(rect.x) + 10,
            static_cast<int>(rect.y) + 30,
            fontSize,
            WHITE
        );

        DrawText(
            TextFormat("[ %d ]", i + 1),
            static_cast<int>(rect.x) + 10,
            static_cast<int>(rect.y) + 65,
            fontSize,
            GRAY
        );
    }
}

void UI::DrawMenuButton(const Rectangle rect, const char* text)
{
    const bool hovered =
        CheckCollisionPointRec(
            GetMousePosition(),
            rect
        );

    const Color fill =
        hovered
        ? Fade(DARKGRAY, 0.9f)
        : Fade(BLACK, 0.7f);

    DrawRectangleRec(rect, fill);
    DrawRectangleLinesEx(rect, 2, WHITE);

    int textWidth =
        MeasureText(text, fontSize);

    DrawText(
        text,
        static_cast<int>(rect.x + (rect.width - textWidth) * 0.5f),
        static_cast<int>(rect.y + (rect.height - fontSize) * 0.5f),
        fontSize,
        WHITE
    );
}

void UI::DrawTimer(const World& world)
{
    // get seconds
    int seconds = static_cast<int>(world.GetSurvivalTime());
    // get minutes
    const int minutes = seconds / 60;
    // seconds
    seconds %= 60;

    // length of screen
    const int screenWidth = GetScreenWidth();

    // text to display
    const char* text = TextFormat("%02d:%02d", minutes, seconds);
    // text width
    const int textWidth = MeasureText(text, 30);

    DrawText(
        text,
        (screenWidth - textWidth) / 2,
        20,
        30,
        RAYWHITE
    );
}
