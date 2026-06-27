#pragma once

#include "Enemy.h"

class BatEnemy : public Enemy
{
public:
    explicit BatEnemy(Vector2 startPos);

    [[nodiscard]] const char* GetTextureName() const override
    {
        return "enemy";
    }

protected:
    // Bat-specific chase behavior. Called by Enemy::Update() once per
    // frame, only while not frozen — no need to check freezeTimer here.
    void UpdateAI(float deltaTime, Vector2 playerPos) override;
};
