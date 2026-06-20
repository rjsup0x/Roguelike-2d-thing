// #include <iostream>
#include <raylib.h>

#include "Enemy.h"
#include "UI/UI.h"
#include "AssetManager.h"

Enemy::Enemy(Vector2 startPos)
    : position(startPos),
      speed(100.0f),
      maxHealth(50),
      health(50)
{
}

void Enemy::Update(float deltaTime, Vector2 playerPos)
{
    // =========================
    // 1. NORMAL AI DIRECTION
    // =========================
    Vector2 toPlayer = Vector2Subtract(playerPos, position);

    if (Vector2Length(toPlayer) > 0.0f)
    {
        toPlayer = Vector2Normalize(toPlayer);

        // move toward player
        position.x += toPlayer.x * speed * deltaTime;
        position.y += toPlayer.y * speed * deltaTime;

        // =========================
        // FACE PLAYER (IMPORTANT FIX)
        // =========================
        facingLeft = (toPlayer.x < 0.0f);
    }
}

void Enemy::Draw() const
{
    Vector2 size = {
        (float)AssetManager::EnemyTex.width,
        (float)AssetManager::EnemyTex.height
    };

    Rectangle source = {
        0,
        0,
        facingLeft ? -size.x : size.x, // FLIP HERE
        size.y
    };

    Rectangle dest = {
        position.x,
        position.y,
        size.x,
        size.y
    };

    Vector2 origin = {
        size.x / 2.0f,
        size.y / 2.0f
    };

    DrawTexturePro(
        AssetManager::EnemyTex,
        source,
        dest,
        origin,
        0.0f,
        WHITE
    );

    UI::DrawHealthBar(position, health, maxHealth);
}

Vector2 Enemy::GetPos() const { return position; }

void Enemy::SetPos(Vector2 newPos) { position = newPos; }

float Enemy::GetRadius() const { return 16.0f; }

void Enemy::TakeDamage(int amount)
{
    health -= amount;
    if (health < 0) health = 0;
}

bool Enemy::isDead() const
{
    return health <= 0;
}

int Enemy::GetHealth() const { return health; }

void Enemy::SetStats(int hp, float spd)
{
    health = hp;
    maxHealth = hp;
    speed = spd;
}
