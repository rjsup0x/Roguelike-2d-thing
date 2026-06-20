#pragma once

#include <raylib.h>
#include <raymath.h>

class Player
{
public:
    Player();

    void Update(float deltaTime);
    void Draw() const;

    Vector2 GetPos() const;
    void SetPos(Vector2 newPos);

    float GetRadius() const;

    int GetHealth() const;
    int GetMaxHealth() const;

    void TakeDamage(int amount);
    bool isDead() const;

    Vector2 GetVelocity() const;

private:
    Vector2 position;
    Vector2 velocity;

    float speed;

    int health;
    int maxHealth;
};
