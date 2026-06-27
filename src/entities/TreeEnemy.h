#pragma once

#include "Enemy.h"

class TreeEnemy : public Enemy
{
public:
    explicit TreeEnemy(Vector2 startPos);
    [[nodiscard]] const char* GetTextureName() const override
    {
        return "tree_enemy";
    }
protected:
    // tree enemy specific chase behavior - should be different from (bat)
    // but we will do that later
    void UpdateAI(float deltaTime, Vector2 playerPos) override;
};