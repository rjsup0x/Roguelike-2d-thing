#pragma once

#include "Enemy.h"

class BatEnemy : public Enemy
{
public:
    BatEnemy(Vector2 startPos);

    void Update(float dt, Vector2 playerPos) override;
};
