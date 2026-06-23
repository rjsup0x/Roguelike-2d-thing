#include "UI.h"
#include <raylib.h>
#include <raymath.h>

// =========================
// WORLD SPACE HEALTH BAR
// =========================
void UI::DrawHealthBar(Vector2 position, int health, int maxHealth)
{
    const float barWidth{6.0f};
    const float barHeight{32.0f};

    float healthPercent = Clamp(
        (float)health / (float)maxHealth,
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
        (unsigned char)(255 * (1.0f - healthPercent)),
        (unsigned char)(255 * healthPercent),
        0,
        255
    };

    // =========================
    // BAR BACKGROUND
    // =========================
    DrawRectangle(
        (int)barPos.x,
        (int)barPos.y,
        (int)barWidth,
        (int)barHeight,
        DARKGRAY
    );

    // =========================
    // BAR FILL
    // =========================
    DrawRectangle(
        (int)barPos.x,
        (int)(barPos.y + barHeight - fillHeight),
        (int)barWidth,
        (int)fillHeight,
        fillColor
    );

    // =========================
    // WORLD TEXT
    // =========================

    const char* text = TextFormat("%d / %d", health, maxHealth);

    int textWidth = MeasureText(text, fontSize);

    DrawText(
        text,
        (int)(barPos.x + (barWidth / 2) - ((float)textWidth / 2)),
        (int)(barPos.y - 14), // ABOVE the bar
        fontSize,
        BLACK
    );
}

void UI::DrawXPBar(int xp, int maxXP, int level)
{
    float barWidth = GetScreenWidth() - 200;
    float barHeight{12.0f};

    float screenX{GetScreenWidth() * 0.5f - barWidth * 0.5f};
    float screenY = GetScreenHeight() - 40;

    float percent{(float)xp / (float)maxXP};
    if (percent > 1.0f) percent = 1.0f;

    // background
    DrawRectangle(screenX, screenY, barWidth, barHeight, DARKGRAY);

    // fill
    DrawRectangle(screenX, screenY, barWidth * percent, barHeight, SKYBLUE);

    // text LEFT (level)
    DrawText(
        TextFormat("LVL: %d", level),
        screenX - 40,
        screenY,
        fontSize,
        BLACK
    );

    // text RIGHT (next level)
    DrawText(
        "NEXT LEVEL",
        screenX + barWidth + 10,
        screenY,
        fontSize,
        BLACK
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
        (float)panelW,
        (float)panelH
    };

    int boxW{160};
    int boxH{100};
    int spacing{20};

    float startX = panel.x + (panelW - (boxW * 3 + spacing * 2)) * 0.5f;
    float y = panel.y + 80;

    for (int i = 0; i < 3; i++)
    {
        Rectangle rect = {
            startX + i * (boxW + spacing),
            y,
            (float)boxW,
            (float)boxH
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

void UI::DrawLevelUp(World& world)
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
        (float)panelW,
        (float)panelH
    };

    // ONLY panel darkening (not full screen)
    DrawRectangleRec(panel, Fade(BLACK, 0.25f));
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

        bool hovered = CheckCollisionPointRec(GetMousePosition(), rect);

        Color base = hovered ? DARKGRAY : Fade(DARKGRAY, 0.9f);

        DrawRectangleRec(rect, base);
        DrawRectangleLinesEx(rect, 2, hovered ? YELLOW : WHITE);

        // subtle highlight
        if (hovered)
            DrawRectangleRec(rect, Fade(YELLOW, 0.2f));

        DrawText(
            world.options[i].name,
            (int)rect.x + 10,
            (int)rect.y + 30,
            fontSize,
            WHITE
        );

        DrawText(
            TextFormat("[ %d ]", i + 1),
            (int)rect.x + 10,
            (int)rect.y + 65,
            fontSize,
            GRAY
        );
    }
}

void UI::DrawMenuButton(Rectangle rect, const char* text)
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

    int fontSize = 24;

    int textWidth =
        MeasureText(text, fontSize);

    DrawText(
        text,
        (int)(rect.x + (rect.width - textWidth) * 0.5f),
        (int)(rect.y + (rect.height - fontSize) * 0.5f),
        fontSize,
        WHITE
    );
}

void UI::DrawTimer(const World& world)
{
    // get seconds
    int seconds = (int)world.GetSurvivalTime();
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
