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
        (int)(barPos.x + (barWidth / 2) - (textWidth / 2)), // centered above bar
        (int)(barPos.y - 14), // ABOVE the bar
        fontSize,
        BLACK
    );
}
