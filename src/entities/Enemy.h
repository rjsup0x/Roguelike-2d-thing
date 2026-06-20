#pragma once

#include "raylib.h"
#include <raymath.h>

class Enemy
{
public:
    Enemy(Vector2 startPos);

    void Update(float deltaTime, Vector2 playerPos);
    void Draw() const;

    Vector2 GetPos() const;
    void SetPos(Vector2 newPos);

    float GetRadius() const;

    void TakeDamage(int amount, Vector2 hitDir);
    bool isDead() const;

    int GetHealth() const;
    void SetStats(int hp, float spd);

private:
    Vector2 position;
    Vector2 velocity;

    float speed;

    int health;
    int maxHealth;

    float separationRadius = 28.0f;

    float rotation = 0.0f;
    bool facingLeft = false;

    float hitFlashTimer = 0.0f;
};
