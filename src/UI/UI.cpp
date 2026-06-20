#include "UI.h"
#include <raylib.h>
#include <raymath.h>

// =========================
// WORLD SPACE HEALTH BAR
// =========================
void UI::DrawHealthBar(Vector2 position, int health, int maxHealth)
{
    const float barWidth = 6.0f;
    const float barHeight = 32.0f;

    float healthPercent = Clamp(
        (float)health / (float)maxHealth,
        0.0f,
        1.0f
    );

    // small gap from texture
    Vector2 barPos = {
        position.x + -20.0f,
        position.y - barHeight * 0.5f
    };

    float fillHeight = barHeight * healthPercent;

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

    int fontSize = 6;
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
    float barWidth = 400.0f;
    float barHeight = 12.0f;

    float screenX = GetScreenWidth() * 0.5f - barWidth * 0.5f;
    float screenY = GetScreenHeight() - 40;

    float percent = (float)xp / (float)maxXP;
    if (percent > 1.0f) percent = 1.0f;

    // background
    DrawRectangle(screenX, screenY, barWidth, barHeight, DARKGRAY);

    // fill
    DrawRectangle(screenX, screenY, barWidth * percent, barHeight, SKYBLUE);

    // text LEFT (level)
    DrawText(
        TextFormat("LV %d", level),
        screenX - 60,
        screenY - 2,
        12,
        WHITE
    );

    // text RIGHT (next level)
    DrawText(
        "NEXT LEVEL",
        screenX + barWidth + 10,
        screenY - 2,
        12,
        WHITE
    );
}
