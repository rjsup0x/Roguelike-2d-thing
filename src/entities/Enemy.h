#pragma once

#include "raylib.h"
#include <raymath.h>

// enemy and possible attributes


class Enemy
{
  public:
    Enemy(Vector2 startPos);

    void Update(float deltaTime, Vector2 playerPos);
    void Draw() const;

    Vector2 GetPos() const;

    float GetRadius() const;

    void TakeDamage(int amount);

    bool isDead() const;

    int GetHealth() const;

    void SetStats(int hp, float spd);

  private:
    Vector2 position;
    float speed;

    int health;
    int maxHealth;
};
