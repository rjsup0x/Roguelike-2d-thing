#include "UI.h"
#include <raylib.h>
#include <raymath.h>

// =========================
// WORLD SPACE HEALTH BAR
// =========================
void UI::DrawHealthBar(Vector2 position, int health, int maxHealth)
{
    constexpr float barWidth{6.0f};
    constexpr float barHeight{32.0f};

    float healthPercent = Clamp(
        static_cast<float>(health) / static_cast<float>(maxHealth),
        0.0f,
        1.0f
    );

    // small gap from texture
    Vector2 barPos = {
        position.x + -26.0f,
        position.y - barHeight * 0.5f
    };

    float fillHeight{barHeight * healthPercent};

    Color fillColor = {
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

    int textWidth = MeasureText(text, fontSize);

    DrawText(
        text,
        static_cast<int>(barPos.x + (barWidth / 2) - (static_cast<float>(textWidth) / 2)),
        static_cast<int>(barPos.y - 14), // ABOVE the bar
        fontSize,
        BLACK
    );
}

void UI::DrawXPBar(int xp, int maxXP, int level)
{
    float screenW = GetScreenWidth();
    float screenH = GetScreenHeight();

    // =========================
    // HUD BACKGROUND PANEL
    // =========================
    float panelHeight = 60.0f;
    float panelY = screenH - panelHeight;

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
    float barWidth = screenW - 200;
    float barHeight = 12.0f;

    float screenX = screenW * 0.5f - barWidth * 0.5f;

    // move bar down INTO the panel
    float screenY = panelY + (panelHeight * 0.5f) - (barHeight * 0.5f);

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

    Vector2 mouse = GetMousePosition();

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

    int boxW{160};
    int spacing{20};

    float startX = panel.x + (panelW - (boxW * 3 + spacing * 2)) * 0.5f;
    float y = panel.y + 80;

    for (int i = 0; i < 3; i++)
    {
        int boxH{100};
        Rectangle rect = {
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

    float time = GetTime();
    float pulse = 1.0f + sinf(time * 6.0f) * 0.03f;

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

    // ONLY panel darkening (not full screen)
    DrawRectangleRec(panel, Fade(BLACK, 0.25f));
    DrawRectangleLinesEx(panel, 2, WHITE);

    DrawText("LEVEL UP!", panel.x + 20, panel.y + 15, 28, WHITE);

    int boxW{160};
    int spacing{20};

    float startX = panel.x + (panelW - (boxW * 3 + spacing * 2)) * 0.5f;
    float y = panel.y + 80;

    for (int i = 0; i < 3; i++)
    {
        constexpr int boxH{100};
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

        bool hovered = CheckCollisionPointRec(GetMousePosition(), rect);

        Color base = hovered ? YELLOW : Fade(DARKGRAY, 0.9f);

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
    bool hovered =
        CheckCollisionPointRec(
            GetMousePosition(),
            rect
        );

    Color fill =
        hovered
        ? Fade(DARKGRAY, 0.9f)
        : Fade(BLACK, 0.7f);

    DrawRectangleRec(rect, fill);
    DrawRectangleLinesEx(rect, 2, WHITE);

    int fontSize{24};

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
    int minutes = seconds / 60;
    // seconds
    seconds %= 60;

    // length of screen
    int screenWidth = GetScreenWidth();

    // text to display
    const char* text = TextFormat("%02d:%02d", minutes, seconds);
    // text width
    int textWidth = MeasureText(text, 30);

    DrawText(
        text,
        (screenWidth - textWidth) / 2,
        20,
        30,
        BLACK
    );
}
