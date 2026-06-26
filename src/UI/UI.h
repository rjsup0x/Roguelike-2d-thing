#pragma once

#include <raylib.h>
#include "World.h"

class UI
{
public:
    // WORLD SPACE (follows entities, inside camera)
    static void DrawHealthBar(Vector2 worldPosition, int health, int maxHealth);

    // static void DrawPlayerHealthHUD(int health, int maxHealth);

    // SCREEN SPACE (HUD)
    // static void DrawHUDHealthBar(Vector2 screenPosition, int health, int maxHealth);
    //
    static void DrawXPBar(int xp, int maxXP, int level);

    //
    static void DrawLevelUp(const World& world);
    static void HandleLevelUpInput(World& world);

    static void DrawMenuButton(Rectangle rect, const char* text);

    static void DrawTimer(const World& world);

    // Was 6 — at that size, health numbers and upgrade-box text are
    // basically unreadable. Bumped to a size that's actually legible and
    // closer to what the rest of the UI uses elsewhere (12-30px).
    static constexpr int fontSize{16};
};
