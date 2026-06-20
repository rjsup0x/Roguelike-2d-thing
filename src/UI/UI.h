#pragma once

#include <raylib.h>

class UI
{
public:
    // WORLD SPACE (follows entities, inside camera)
    static void DrawHealthBar(Vector2 worldPosition, int health, int maxHealth);

    static void DrawPlayerHealthHUD(int health, int maxHealth);

    // SCREEN SPACE (HUD)
    // static void DrawHUDHealthBar(Vector2 screenPosition, int health, int maxHealth);
    //
    static void DrawXPBar(int xp, int maxXP, int level);
};
