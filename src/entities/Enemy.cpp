// #include <iostream>
#include <raylib.h>

#include "Enemy.h"
#include "UI/UI.h"
#include "AssetManager.h"

Enemy::Enemy(Vector2 startPos)
    : position(startPos),
      velocity({0, 0}),
      speed(100.0f),
      maxHealth(50),
      health(50)
{

}

void Enemy::Update(float deltaTime, Vector2 playerPos)
{
    // -------------------------
    // timers
    // -------------------------
    if (hitFlashTimer > 0.0f)
        hitFlashTimer -= deltaTime;

    if (freezeTimer > 0.0f)
        freezeTimer -= deltaTime;

    // -------------------------
    // update damage numbers
    // -------------------------
    for (auto &d : damageNumbers)
    {
        d.timer -= deltaTime;
        d.pos.y -= 30.0f * deltaTime;
        d.alpha = d.timer / 0.6f;
    }

    damageNumbers.erase(
        std::remove_if(damageNumbers.begin(), damageNumbers.end(),
        [](DamageNumber &d) { return d.timer <= 0.0f; }),
        damageNumbers.end()
    );

    // -------------------------
    // KNOCKBACK MOVEMENT
    // -------------------------
    position = Vector2Add(position, Vector2Scale(velocity, deltaTime));

    // apply friction (important or they slide forever)
    velocity = Vector2Scale(velocity, 0.90f);

    // -------------------------
    // freeze = no AI movement
    // -------------------------
    if (freezeTimer > 0.0f)
        return;

    // -------------------------
    // AI MOVE TOWARD PLAYER
    // -------------------------
    Vector2 direction = Vector2Subtract(playerPos, position);

    if (Vector2Length(direction) > 0.0f)
    {
        direction = Vector2Normalize(direction);

        position.x += direction.x * speed * deltaTime;
        position.y += direction.y * speed * deltaTime;
    }

    // -------------------------
    // UPDATE FACING (FIX)
    // -------------------------
    Vector2 toPlayer = Vector2Subtract(playerPos, position);

    if (Vector2Length(toPlayer) > 0.0f)
    {
        toPlayer = Vector2Normalize(toPlayer);

        // only flip direction (do NOT use for movement)
        facingLeft = (toPlayer.x < 0.0f);
    }

    // -------------------------
    // HIT FLASH TIMER
    // -------------------------
    if (hitFlashTimer > 0.0f)
        hitFlashTimer -= deltaTime;
}

void Enemy::Draw() const
{
    Vector2 size = {
        (float)AssetManager::EnemyTex.width,
        (float)AssetManager::EnemyTex.height
    };

    Color tint = WHITE;

    if (hitFlashTimer > 0.0f)
        tint = RED;

    Rectangle source = {
        0,
        0,
        facingLeft ? -size.x : size.x, // FLIP LEFT/RIGHT ONLY
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
        tint
    );

    UI::DrawHealthBar(position, health, maxHealth);

    for (const auto &d : damageNumbers)
    {
        DrawText(
            TextFormat("%d", d.value),
            (int)d.pos.x,
            (int)d.pos.y,
            18,
            Fade(RED, d.alpha)
        );
    }
}

Vector2 Enemy::GetPos() const { return position; }

void Enemy::SetPos(Vector2 newPos) { position = newPos; }

float Enemy::GetRadius() const { return 16.0f; }

void Enemy::TakeDamage(int amount, Vector2 hitDir)
{
    health -= amount;
        if (health < 0) health = 0;

    // knockback
    float knockback = 250.0f;

    velocity = Vector2Add(
        velocity,
        Vector2Scale(hitDir, knockback)
    );

    // freeze enemy briefly
    freezeTimer = 0.08f;

    // flash red
    hitFlashTimer = 0.12f;

    // spawn damage number
    DamageNumber d;
    d.value = amount;
    d.pos = position;
    d.timer = 0.6f;
    d.alpha = 1.0f;

    damageNumbers.push_back(d);
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
