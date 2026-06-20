#include <iostream>
#include <raylib.h>

#include "Enemy.h"
#include "UI/UI.h"
#include "AssetManager.h"

// init enemy constructor
Enemy::Enemy(Vector2 startPos)
    : position(startPos),
      speed(100.0f),
      maxHealth(50),
      health(50)
{
    std::cout << "Enemy Initialized\n";
}

// enemy ai
void Enemy::Update(float deltaTime, Vector2 playerPos)
{
    // enemy moves towards player postion
    Vector2 direction = Vector2Subtract(playerPos, position);

    if (Vector2Length(direction)  > 0.0f)
    {
        direction = Vector2Normalize(direction);

        position.x += direction.x * speed * deltaTime;
        position.y += direction.y * speed * deltaTime;
    }
}

void Enemy::Draw() const
{
    Vector2 size = {
        (float)AssetManager::EnemyTex.width,
        (float)AssetManager::EnemyTex.height
    };

    DrawTextureV(
        AssetManager::EnemyTex,
        {
            position.x - size.x / 2.0f,
            position.y - size.y / 2.0f
        },
        WHITE
    );

    UI::DrawHealthBar(position, health, maxHealth);
}

Vector2 Enemy::GetPos() const
{
    return position;
}

float Enemy::GetRadius() const
{
    return 16.0f;
}

void Enemy::TakeDamage(int amount)
{
    health -= amount;

    if (health < 0)
    {
        health = 0;
    }
}

bool Enemy::isDead() const
{
    return health <= 0;
}

int Enemy::GetHealth() const
{
    return health;
}

void Enemy::SetStats(int hp, float spd)
{
    health = hp;
    maxHealth = hp;
    speed = spd;
}
